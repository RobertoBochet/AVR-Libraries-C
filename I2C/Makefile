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

debug: Debug\libI2C.a

release: Release\libI2C.a


Debug\I2C.o: I2C.h I2C.c
	-mkdir Debug
	avr-gcc.exe -c $(OPTIONS) $(DEBUG) -o "Debug\I2C.o" "I2C.c"

Debug\libI2C.a: Debug\I2C.o
	avr-ar.exe -r -o "Debug\libI2C.a" "Debug\I2C.o"

Release\I2C.o: I2C.h I2C.c
	-mkdir Release
	avr-gcc.exe -c $(OPTIONS) $(RELEASE) -o "Release\I2C.o" "I2C.c"

Release\libI2C.a: Release\I2C.o
	avr-ar.exe -r -o "Release\libI2C.a" "Release\I2C.o"


clean:
	-rm -f Debug/*.o
	-rm -f Release/*.o