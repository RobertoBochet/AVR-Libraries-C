#pragma once

#include <avr/io.h>

uint8_t EEPROMRead(uint16_t);//(address)//Restituisce il valore all'indirizzo {address} della EEPROM 
void EEPROMReadN(uint16_t, uint8_t*, uint16_t);//(address, pointer, n)//Recupera e posiziona nei registri a partire da {pointer} gli {n} ottetti a partire dall'indirizzo {address} della EEPROM

void EEPROMWrite(uint16_t, uint8_t);//(address, value)//Scrive il valore {value} all'indirizzo {address} della EEPROM
void EEPROMWriteN(uint16_t, uint8_t*, uint16_t);//(address, pointer, n)//Scrive gli {n} ottetti a partire dal registro {pointer} nella EEPROM a partire dall'indirizzo {address}