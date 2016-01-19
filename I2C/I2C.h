#pragma once

#include <avr/io.h>

typedef enum {
	i2cErrorNone,//Non si sono verificati errori
	i2cErrorStartSendFailed,//Non si è riuscito a trasmettere il segnale di start
	i2cErrorRestartSendFailed,//Non si è riusciti a trasmettere il segnale di restart
	i2cErrorSlawACKNotReceived,//Lo slave contattato in modalità scrittura non ha risposto con un ACK, probabilmente non è presente sul bus
	i2cErrorSlarACKNotReceived,//Lo slave contattato in modalità lettura non ha risposto con un ACK
	i2cErrorDataACKNotReceived,//I dati sono stati inviati allo slave ma l'ACK atteso non è arrivato
	i2cErrorReceiveACKSendFailed,
	i2cErrorReceiveNACKSendFailed,
	i2cErrorTimeoutStart,//è scaduto il timeout nel tentativo di inviare il segnale di start, probabilmente il bus non presenta le giuste resistenze di pullup
	i2cErrorTimeoutRestart,//è scaduto il timeout nel tentativo di inviare il segnale di restart
	i2cErrorTimeoutContactSlave,//è scaduto il timeout nel tentativo di contattare uno slave
	i2cErrorTimeoutSendData,//è scaduto il timeout nel tentativo di inviare un byte
	i2cErrorTimeoutDataReceive//è scaduto il timeout nell'attesa di un byte da parte di uno slave
} I2CError_t;
typedef enum { i2cP1, i2cP4, i2cP16, i2cP64 } I2CPrescaler_t;//Valori di prescaler per la generazione del segnale di clock

extern I2CError_t I2CError;//Variabile di segnalazione errori

void I2CInit(uint8_t, I2CPrescaler_t);//(twbr, prescaler)//Iniziazlizza il bus con TWBR {twbr} e prescaler {prescaler}
	
uint8_t I2CRead(uint8_t, char, void*, uint8_t);//(slaveAddress, registryAddress, array, n)//Legge {n} byte dallo slave di indirizzo {slaveAddress} a partire dal registro {registryAddress} e li posiziona in {array}
uint8_t I2CWrite(uint8_t, char, const void*, uint8_t);//(slaveAddress, registryAddress, array, n)//Srive gli {n} byte conenuti in {array} a partire dal registro {registryAddress} dallo slave di indirizzo {slaveAddress}

uint8_t I2CCheckBus(char[16]);//(array)//Cerca sul bus i dispositivi in ascolto su tutti gli indirizzi per ogni indirizzo setta ad un 1 il bit corrispondente e in caso di errore restituisce 0