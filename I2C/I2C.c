#define TIMEOUT_START 1000
#define TIMEOUT_CONTACT_SLAVE 1000
#define TIMEOUT_SEND 1000
#define TIMEOUT_RECEIVE 1000
#define DELAY_STOP 100

#include "I2C.h"
#include <util/delay_basic.h>

typedef enum { i2cOperationWrite, i2cOperationRead } I2COperation_t;//Operazioni per di trasmissione

I2CError_t I2CError;//Variabile di segnalazione errori

static inline uint8_t I2CWait(uint16_t timeout)
{
	while (!(TWCR & (1 << TWINT)) && timeout--);//Creo un ciclo di attesa che si ripeta fintanto che l'operazione sia conclusa o il timeout sia arrivato
	return TWCR & (1 << TWINT);//Restituisco il valore del flag interrupt per definire se l'operazione sia stata conclusa normalmente o per timeout
}
static inline void I2CStop()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);//Abilito il bus, resetto il bit l'interrupt e invio il segnale di stop
	_delay_loop_1(DELAY_STOP);
}
static inline uint8_t I2CStart()
{
	I2CError = i2cErrorNone;//Resetto la variabile di segnalazione errori
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);//Abilito il bus, resetto il bit l'interrupt e invio il segnale di start
	
	if(!I2CWait(TIMEOUT_START))//Attendo e verifico se l'uscita dall'attesa è causata da timeout
	{
		I2CStop();//Invio il segnale di stop sul bus
		I2CError = i2cErrorTimeoutStart;//Imposto l'errore per il timeout
		return 0;//Restituisco 0 per segnalare l'operazione non riuscita
	}
	
	if((TWSR & 0xF8) != 0x08)//Verifico che l'operazione sia andata a buon fine
	{
		I2CStop();//Invio il segnale di stop sul bus
		I2CError = i2cErrorStartSendFailed;//Modifico la variabile di segnalazione errori con il valore di errore per start non riuscito
		return 0;//Restituisco 0 per segnalare l'operazione non riuscita
	}
	
	return 1;//Operazione conclusa senza errori
}
static inline uint8_t I2CRestart()
{
	I2CError = i2cErrorNone;//Resetto la variabile di segnalazione errori
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);//Abilito il bus, resetto il bit l'interrupt e invio il segnale di restart
	
	if(!I2CWait(TIMEOUT_START))//Attendo e verifico se l'uscita dall'attesa è causata da timeout
	{
		I2CStop();//Invio il segnale di stop sul bus
		I2CError = i2cErrorTimeoutRestart;//Imposto l'errore per il timeout
		return 0;//Restituisco 0 per segnalare l'operazione non riuscita
	}
	
	if((TWSR & 0xF8) != 0x10)//Verifico che l'operazione sia andata a buon fine
	{
		I2CStop();//Invio il segnale di stop sul bus
		I2CError = i2cErrorRestartSendFailed;//Modifico la variabile di segnalazione errori con il valore di errore per restart non riuscito
		return 0;//Restituisco 0 per segnalare l'operazione non riuscita
	}
	
	return 1;//Operazione conclusa senza errori
}
static inline uint8_t I2CContactSlave(uint8_t slaveAddress, I2COperation_t operation)
{
	I2CError = i2cErrorNone;//Resetto la variabile di segnalazione errori
	TWDR = (slaveAddress << 1) | operation;//Sposto l'indirizzo del device un bit a sinistra e aggiungo il bit di modalità
	TWCR = (1 << TWINT) | (1 << TWEN);//Abilito il bus, e resetto il bit di interrupt
	
	if(!I2CWait(TIMEOUT_CONTACT_SLAVE))//Attendo e verifico se l'uscita dall'attesa è causata da timeout
	{
		I2CStop();//Invio il segnale di stop sul bus
		I2CError = i2cErrorTimeoutContactSlave;//Imposto l'errore per il timeout
		return 0;//Restituisco 0 per segnalare l'operazione non riuscita
	}
	
	if((TWSR & 0xF8) != (operation ? 0x40 : 0x18))//Verifico che l'operazione sia andata a buon fine
	{
		I2CStop();//Invio il segnale di stop sul bus
		I2CError = operation ? i2cErrorSlarACKNotReceived : i2cErrorSlawACKNotReceived;//Modifico la variabile di segnalazione errrori con il valore di errore di contatto device R/W
		return 0;//Restituisco 0 per segnalare l'operazione non riuscita
	}
	
	return 1;//Operazione conclusa senza errori
}
static inline uint8_t I2CSend(char data)
{
	I2CError = i2cErrorNone;//Resetto la varibaile di segnalazione errori
	TWDR = data;//Posiziono il dato da inviare nel registro dati del I2C
	TWCR = (1 << TWINT) | (1 << TWEN);//Resetto il bit di interrupt e avvio il bus
	
	if(!I2CWait(TIMEOUT_SEND))//Attendo e verifico se l'uscita dall'attesa è causata da timeout
	{
		I2CStop();//Invio il segnale di stop sul bus
		I2CError = i2cErrorTimeoutSendData;//Imposto l'errore per il timeout
		return 0;//Restituisco 0 per segnalare l'operazione non riuscita
	}
	
	if((TWSR & 0xF8) != 0x28)//Verifico che l'operazione sia andata a buon fine
	{
		I2CStop();//Invio il segnale di stop sul bus
		I2CError = i2cErrorDataACKNotReceived;//Modifico la variabile di segnalazione errrori con il valore di errore di dato in invio
		return 0;//Restituisco 0 per segnalare l'operazione non riuscita
	}
	
	return 1;//Operazione conclusa senza errori
}
static inline uint8_t I2CReceive(void* data, uint8_t isLast)
{
	I2CError = i2cErrorNone;//Resetto la varibaile di segnalazione errori
	TWCR = (1 << TWINT) | (1 << TWEN) | (isLast ? 0 : (1 << TWEA));//Resetto il bit di interrupt, abilito il bus e in caso invio l'ACK
	
	if(!I2CWait(TIMEOUT_RECEIVE))//Attendo e verifico se l'uscita dall'attesa è causata da timeout
	{
		I2CStop();//Invio il segnale di stop sul bus
		I2CError = i2cErrorTimeoutDataReceive;//Imposto l'errore per il timeout
		return 0;//Restituisco 0 per segnalare l'operazione non riuscita
	}
	
	if((TWSR & 0xF8) != (isLast ? 0x58 : 0x50))//Verifico che l'operazione sia andata a buon fine
	{
		I2CStop();//Invio il segnale di stop sul bus
		I2CError = isLast ? i2cErrorReceiveACKSendFailed : i2cErrorReceiveNACKSendFailed;//Modifico la variabile di segnalazione errrori con il valore di errore di dato in ricezione
		return 0;//Restituisco 0 per segnalare l'operazione non riuscita
	}
	*(char*)data = TWDR;
	
	return 1;//Operazione conclusa senza errori
}

