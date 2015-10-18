#ifndef BUFFER_H
#define BUFFER_H

#include <avr/io.h>

typedef struct
{
	uint16_t size;//Dimensione in byte del buffer
	uint8_t* array;//Puntatore alla prima cella di memoria allocata
	uint8_t* firstElement;//Puntatore al primo elemento inserito
	uint8_t* firstSpace;//Puntatore al primo spazio libero
} Buffer;

uint8_t BufferInit(Buffer*, uint8_t[], uint16_t);//Inizializza il buffer e restituisce 1 se l'operazione è riuscita

uint8_t BufferIsEmpty(Buffer*);//Verifica che il buffer sia vuoto
uint8_t BufferIsFull(Buffer*);//Verfica che il buffer sia pieno
uint16_t BufferCount(Buffer*);//Restituisce il numero di elementi presenti nel buffer
uint16_t BufferFreeSpace(Buffer*);//Restituisce lo spazio disponibile nel buffer

uint8_t BufferPush(Buffer*, uint8_t);//Inserisce un byte nel buffer
uint8_t BufferPull(Buffer*);//Preleva e restituisce il primo byte inserito (FIFO)
uint8_t BufferPop(Buffer*);//preleva e restituisce l'ultimo byte inserito (LIFO)

void BufferFlush(Buffer*);//Elimina tutti gli elementi dal buffer

#endif