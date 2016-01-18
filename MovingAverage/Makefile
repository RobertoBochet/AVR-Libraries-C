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

debug: Debug\libMovingAverage.a

release: Release\libMovingAverage.a


Debug\SMA.o: MovingAverage.h SMA.c
	-mkdir Debug
	avr-gcc.exe -c $(OPTIONS) $(DEBUG) -o "Debug\SMA.o" "SMA.c"

Debug\WMA.o: MovingAverage.h WMA.c
	-mkdir Debug
	avr-gcc.exe -c $(OPTIONS) $(DEBUG) -o "Debug\WMA.o" "WMA.c"
	
Debug\libMovingAverage.a: Debug\SMA.o Debug\WMA.o
	avr-ar.exe -r -o "Debug\libMovingAverage.a" "Debug\SMA.o" "Debug\WMA.o"

Release\SMA.o: MovingAverage.h SMA.c
	-mkdir Release
	avr-gcc.exe -c $(OPTIONS) $(RELEASE) -o "Release\SMA.o" "SMA.c"

Release\WMA.o: MovingAverage.h WMA.c
	-mkdir Release
	avr-gcc.exe -c $(OPTIONS) $(RELEASE) -o "Release\WMA.o" "WMA.c"
	
Release\libMovingAverage.a: Release\SMA.o Release\WMA.o
	avr-ar.exe -r -o "Release\libMovingAverage.a" "Release\SMA.o" "Release\WMA.o"


clean:
	-rm -f Debug/*.o
	-rm -f Release/*.o