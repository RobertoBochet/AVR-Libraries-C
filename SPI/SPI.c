#include "SPI.h"
#include <util/delay_basic.h>

void SPIInit(SPIPrescaler_t prescaler)
{
	DDRB |= (1 << DDB5) | (1 << DDB3) | (1 << DDB2);//Imposto SCK, MOSI e SS come output
	DDRB &= ~(1 << DDB4);//Imposto MISO come input
	SPCR = (1 << SPE) | (1 << MSTR) | (prescaler);//Abilito SPI, imposto il microcontrollore come master e imposto il prescaler
}

void SPIReadArray(volatile uint8_t* ssPort, uint8_t ssPortN, void* array, uint16_t n)
{
	*ssPort &= ~(1 << ssPortN);//Seleziono lo slave
	
	while(n != 0)
	{
		SPDR = 0;//Inserisco un byte nullo nel registro a scorrimento
		while(!(SPSR & (1 << SPIF)));//Attendo che il trasferimento sia stato completato
		*(char*)array = SPDR;//Posiziono il valore ricevuto nell'array alla posizione corrente
		array++;//Sposto avanti di uno il puntatore nell'array
		n--;//Diminuisco di uno gli ottetti ancora da leggere
	}
	
	*ssPort |= 1 << ssPortN;//Deseleziono lo slave	
}
void SPIWriteArray(volatile uint8_t* ssPort, uint8_t ssPortN,  const void* array, uint16_t n)
{
	*ssPort &= ~(1 << ssPortN);//Seleziono lo slave
	
	while(n != 0)//Avvio un ciclo che termina quando tutti gli ottetti sono stati inviati
	{
		SPDR = *(char*)array;//Posiziono l'ottetto corrente nel registro dati del SPI
		array++;//Sposto il cursore all'ottetto corrente avanti di un byte
		n--;//Diminuisco di uno gli ottetti ancora da inviare
		while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
	}
	
	*ssPort |= 1 << ssPortN;//Deseleziono lo slave
}

char SPIReadRegister(volatile uint8_t* ssPort, uint8_t ssPortN, char address)
{
	*ssPort &= ~(1 << ssPortN);//Seleziono lo slave
	
	SPDR = 1 << 7 | address;//Posiziono l'ottetto da inviare nel registro dati del SPI
	while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
	
	SPDR = 0;//Posiziono un ottetto nullo nel registro dati del SPI
	while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato ricevuto
	
	*ssPort |= 1 << ssPortN;//Deseleziono lo slave
	
	return SPDR;//Restituisco l'ottetto ricevuto
}
void SPIReadRegisters(volatile uint8_t* ssPort, uint8_t ssPortN, const char* address, char* array, uint16_t n)
{
	*ssPort &= ~(1 << ssPortN);//Seleziono lo slave
	
	SPDR = (1 < 7) | *address;//Posiziono l'indirizzo al registro corrente nel registro a scorrimento
	while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
		
	while(n != 0)
	{
		address++;//Incremento di uno il puntatore all'indirizzo corrente
		SPDR = (1 < 7) | *address;//Posiziono l'indirizzo al registro corrente nel registro a scorrimento
		while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
		*array = SPDR;//Posiziono l'ottetto ricevuto nel buffer
		array++;//Sposto avanti di uno il puntatore nel buffer
		n--;//Diminuisco di uno gli ottetti ancora da leggere
	}
	
	*ssPort |= 1 << ssPortN;//Deseleziono lo slave
}
void SPIReadRegistersSequential(volatile uint8_t* ssPort, uint8_t ssPortN, char address, char* array, uint16_t n, char (*next)(char))
{
	*ssPort &= ~(1 << ssPortN);//Seleziono lo slave
	
	address = 1 << 7 | address;//Preparo l'indirizzo del registro da cui voglio iniziare a leggere
	
	SPDR = address;//Posiziono l'indirizzo del registro corrente nel registro a scorrimento
	while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
	
	while(n != 0)
	{
		if(next == 0) address++;//Se la funzione di incremento non è stato dichiarato seguo un incremento lineare
		else address = (*next)(address);
		
		SPDR = address;//Posiziono l'indirizzo del registro corrente nel registro a scorrimento
		while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
		*array = SPDR;//Posiziono l'ottetto ricevuto nel buffer
		array++;//Sposto avanti di uno il puntatore nel buffer
		n--;//Diminuisco di uno gli ottetti ancora da leggere
	}
	
	*ssPort |= 1 << ssPortN;//Deseleziono lo slave
}
void SPIReadRegisterRepeated(volatile uint8_t* ssPort, uint8_t ssPortN, char address, char* array, uint16_t n)
{
	*ssPort &= ~(1 << ssPortN);//Seleziono lo slave
	
	address = 1 << 7 | address;//Preparo l'indirzzo al registro da leggere
	
	SPDR = address;//Posiziono l'indirizzo al registro da leggere e lo posiziono nel registro a scorrimento
	while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
	
	while(n != 0)
	{
		SPDR = address;//Posiziono l'indirizzo al registro da leggere e lo posiziono nel registro a scorrimento
		while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
		*array = SPDR;//Posiziono l'ottetto ricevuto nel buffer
		array++;//Sposto avanti di uno il puntatore nel buffer
		n--;//Diminuisco di uno gli ottetti ancora da leggere
	}
	
	*ssPort |= 1 << ssPortN;//Deseleziono lo slave
}

