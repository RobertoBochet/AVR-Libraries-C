#include "EEPROM.h"

int main()
{
	uint8_t a = 54;
	uint8_t b[] = {0x1A, 0x2A, 0x3A};
	uint16_t c[] = {0x1A1B, 0x2A2B, 0x3A3B};
	uint32_t d[] = {0x1A1B1C1D, 0x2A2B2C2D, 0x3A3B3C3D};
	double e = 5.54534;
	
	uint8_t aa = 0;
	uint8_t bb[] = {0, 0, 0};
	uint16_t cc[] = {0, 0, 0};
	uint32_t dd[] = {0, 0, 0};
	double ee = 0;
	
	EEPROMWrite(0x000, a);
	EEPROMWriteN(0x010, b, 3);
	EEPROMWriteN(0x020, (uint8_t*) c, 3*sizeof(uint16_t));
	EEPROMWriteN(0x030, (uint8_t*) d, 3*sizeof(uint32_t));
	EEPROMWriteN(0x040, (uint8_t*) &e, sizeof(double));
	
	aa = EEPROMRead(0x000);
	EEPROMReadN(0x010, bb, 3);
	EEPROMReadN(0x020, (uint8_t*) cc, 3*sizeof(uint16_t));
	EEPROMReadN(0x030, (uint8_t*) dd, 3*sizeof(uint32_t));
	EEPROMReadN(0x040, (uint8_t*) &ee, sizeof(double));
	
	EEPROMWrite(0x050, aa);
	EEPROMWriteN(0x060, bb, 3);
	EEPROMWriteN(0x070, (uint8_t*) cc, 3*sizeof(uint16_t));
	EEPROMWriteN(0x080, (uint8_t*) dd, 3*sizeof(uint32_t));
	EEPROMWriteN(0x090, (uint8_t*) &ee, sizeof(double));
}