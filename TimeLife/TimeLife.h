#pragma once

#include <avr/io.h>

void MillisSetup(uint32_t);//(timerFrequency)//Inizializza la funzione Millis accetando come paramentro {timerFrequency} la frequenza del timer
void MillisReset();//Resetta i contatori della Millis

uint64_t Millis();//Restituisce i millisecondi passati nell'inizializzazione di Millis