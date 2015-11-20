#include "UART.h"
#include <avr/interrupt.h>
#include "../Buffer/Buffer.h"

Buffer UARTRxBuffer;//Buffer di ricezione
Buffer UARTTxBuffer;//Buffer di trasmissione

inline void UARTRxInterruptEnabled();//Abilita l'interrupt per la ricezione di un byte
inline void UARTRxInterruptDisabled();//Disabilita l'interrupt per la ricezione di un byte
inline void UARTTxInterruptEnabled();//Abilita l'interrupt per la conclusione di invio di un byte
inline void UARTTxInterruptDisabled();//Disabilita l'interrupt per la conclusione di invio di un byte

inline void UARTReceiveData();//Inizia la ricezione di un byte
inline void UARTSendData();//Inizia la trasmissione di un byte

void UARTInit(uint16_t ubrr, uint8_t RxBufferArray[], uint8_t RxBufferArraySize, uint8_t TxBufferArray[], uint8_t TxBufferArraySize)
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

inline void UARTSetUBRR(uint16_t ubrr)
{
	UBRR0H = ubrr >> 8;
	UBRR0L = ubrr;
}

inline void UARTDoubleSpeedEnabled()
{
	UCSR0A |= 1 << U2X0;
}
inline void UARTDoubleSpeedDisabled()
{
	UCSR0A &= ~(1 << U2X0);
}

inline void UARTRxInterruptEnabled()
{
	UCSR0B |= 1 << RXCIE0;
}
inline void UARTRxInterruptDisabled()
{
	UCSR0B &= ~(1 << RXCIE0);
}
inline void UARTTxInterruptEnabled()
{
	UCSR0B |= 1 << TXCIE0;
}
inline void UARTTxInterruptDisabled()
{
	UCSR0B &= ~(1 << TXCIE0);
}

uint8_t UARTRxAvailable()
{
	return BufferCount(&UARTRxBuffer);//Restituisco il numero di byte presenti nel buffer di ricezione
}

uint8_t UARTRx()
{
	return BufferPull(&UARTRxBuffer);//Restituisco il primo byte ricevuto
}
void UARTRxArray(uint8_t array[], uint16_t n)
{
	for(; n != 0; n--)//Eseguo il ciclo per il numero di byte da copiare
	{
		*array = BufferPull(&UARTRxBuffer);//Prelevo e inserisco il byte ricevuto meno recente nell'array 
		array++;//Sposto il puntatore all'array avanti di un byte
	}
}

void UARTTx(uint8_t value)
{
	while(BufferFreeSpace(&UARTTxBuffer) == 0);//Attendo che nel buffer di trasmisione vi sia almeno un byte libero
		
	UARTTxInterruptDisabled();//Disabilito l'interrupt per la conclusione di invio di un byte
	
	BufferPush(&UARTTxBuffer, value);//Inserisco il byte nel buffer di trasmissione
	
	UARTTxInterruptEnabled();//Abilito l'interrupt per la conclusione di invio di un byte
	
	UARTSendData();//Rinizio la procedura di invio dei dati nel buffer
}
void UARTTxString(const char* s)
{
	do {
		while(!BufferIsEmpty(&UARTTxBuffer));//Attendo che il buffer sia completamente vuoto
		
		UARTTxInterruptDisabled();//Disabilito l'interrupt per la conclusione di invio di un byte
		
		do {
			BufferPush(&UARTTxBuffer, *s);//Inserisco il carattere corrente nel buffer di trasmissione
			s++;//Faccio avanzare di un carattere il puntatore al carattere corrente
		} while(*s != '\0' && !BufferIsFull(&UARTTxBuffer));//Continuo fintanto che il carattere corrente è il carattere '\0' di fine stringa o il buffer di trasmissione sia pieno
		
		UARTTxInterruptEnabled();//Abilito l'interrupt per la conclusione di invio di un byte
		
		UARTSendData();//Rinizio la procedura di invio dei dati
	} while(*s != '\0');//Rieseguo il cilco se non è stato ancora incontrato il carattere '\0' di fine stringa
}
void UARTTxArray(const uint8_t array[], uint16_t n)
{
	do {		
		while(BufferFreeSpace(&UARTTxBuffer) < n && !BufferIsEmpty(&UARTTxBuffer));//Attendo che nel buffer vi sia spazio sufficiente per tutti i byte o che sia completamente vuoto
		
		UARTTxInterruptDisabled();//Disabilito l'interrupt per la conclusione di invio di un byte
		
		do {
			BufferPush(&UARTTxBuffer, *array);//Inserisco il byte corrente nel buffer di trasmissione
			n--;//Diminuisco di uno i byte da dover ancora inviare
			array++;//Sposto di un byte in avanti il puntatore al byte corrente
		} while(n != 0 && !BufferIsFull(&UARTTxBuffer));//Continuo fintanto che i byte ancora da inviare siano finiti o il buffer di trasmissione sia pieno
		
		UARTTxInterruptEnabled();//Abilito l'interrupt per la conclusione di invio di un byte
		
		UARTSendData();//Rinizio la procedura di invio dei dati
	} while(n != 0);//Rieseguo il cilco se rimangono ancora byte da inviare
}

inline void UARTReceiveData()
{
	uint8_t waste;//Alloco un byte in caso dovessi eliminare dei dati
	if(BufferIsFull(&UARTRxBuffer)) waste = UDR0;//Se il Buffer di ricezione è pieno scarto il valore ricevuto
	else BufferPush(&UARTRxBuffer, UDR0);//Altrimenti inserisco il byte ricevuto nel buffer di ricezione
}
inline void UARTSendData()
{
	if(!BufferIsEmpty(&UARTTxBuffer))//Verifico che il buffer di trasmissione non sia vuoto
	{
		while(!(UCSR0A & (1 << UDRE0)));//Attendo che il buffer sia pronto
		UDR0 = BufferPull(&UARTTxBuffer);//Prelevo dal buffer di trasmissione ed invio il dato ad 8 bit
	}
}

ISR(USART_RX_vect) { UARTReceiveData(); }//Al verificarsi dell'interrupt che segnala la fine della ricezione di un byte inizio la ricezione software dello stesso
ISR(USART_TX_vect) { UARTSendData(); }//Al verificarsi dell'interrupt che segnala la fine della trasmissione di un byte inizio la procedura per inviare un altro byte