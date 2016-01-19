#pragma once

#include <avr/io.h>

typedef enum { spiP4, spiP16, spiP64, spiP128 } SPIPrescaler_t;//Valori di prescaler per la generazione del onda SCK

static inline void SPISetPrescaler(SPIPrescaler_t prescaler) { SPCR = (SPCR & 0b11111100) | prescaler; }//(prescaler)//Modifica il presclaer del mopdulo SPI con {prescaler}

static inline void SPIDoubleSpeedEnabled() { SPSR |= 1 << SPI2X; }//Abilita la doppia velocità
static inline void SPIDoubleSpeedDisabled() { SPSR &= ~(1 << SPI2X); }//Disabilità la doppia velocità

static inline void SPICPOLEnabled() { SPCR |= 1 << CPOL; }
static inline void SPICPOLDisabled() { SPCR &= ~(1 << CPOL); }
static inline void SPICPHAEnabled() { SPCR |= 1 << CPHA; }
static inline void SPICPHADisabled() { SPCR &= ~(1 << CPHA); }
static inline void SPIFirstLSB() { SPCR |= 1 << DORD; }//Imposta l'SPI per inviare prima il bit meno significativo
static inline void SPIFirstMSB() { SPCR &= ~(1 << DORD); }//Imposta l'SPI per inviare prima il bit più significativo
	
void SPIInit(SPIPrescaler_t);//(prescaler)//Inizializza il modulo SPI come master con il valore di prescaler {prescaler}

void SPIReadArray(volatile uint8_t*, uint8_t, void*, uint16_t);//(port, portN, buffer, n)//Legge {n} ottetti dallo slave sulla porta {port} al pin {portN} e li posiziona in {buffer}
void SPIWriteArray(volatile uint8_t*, uint8_t, const void*, uint16_t);//(port, portN, buffer, n)//Invia gli {n} ottetti contenuti in {buffer} allo slave sulla porta {port} al pin {portN}

char SPIReadRegister(volatile uint8_t*, uint8_t, char);//(port, portN, registry)//Fa richiesta di lettura del registro {registry} allo slave sulla porta {port} al pin {portN} e lo restituisce
void SPIReadRegisters(volatile uint8_t*, uint8_t, const char*, char*, uint16_t);//(port, portN, registers, buffer, n)//Fa richiesta di lettura degli {n} registri {registers} allo slave sulla porta {port} al pin {portN} e li posiziona in {buffer}
void SPIReadRegistersSequential(volatile uint8_t*, uint8_t, char, char*, uint16_t, char (*)(char));//(port, portN, startRegistry, buffer, n, incrementFunction)//Fa richiesta di lettura degli {n} registri a partire da {startRegister} allo slave sulla porta {port} al pin {portN} e li posiziona in {buffer}. Se {incrementFunction} viene dichiarato viene usato per l'incremnto in caso contrario l'incremento sarà lineare
void SPIReadRegisterRepeated(volatile uint8_t*, uint8_t, char, char*, uint16_t);//(port, portN, registry, buffer, n)//Fa richiesta di lettura del registro {registry} {n} volte allo slave sulla porta {port} al pin {portN} e li posiziona in {buffer}

void SPIWriteRegister(volatile uint8_t*, uint8_t, char, char);//(port, portN, registry, value)//Fa richiesta di scittura dell'ottetto {value} nel registro {registry} dello slave sulla porta {port} al pin {portN}
void SPIWriteRegisters(volatile uint8_t*, uint8_t, const char*, const char*, uint16_t);//(port, portN, registers, values, n)//Fa richiesta di scrittura degli {n} ottetti {values} nei rispettivi registri {registers} dello slave sulla porta {port} al pin {portN}
	
void SPISetBits(volatile uint8_t*, uint8_t, char, char);//(port, portN, registry, mask)//Imposta i bit del registro {registry} secondo la maschera {mask} dello slave sulla porta {port} al pin {portN}
void SPIClearBits(volatile uint8_t*, uint8_t, char, char);//(port, portN, registry, mask)//Resetta i bit del registro {registry} secondo la maschera {mask} dello slave sulla porta {port} al pin {portN}