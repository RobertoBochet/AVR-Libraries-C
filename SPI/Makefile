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

debug: Debug\libSPI.a

release: Release\libSPI.a


Debug\SPI.o: SPI.h SPI.c
	-mkdir Debug
	avr-gcc.exe -c $(OPTIONS) $(DEBUG) -o "Debug\SPI.o" "SPI.c"

Debug\libSPI.a: Debug\SPI.o
	avr-ar.exe -r -o "Debug\libSPI.a" "Debug\SPI.o"

Release\SPI.o: SPI.h SPI.c
	-mkdir Release
	avr-gcc.exe -c $(OPTIONS) $(RELEASE) -o "Release\SPI.o" "SPI.c"

Release\libSPI.a: Release\SPI.o
	avr-ar.exe -r -o "Release\libSPI.a" "Release\SPI.o"


clean:
	-rm -f Debug/*.o
	-rm -f Release/*.o