void SPIWriteRegister(volatile uint8_t* ssPort, uint8_t ssPortN, char address, char value)
{
	*ssPort &= ~(1 << ssPortN);//Seleziono lo slave
	
	SPDR = address;//Posiziono l'indirizzo da inviare nel registro dati del SPI
	while(!(SPSR & (1 << SPIF)));//Attendo che l'indirizzo sia stato inviato
	
	SPDR = value;//Posiziono l'ottetto da inviare nel registro dati del SPI
	while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
	
	*ssPort |= 1 << ssPortN;//Deseleziono lo slave
}
void SPIWriteRegisters(volatile uint8_t* ssPort, uint8_t ssPortN, const char* address, const char* value, uint16_t n)
{
	while(n != 0)
	{
		*ssPort &= ~(1 << ssPortN);//Seleziono lo slave
		
		SPDR = *(char*)address;//Preparo l'indirizzo del registro da scrivere
		while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
		
		SPDR = *(char*)value;//Preparo il valore da scrivere nel registro
		while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
		
		*ssPort |= 1 << ssPortN;//Deseleziono lo slave
		
		address++;
		value++;
		n--;
	}
}

void SPISetBits(volatile uint8_t* ssPort, uint8_t ssPortN, char address, char mask)
{
	char value = 0;
	*ssPort &= ~(1 << ssPortN);//Seleziono lo slave
	
	SPDR = 1 << 7 | address;//Posiziono l'indirizzo del registro corrente nel registro a scorrimento
	while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
	SPDR = 0;//Posiziono un byte nullo nel registro a scorrimento
	while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
	value = SPDR;//Memorizzo il valore ricevuto
	
	*ssPort |= 1 << ssPortN;//Deseleziono lo slave
	
	mask |= value;//Calcolo il nuovo valore per il registro
	if(mask == value) return;//Se il nuovo valore corrisponde a quello vecchio termino la funzione
	
	_delay_loop_1(10);
	*ssPort &= ~(1 << ssPortN);//Seleziono lo slave
	
	SPDR = address;//Posiziono l'indirizzo del registro corrente nel registro a scorrimento
	while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
	SPDR = mask;//Posiziono il nuovo valore nel registro a scorrimento
	while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
	
	*ssPort |= 1 << ssPortN;//Deseleziono lo slave
}
void SPIClearBits(volatile uint8_t* ssPort, uint8_t ssPortN, char address, char mask)
{
	char value = 0;
	*ssPort &= ~(1 << ssPortN);//Seleziono lo slave
	
	SPDR = 1 << 7 | address;//Posiziono l'indirizzo del registro corrente nel registro a scorrimento
	while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
	SPDR = 0;//Posiziono un byte nullo nel registro a scorrimento
	while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
	value = SPDR;//Memorizzo il valore ricevuto
	
	*ssPort |= 1 << ssPortN;//Deseleziono lo slave
	
	mask = value & ~(mask);//Calcolo il nuovo valore per il registro
	if(mask == value) return;//Se il nuovo valore corrisponde a quello vecchio termino la funzione
	
	_delay_loop_1(10);
	*ssPort &= ~(1 << ssPortN);//Seleziono lo slave
	
	SPDR = address;//Posiziono l'indirizzo del registro corrente nel registro a scorrimento
	while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
	SPDR = mask;//Posiziono il nuovo valore nel registro a scorrimento
	while(!(SPSR & (1 << SPIF)));//Attendo che l'ottetto sia stato inviato
	
	*ssPort |= 1 << ssPortN;//Deseleziono lo slave
}