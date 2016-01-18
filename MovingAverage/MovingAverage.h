#pragma once

#include <avr/io.h>

typedef struct sma_t
{
	float* beginningArray;//Puntatore al primo elemento dell'array
	float* endArray;//Puntatore all'ultimo elemento dell'array
	float* oldestElement;//Puntatore all'elemento meno recente dell'array
	uint32_t elements;//Numero di elementi della media
	double sum;//Sommatoria degli elementi della media
} SMA_t;

void SMAInit(SMA_t*, float[], uint32_t);//(sma, array, size)//Inizializza la media mobile semplice {sma} con {array} di {size} elementi
void SMAAdd(SMA_t*, float);//(sma, value)//Aggiunge come nuovo valore {value} alla media {sma}
float SMAGet(SMA_t*);//(sma)//Restituisce la media di {sma}
void SMAClear(SMA_t*);//(sma)//Resetta la media di {sma}


typedef struct wma_t
{
	float* beginningArray;//Puntatore al primo elemento dell'array
	float* endArray;//Puntatore all'ultimo elemento dell'array
	float* oldestElement;//Puntatore all'elemento meno recente dell'array
	uint32_t elements;//Numero di elementi della media
} WMA_t;

void WMAInit(WMA_t*, float[], uint32_t);//(wma, array, size)//Inizializza la media mobile ponderata {wma} con {array} di {size} elementi
void WMAAdd(WMA_t*, float);//(wma, value)//Aggiunge come nuovo valore {value} alla media {wma}
float WMAGet(WMA_t*);//(wma)//Restituisce la media di {wma}
void WMAClear(WMA_t*);//(wma)//Resetta la media di {wma}