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

debug: Debug\libUART.a

release: Release\libUART.a


Debug\UART.o: UART.h UART.c
	-mkdir Debug
	avr-gcc.exe -c $(OPTIONS) $(DEBUG) -o "Debug\UART.o" "UART.c"

Debug\libUART.a: Debug\UART.o
	avr-ar.exe -r -o "Debug\libUART.a" "Debug\UART.o"

Release\UART.o: UART.h UART.c
	-mkdir Release
	avr-gcc.exe -c $(OPTIONS) $(RELEASE) -o "Release\UART.o" "UART.c"

Release\libUART.a: Release\UART.o
	avr-ar.exe -r -o "Release\libUART.a" "Release\UART.o"


clean:
	-rm -f Debug/*.o
	-rm -f Release/*.o