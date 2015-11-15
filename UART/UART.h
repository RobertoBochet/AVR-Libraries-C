#pragma once

#include <avr/io.h>

void UARTInit(uint16_t, uint8_t[], uint8_t, uint8_t[], uint8_t);//(UBRR, RxBufferArray, RxBufferArraySize, TxBufferArray, TxBufferArraySize)//Inizializza il modulo UART con UBRR {UBRR}, buffer di ricezione {RxBufferArray} di {RxBufferArraySize} elementi e buffer di trasmissione {TxBufferArray} di {TxBufferArraySize} elementi

void UARTSetUBRR(uint16_t);//(UBRR)//Imposta l'ubrr a {UBRR}

void UARTDoubleSpeedEnabled();//Abilita la velocità doppia
void UARTDoubleSpeedDisabled();//Disabilità la velocità doppia

uint8_t UARTRxAvailable();//Restituisce il numero di elementi nel buffer di ricezione

uint8_t UARTRx();//Preleva dal buffer di ricezione e restituisce il byte meno recente
void UARTRxArray(uint8_t[], uint16_t);//(array, n)//Preleva {n} elementi dal buffer di ricezione e li posiziona in {array}

void UARTTx(uint8_t);//(value)//Trasmette il byte {value}
void UARTTxString(char*);//(char)//Trasmette una stringa a partire dal carattere {char}
void UARTTxArray(uint8_t[], uint16_t);//(array, n)//Trasmette {n} byte dell'array {array}