void I2CInit(uint8_t twbr, I2CPrescaler_t prescaler)
{
	TWBR = twbr;//Imposto il valore per la generazione del segnale di clock del bus
	TWSR = prescaler;//Imposto il prescaler per la generazione del segnale di clock del bus
	
	I2CError = i2cErrorNone;//Resetto la variabile di segnalazione errori
}

uint8_t I2CRead(uint8_t slave, char reg, void* array, uint8_t n)
{
	if(!I2CStart()) return 0;//Invio il segnale di start
	if(!I2CContactSlave(slave, i2cOperationWrite)) return 0;//Invio l'indirizzo dello slave in modalità scrittura
	if(!I2CSend(reg)) return 0;//Invio l'indirizzo del registro da cui voglio iniziare a leggere
	if(!I2CRestart()) return 0;//Invio il segnale di restart
	if(!I2CContactSlave(slave, i2cOperationRead)) return 0;//Invio l'indirizzo dello slave in modalità lettura
	while(n != 0)//Ripeto il ciclo finchè i byte da leggere non sono finiti
	{
		if(!I2CReceive(array, (n == 1))) return 0;//Ricevo il byte corrente da leggere dallo slave e lo posiziono nel buffer
		array++;//Sposto il puntatore al byte corrente da leggere nel buffer avanti di uno
		n--;//Diminuisco di uno i byte ancora da leggere
	}
	I2CStop();//Invio il segnale di stop
	
	return 1;//Operazione conclusa senza errori
}
uint8_t I2CWrite(uint8_t slave, char reg, const void* array, uint8_t n)
{
	if(!I2CStart()) return 0;//Invio il segnale di start
	if(!I2CContactSlave(slave, i2cOperationWrite)) return 0;//Invio l'indirizzo dello slave in modalità scrittura
	if(!I2CSend(reg)) return 0;//Invio l'indirizzo del registro da cui voglio inziare la scrittura
	while(n != 0)//Ripeto il ciclo finchè i byte da scrivere non sono finiti
	{
		if(!I2CSend(*(char*)array)) return 0;//Invio il byte corrente da scrivere nel registro
		array++;//Sposto il puntatore ai byte da scrivere avanti di uno
		n--;//Diminuisco di uno i byte ancora da scrivere
	}
	I2CStop();//Invio il segnale di stop
	
	return 1;//Operazione conclusa senza errori
}

uint8_t I2CCheckBus(char array[16])
{
	for(uint8_t i = 0; i < 128; i++)//Eseguo il ciclo per i 128 indirizzi del bus
	{
		if(!I2CStart()) return 0;//In caso di errori sul bus restituisco 0
		array[i / 8] |= I2CContactSlave(i, i2cOperationWrite) << i % 8;//Cerca di contattare lo slave all'indirizzo corrente, se questo risponde imposta a 1 il bit corrispondente
		I2CStop();//Invio il segnale di stop
	}
	return 1;//Operazione conclusa senza errori
}