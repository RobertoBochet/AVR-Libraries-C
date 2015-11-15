#pragma once

#include <avr/io.h>

typedef struct
{
	float* beginningArray;//Puntatore al primo elemento dell'array
	float* endArray;//Puntatore all'ultimo elemento dell'array
	float* oldestElement;//Puntatore all'elemento meno recente dell'array
	uint32_t elements;//Numero di elementi della media
	double sum;//Sommatoria degli elementi della media
} SMA;

void SMAInit(SMA*, float[], uint32_t);//(sma, array, size)//Inizializza la media mobile semplice {sma} con {array} di {size} elementi
void SMAAdd(SMA*, float);//(sma, value)//Aggiunge come nuovo valore {value} alla media {sma}
float SMAGet(SMA*);//(sma)//Restituisce la media di {sma}
void SMAClear(SMA*);//(sma)//Resetta la media di {sma}


typedef struct
{
	float* beginningArray;//Puntatore al primo elemento dell'array
	float* endArray;//Puntatore all'ultimo elemento dell'array
	float* oldestElement;//Puntatore all'elemento meno recente dell'array
	uint32_t elements;//Numero di elementi della media
} WMA;

void WMAInit(WMA*, float[], uint32_t);//(wma, array, size)//Inizializza la media mobile ponderata {wma} con {array} di {size} elementi
void WMAAdd(WMA*, float);//(wma, value)//Aggiunge come nuovo valore {value} alla media {wma}
float WMAGet(WMA*);//(wma)//Restituisce la media di {wma}
void WMAClear(WMA*);//(wma)//Resetta la media di {wma}