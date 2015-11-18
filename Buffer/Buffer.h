#pragma once

#include <avr/io.h>

typedef struct
{
	uint16_t size;//Dimensione in byte del buffer
	uint8_t* array;//Puntatore alla prima cella di memoria allocata
	uint8_t* firstElement;//Puntatore al primo elemento inserito
	uint8_t* firstSpace;//Puntatore al primo spazio libero
} Buffer;

void BufferInit(Buffer*, uint8_t[], uint16_t);//(buffer, array, size)//Inizializza il buffer {buffer} con l'array di byte {array} di dimensione {size}

uint8_t BufferIsEmpty(Buffer*);//(buffer)//Verifica che il buffer {buffer} sia vuoto
uint8_t BufferIsFull(Buffer*);//(buffer)//Verfica che il buffer {buffer} sia pieno
uint16_t BufferCount(Buffer*);//(buffer)//Restituisce il numero di elementi presenti nel buffer {buffer}
uint16_t BufferFreeSpace(Buffer*);//(buffer)//Restituisce lo spazio disponibile nel buffer {buffer}

uint8_t BufferPush(Buffer*, uint8_t);//(buffer, value)//Inserisce nel buffer {buffer} il byte {value}
uint8_t BufferPull(Buffer*);//(buffer)//preleva dal buffer {buffer} il byte meno recente e lo restituisce (FIFO)
uint8_t BufferPop(Buffer*);//(buffer)//preleva dal buffer {buffer} il byte piu recente e lo restituisce (LIFO)

void BufferFlush(Buffer*);//(buffer)//Svuota il buffer {buffer}