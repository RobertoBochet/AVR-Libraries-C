#pragma once

#include <avr/io.h>

typedef struct buffer_t
{
	uint16_t size;//Dimensione in byte del buffer
	char* array;//Puntatore alla prima cella di memoria allocata
	char* firstElement;//Puntatore al primo elemento inserito
	char* firstSpace;//Puntatore al primo spazio libero
} Buffer_t;

void BufferInit(Buffer_t*, void*, uint16_t);//(buffer, array, size)//Inizializza il buffer {buffer} con l'array di byte {array} di dimensione {size}

uint8_t BufferIsEmpty(Buffer_t*);//(buffer)//Verifica che il buffer {buffer} sia vuoto
uint8_t BufferIsFull(Buffer_t*);//(buffer)//Verfica che il buffer {buffer} sia pieno
uint16_t BufferCount(Buffer_t*);//(buffer)//Restituisce il numero di elementi presenti nel buffer {buffer}
uint16_t BufferFreeSpace(Buffer_t*);//(buffer)//Restituisce lo spazio disponibile nel buffer {buffer}

uint8_t BufferPush(Buffer_t*, char);//(buffer, value)//Inserisce nel buffer {buffer} il byte {value}
char BufferPull(Buffer_t*);//(buffer)//preleva dal buffer {buffer} il byte meno recente e lo restituisce (FIFO)
char BufferPop(Buffer_t*);//(buffer)//preleva dal buffer {buffer} il byte piu recente e lo restituisce (LIFO)

void BufferFlush(Buffer_t*);//(buffer)//Svuota il buffer {buffer}