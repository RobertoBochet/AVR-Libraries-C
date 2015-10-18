#include "Buffer.h"
#include <stdlib.h>

uint8_t BufferInit(Buffer* buffer, uint8_t array[], uint16_t size)
{
	if(buffer == NULL || array == NULL || size == 0) return 0;//Se buffer o array puntano a NULL oppure la dimensione dell'array è uguale a 0 restituisco false
	
	buffer->size = size;//Imposto la variabile che indica le dimensioni del buffer
	buffer->array = buffer->firstSpace = array;//Imposto il puntatore all'array e al primo spazio libero
	buffer->firstElement = NULL;//Imposto il puntatore al primo elemento a NULL
	
	return 1;//Restituisco 1 per segnalare l'inizailizzazione riuscita
}

uint8_t BufferIsEmpty(Buffer* buffer)
{
	return buffer->firstElement == NULL;//Se il puntatore al primo elemento punta a NULL segno che il buffer è vuoto restituisco 1
}
uint8_t BufferIsFull(Buffer* buffer)
{
	return buffer->firstElement == buffer->firstSpace;//Se i puntatori del primo elemento e primo spazio libero puntano alla stessa cella segno che il buffer è pieno restituisco 1
}
uint16_t BufferCount(Buffer* buffer)
{
	if(BufferIsEmpty(buffer)) return 0;//Se il buffer è vuoto restituisco 0
	if(BufferIsFull(buffer)) return buffer->size;//Se il buffer è pieno restituisco la dimensione del buffer
	
	if(buffer->firstElement < buffer->firstSpace) return buffer->firstSpace - buffer->firstElement;//Se il puntatore al primo elemento si trova prima del primo spazio libero restituisco la differenza tra i due
	else return buffer->size - (buffer->firstElement - buffer->firstSpace);//Se il puntatore al primo elemento di trova dopo il primo spazio libero restituisco la dimensione del buffer meno la differenza tra il puntatore al primo elemento e il primo spazio libero
}

uint8_t BufferPush(Buffer* buffer, uint8_t value)
{
	if (BufferIsFull(buffer)) return 0;//Se il buffer è pieno restituisco false
	
	if (BufferIsEmpty(buffer)) buffer->firstElement = buffer->firstSpace;//Se il buffer è vuoto imposto il puntatore al primo elemento al primo spazio libero
	*buffer->firstSpace = value;//Imposto il valore nel primo spazio disponibile nel buffer
	buffer->firstSpace++;//Sposto il puntatore una cella in avanti
	if (buffer->array + buffer->size == buffer->firstSpace) buffer->firstSpace = buffer->array;//Se il puntatore sconfina in una zona al di fuori del buffer lo si reimposta alla prima cella allocata
	
	return 1;//Restituisco true se il valore è stato inserito correttamente
}
uint8_t BufferPull(Buffer* buffer)
{
	uint8_t value;
	if (BufferIsEmpty(buffer)) return 0;//Se il buffer è vuoto restituisco 0
	
	value = *buffer->firstElement;//Copio il valore nella variabile che verrà restituita
	buffer->firstElement++;//Sposto il puntatore una cella in avanti
	if (buffer->array + buffer->size == buffer->firstElement) buffer->firstElement = buffer->array;//Se il puntatore sconfina in una zona al di fuori del buffer lo si reimposta alla prima cella allocata
	if (buffer->firstElement == buffer->firstSpace) buffer->firstElement = NULL;//Se i due puntatori finisco a puntare alla stessa cella come se il buffer fosse pieno reimposto il puntatore al primo elemento a null
	
	return value;//Restituisco il valore
}
uint8_t BufferPop(Buffer* buffer)
{
	if (BufferIsEmpty(buffer)) return 0;//Se il buffer è vuoto restituisco 0
	
	if (buffer->firstSpace == buffer->array) buffer->firstSpace += buffer->size - 1;//Se il puntatore al primo spazio libero punta alla prima cella allocata allora lo sposto all'ultima cella allocata
	else buffer->firstSpace--;//In caso contrario sposto il puntatore al primo spazio libero indietro di una cella
	if (buffer->firstElement == buffer->firstSpace) buffer->firstElement = NULL;//Se i due puntatori finisco a puntare alla stessa cella come se il buffer fosse pieno reimposto il puntatore al primo elemento a null
	
	return *buffer->firstSpace;//Restituisco il valore
}

void BufferFlush(Buffer* buffer)
{
	buffer->firstElement = NULL;//Reimposto il puntatore al primo elemento a NULL
	buffer->firstSpace = buffer->array;//Reimposto il puntatore al primo spazio libero al primo elemento dell'array
}
