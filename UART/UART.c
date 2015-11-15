#include "UART.h"
#include <avr/interrupt.h>
#include "../Buffer/Buffer.h"

Buffer UARTRxBuffer;//Buffer di ricezione
Buffer UARTTxBuffer;//Buffer di trasmissione

void UARTRxInterruptEnabled();//Abilita l'interrupt per la ricezione di un byte
void UARTRxInterruptDisabled();//Disabilita l'interrupt per la ricezione di un byte
void UARTTxInterruptEnabled();//Abilita l'interrupt per la conclusione di invio di un byte
void UARTTxInterruptDisabled();//Disabilita l'interrupt per la conclusione di invio di un byte

void UARTReceiveData();//Inizia la ricezione di un byte
void UARTSendData();//Inizia la trasmissione di un byte

void UARTInit(uint16_t ubrr, uint8_t RxBufferArray[], uint8_t RxBufferArraySize, uint8_t TxBufferArray[], uint8_t TxBufferArraySize)
{
	UCSR0B = (1<<RXEN0) | (1<<TXEN0); //Abilito il pin di ricezione e il pin di trasmissione
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);//Imposto a 8 la lunghezza in bit di un carattere
	
	UBRR0H = ubrr >> 8;
	UBRR0L = ubrr;
	
	BufferInit(&UARTRxBuffer, RxBufferArray, RxBufferArraySize);//Inizializzo il buffer di ricezione
	BufferInit(&UARTTxBuffer, TxBufferArray, TxBufferArraySize);//Inizializzo il buffer di trasmissione
	
	UARTRxInterruptEnabled();//Abilito l'interrupt per la ricezione di un byte
	UARTTxInterruptEnabled();//Abilito l'interrupt per la conclusione di invio di un byte
	
	sei();//Abilito gli interrupt globali
}

void UARTSetUBRR(uint16_t ubrr)
{
	UBRR0H = ubrr >> 8;
	UBRR0L = ubrr;
}

void UARTDoubleSpeedEnabled()
{
	UCSR0A |= 1 << U2X0;
}
void UARTDoubleSpeedDisabled()
{
	UCSR0A &= ~(1 << U2X0);
}

void UARTRxInterruptEnabled()
{
	UCSR0B |= 1 << RXCIE0;
}
void UARTRxInterruptDisabled()
{
	UCSR0B &= ~(1 << RXCIE0);
}
void UARTTxInterruptEnabled()
{
	UCSR0B |= 1 << TXCIE0;
}
void UARTTxInterruptDisabled()
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
	UARTTxInterruptDisabled();//Disabilito l'interrupt per la conclusione di invio di un byte
	
	BufferPush(&UARTTxBuffer, value);//Inserisco il byte nel buffer di trasmissione
	
	UARTTxInterruptEnabled();//Abilito l'interrupt per la conclusione di invio di un byte
	
	UARTSendData();//Rinizio la procedura di invio dei dati nel buffer
}
void UARTTxString(char* s)
{
	while(*s != '\0')//Eseguo il ciclo fintanto che il carattere corrente non è '\0' (fine stringa)
	{
		UARTTx(*s);//Invio il carattere corrente
		s++;//Sposto il puntatore al carattere corrente a quello successivo
	}	
}
void UARTTxArray(uint8_t array[], uint16_t n)
{
	if(BufferFreeSpace(&UARTTxBuffer) < n)//In caso nel buffer di trasmissione non vi sia spazio sufficiente per trasmettere tutto l'array in una volta sola
	{
		for(; n != 0; n--)//Eseguo il ciclo per il numero di byte da copiare
		{
			UARTTx(*array);
			array++;//Sposto il puntatore all'array avanti di un byte
		}
	}
	else//Se invece nel buffer di trasmissione vi è spazio sufficiente per trasmettere tutto l'array in una volta sola
	{
		UARTTxInterruptDisabled();//Disabilito l'interrupt per la conclusione di invio di un byte
		
		for(; n != 0; n--)//Eseguo il ciclo per il numero di byte da copiare
		{
			BufferPush(&UARTTxBuffer, *array);
			array++;//Sposto il puntatore all'array avanti di un byte
		}
		
		UARTTxInterruptEnabled();//Abilito l'interrupt per la conclusione di invio di un byte

		UARTSendData();//Rinizio la procedura di invio dei dati nel buffer
	}
}

void UARTReceiveData()
{
	uint8_t waste;//Alloco un byte in caso dovessi eliminare dei dati
	if(BufferIsFull(&UARTRxBuffer)) waste = UDR0;//Se il Buffer di ricezione è pieno scarto il valore ricevuto
	else BufferPush(&UARTRxBuffer, UDR0);//Altrimenti inserisco il byte ricevuto nel buffer di ricezione
}
void UARTSendData()
{
	if(!BufferIsEmpty(&UARTTxBuffer))//Verifico che il buffer di trasmissione non sia vuoto
	{
		while(!(UCSR0A & (1 << UDRE0)));//Attendo che il buffer sia pronto
		UDR0 = BufferPull(&UARTTxBuffer);//Prelevo dal buffer di trasmissione ed invio il dato ad 8 bit
	}
}

ISR(USART_RX_vect) { UARTReceiveData(); }//Al verificarsi dell'interrupt che segnala la fine della ricezione di un byte inizio la ricezione software dello stesso
ISR(USART_TX_vect) { UARTSendData(); }//Al verificarsi dell'interrupt che segnala la fine della trasmissione di un byte inizio la procedura per inviare un altro byte