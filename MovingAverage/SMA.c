#include "MovingAverage.h"

void SMAInit(SMA* sma, float array[], uint32_t size)
{
	sma->oldestElement = sma->beginningArray = array;//Imposto i puntatori all'inizio dell'array e all'elemento meno recente con l'inizio dell'array fornito
	sma->endArray = array + size - 1;//Imposto il puntatore alla fine dell'array attraverso l'inizio dell'array fornito
	sma->elements = sma->sum = 0;//Imposto a 0 il numero di elementi della media e la somma degli stessi
}
void SMAAdd(SMA* sma, float value)
{
	if(sma->beginningArray + sma->elements - 1 == sma->endArray) sma->sum -= *sma->oldestElement;//Se l'array è pieno sotraggo alla somma il valore meno recente prima di rimuoverlo
	else sma->elements++;//In caso contrario incremento di uno gli elementi che compongono la media
	
	*sma->oldestElement = value;//Aggiorno il valore meno recente con il nuovo valore
	sma->sum += value;//Aggiungo il nuovo valore alla somma
	
	if(sma->oldestElement == sma->endArray) sma->oldestElement = sma->beginningArray;//Se l'elemento aggiornato era l'ultimo dell'array fisico porto il puntatore all'elemento meno recente all'inizio dell'array
	else sma->oldestElement++;//In caso contrario porto il puntatore all'elemento meno recente al successivo elemento
}
float SMAGet(SMA* sma)
{
	return sma->sum / sma->elements;//Restituisco la media semplice
}
void SMAClear(SMA* sma)
{
	sma->oldestElement = sma->beginningArray;//Reimposto il puntatore al valore meno recente al primo elemento dell'array
	sma->elements = sma->sum = 0;//Reimposto a 0 il numero degli elementi della media e la somma degli stessi
}