#include "UART.h"
#include <avr/interrupt.h>
#include "../Buffer/Buffer.h"

Buffer_t UARTRxBuffer;//Buffer di ricezione
Buffer_t UARTTxBuffer;//Buffer di trasmissione

static inline void UARTRxInterruptEnabled() { UCSR0B |= 1 << RXCIE0; }
static inline void UARTRxInterruptDisabled() { UCSR0B &= ~(1 << RXCIE0); }
static inline void UARTTxInterruptEnabled() { UCSR0B |= 1 << TXCIE0; }
static inline void UARTTxInterruptDisabled() { UCSR0B &= ~(1 << TXCIE0); }
	
static inline void UARTReceiveData();//Inizia la ricezione di un byte
static inline void UARTSendData();//Inizia la trasmissione di un byte

void UARTInit(uint16_t ubrr, void* RxBufferArray, uint8_t RxBufferArraySize, void* TxBufferArray, uint8_t TxBufferArraySize)
{
	UCSR0B = (1<<RXEN0) | (1<<TXEN0); //Abilito il pin di ricezione e il pin di trasmissione
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);//Imposto a 8 la lunghezza in bit di un carattere
	
	UARTSetUBRR(ubrr);
	
	BufferInit(&UARTRxBuffer, RxBufferArray, RxBufferArraySize);//Inizializzo il buffer di ricezione
	BufferInit(&UARTTxBuffer, TxBufferArray, TxBufferArraySize);//Inizializzo il buffer di trasmissione
	
	UARTRxInterruptEnabled();//Abilito l'interrupt per la ricezione di un byte
	UARTTxInterruptEnabled();//Abilito l'interrupt per la conclusione di invio di un byte
	
	sei();//Abilito gli interrupt globali
}

uint8_t UARTRxAvailable()
{
	return BufferCount(&UARTRxBuffer);//Restituisco il numero di byte presenti nel buffer di ricezione
}

char UARTRx()
{
	return BufferPull(&UARTRxBuffer);//Restituisco il primo byte ricevuto
}
void UARTRxArray(void* array, uint16_t n)
{
	while(n != 0)//Eseguo il ciclo per il numero di byte da copiare
	{
		*(char*)array = BufferPull(&UARTRxBuffer);//Prelevo e inserisco il byte ricevuto meno recente nell'array 
		array++;//Sposto il puntatore all'array avanti di un byte
		n--;//Decremento il numero di byte ancora da copiare
	}
}

void UARTTx(char value)
{
	while(BufferFreeSpace(&UARTTxBuffer) == 0);//Attendo che nel buffer di trasmisione vi sia almeno un byte libero
		
	UARTTxInterruptDisabled();//Disabilito l'interrupt per la conclusione di invio di un byte
	
	BufferPush(&UARTTxBuffer, value);//Inserisco il byte nel buffer di trasmissione
	
	UARTTxInterruptEnabled();//Abilito l'interrupt per la conclusione di invio di un byte
	
	UARTSendData();//Rinizio la procedura di invio dei dati nel buffer
}
void UARTTxString(const char* s)
{
	while(*s != '\0')//Rieseguo il ciclo se non è stato ancora incontrato il carattere '\0' di fine stringa
	{
		while(!BufferIsEmpty(&UARTTxBuffer));//Attendo che il buffer sia completamente vuoto
		
		UARTTxInterruptDisabled();//Disabilito l'interrupt per la conclusione di invio di un byte
		
		while(*s != '\0' && !BufferIsFull(&UARTTxBuffer))//Continuo fintanto che il carattere corrente è il carattere '\0' di fine stringa o il buffer di trasmissione sia pieno
		{
			BufferPush(&UARTTxBuffer, *s);//Inserisco il carattere corrente nel buffer di trasmissione
			s++;//Faccio avanzare di un carattere il puntatore al carattere corrente
		}
		
		UARTTxInterruptEnabled();//Abilito l'interrupt per la conclusione di invio di un byte
		
		UARTSendData();//Rinizio la procedura di invio dei dati
	} 
}
void UARTTxArray(const void* array, uint16_t n)
{
	while(n != 0)//Rieseguo il ciclo se rimangono ancora byte da inviare
	{		
		while(BufferFreeSpace(&UARTTxBuffer) < n && !BufferIsEmpty(&UARTTxBuffer));//Attendo che nel buffer vi sia spazio sufficiente per tutti i byte o che sia completamente vuoto
		
		UARTTxInterruptDisabled();//Disabilito l'interrupt per la conclusione di invio di un byte
		
		while(n != 0 && !BufferIsFull(&UARTTxBuffer))//Continuo fintanto che i byte ancora da inviare siano finiti o il buffer di trasmissione sia pieno
		{
			BufferPush(&UARTTxBuffer, *(char*)array);//Inserisco il byte corrente nel buffer di trasmissione
			n--;//Diminuisco di uno i byte da dover ancora inviare
			array++;//Sposto di un byte in avanti il puntatore al byte corrente
		}
		
		UARTTxInterruptEnabled();//Abilito l'interrupt per la conclusione di invio di un byte
		
		UARTSendData();//Rinizio la procedura di invio dei dati
	}
}

static inline void UARTReceiveData()
{
	char waste;//Alloco un byte in caso dovessi eliminare dei dati
	if(BufferIsFull(&UARTRxBuffer)) waste = UDR0;//Se il Buffer di ricezione è pieno scarto il valore ricevuto
	else BufferPush(&UARTRxBuffer, UDR0);//Altrimenti inserisco il byte ricevuto nel buffer di ricezione
}
static inline void UARTSendData()
{
	if(!BufferIsEmpty(&UARTTxBuffer))//Verifico che il buffer di trasmissione non sia vuoto
	{
		while(!(UCSR0A & (1 << UDRE0)));//Attendo che il buffer sia pronto
		UDR0 = BufferPull(&UARTTxBuffer);//Prelevo dal buffer di trasmissione ed invio il dato ad 8 bit
	}
}

ISR(USART_RX_vect) { UARTReceiveData(); }//Al verificarsi dell'interrupt che segnala la fine della ricezione di un byte inizio la ricezione software dello stesso
ISR(USART_TX_vect) { UARTSendData(); }//Al verificarsi dell'interrupt che segnala la fine della trasmissione di un byte inizio la procedura per inviare un altro byte