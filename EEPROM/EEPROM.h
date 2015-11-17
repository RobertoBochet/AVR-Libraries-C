#pragma once

#include <avr/io.h>

uint8_t EEPROMRead(uint16_t);//(address)//Restituisce il valore all'indirizzo {address} della EEPROM 
void EEPROMReadArray(uint16_t, uint8_t[], uint16_t);//(address, pointer, n)//Recupera e inserisce in {array} {n} byte dalla EEPROM a partire dall'indirizzo {address}

void EEPROMWrite(uint16_t, uint8_t);//(address, value)//Scrive il valore {value} all'indirizzo {address} della EEPROM
void EEPROMWriteArray(uint16_t, uint8_t[], uint16_t);//(address, pointer, n)//Scrive {n} byte dall'array {array} dalla EEPROM a partire dall'indirizzo {address}