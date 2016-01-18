#pragma once

#include <avr/io.h>

static inline void UARTSetUBRR(uint16_t ubrr) { UBRR0H = ubrr >> 8; UBRR0L = ubrr; }

static inline void UARTDoubleSpeedEnabled() { UCSR0A |= 1 << U2X0; }
static inline void UARTDoubleSpeedDisabled() { UCSR0A &= ~(1 << U2X0); }

void UARTInit(uint16_t, void*, uint8_t, void*, uint8_t);//(UBRR, RxBufferArray, RxBufferArraySize, TxBufferArray, TxBufferArraySize)//Inizializza il modulo UART con UBRR {UBRR}, buffer di ricezione {RxBufferArray} di {RxBufferArraySize} elementi e buffer di trasmissione {TxBufferArray} di {TxBufferArraySize} elementi

uint8_t UARTRxAvailable();//Restituisce il numero di elementi nel buffer di ricezione

char UARTRx();//Preleva dal buffer di ricezione e restituisce il byte meno recente
void UARTRxArray(void*, uint16_t);//(array, n)//Preleva {n} elementi dal buffer di ricezione e li posiziona in {array}

void UARTTx(char);//(value)//Trasmette il byte {value}
void UARTTxString(const char*);//(char)//Trasmette una stringa a partire dal carattere {char}
void UARTTxArray(const void*, uint16_t);//(array, n)//Trasmette {n} byte dell'array {array}