ATMEGA_INCLUDE="C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.91\include"
MCU_INCLUDE="C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.91\gcc\dev\atmega328p"
MCU="atmega328p"

STANDARD_OPTIONS=-x c -std=gnu99 -I $(ATMEGA_INCLUDE) -mmcu=$(MCU) -B $(MCU_INCLUDE)
ADDITIONAL_OPTIONS=-ffunction-sections -fdata-sections -fpack-struct -fshort-enums
DEBUG=-DDEBUG -O1
RELEASE=-DNDEBUG -Os
OPTIONS=$(STANDARD_OPTIONS) $(ADDITIONAL_OPTIONS)

.PHONY: all debug release clean

all: debug release

debug: Debug\libEEPROM.a

release: Release\libEEPROM.a


Debug\EEPROM.o: EEPROM.h EEPROM.c
	-mkdir Debug
	avr-gcc.exe -c $(OPTIONS) $(DEBUG) -o "Debug\EEPROM.o" "EEPROM.c"

Debug\libEEPROM.a: Debug\EEPROM.o
	avr-ar.exe -r -o "Debug\libEEPROM.a" "Debug\EEPROM.o"

Release\EEPROM.o: EEPROM.h EEPROM.c
	-mkdir Release
	avr-gcc.exe -c $(OPTIONS) $(RELEASE) -o "Release\EEPROM.o" "EEPROM.c"

Release\libEEPROM.a: Release\EEPROM.o
	avr-ar.exe -r -o "Release\libEEPROM.a" "Release\EEPROM.o"


clean:
	-rm -f Debug/*.o
	-rm -f Release/*.o