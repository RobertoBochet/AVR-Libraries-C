#include "TimeLife.h"
#include <avr/interrupt.h>

uint64_t overflowCount;//Contatore degli overflow
float overflowTime;//Millisecondi per overflow

void MillisSetup(uint32_t timerFrequency)
{
	uint8_t temp;
	uint16_t prescaler = 1;
	
	temp = TCCR0B & 0b111;//Isolo i soli bit legati al prescaler
	if(temp == 0b010) prescaler = 8;
	else if(temp == 0b011) prescaler = 64;
	else if(temp == 0b100) prescaler = 256;
	else if(temp == 0b101) prescaler = 1024;
	
	timerFrequency /= 1000;//Porto la frequenza di clock da Hz a kHz
	
	temp = ((TCCR0B & 0b1000) >> 1) | (TCCR0A & 0b11);//Isolo i soli bit legati alla modalità di funzionamento del timer
	if(temp == 0b000 || temp == 0b011) overflowTime = 256.0*prescaler/timerFrequency;//Normale o fast PWM
	else if(temp == 0b001) overflowTime = OCR0A*prescaler/timerFrequency;//PWM fase corretta
	else if(temp == 0b010) overflowTime = (1.0+OCR0A)*2.0*prescaler/timerFrequency;//CTC
	else if(temp == 0b101) overflowTime = 510.0*prescaler/timerFrequency;//PWM fase corretta OCRA
	else if(temp == 0b111) overflowTime = (1.0+OCR0A)*prescaler/timerFrequency;//Fast PWM OCRA
	
	TIMSK0 |= 1 << TOIE0;//Abilito l'interrupt di overflow del timer 0
	sei();//Abilito gli interrupt globali
	TCNT0 = overflowCount = 0;//Reset dei contatori
}
void MillisReset() { TCNT0 = overflowCount = 0; }

uint64_t Millis() { return (uint64_t) (overflowTime * overflowCount); }

ISR(TIMER0_OVF_vect) { overflowCount++; }