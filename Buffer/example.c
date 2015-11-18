#include "Buffer.h"

int main()
{
	uint8_t bufferArray[32];
	Buffer buffer;
	
	uint8_t a = 0;
	uint16_t b = 0;
	
	BufferInit(&buffer, bufferArray, 32);
	
	BufferPush(&buffer, 43);
	BufferPush(&buffer, 32);
	BufferPush(&buffer, 234);
	BufferPush(&buffer, 143);
	
	a = BufferPop(&buffer);//143
	a = BufferPull(&buffer);//43
	a = BufferPull(&buffer);//32
	
	b = BufferCount(&buffer);//1
	
	BufferFlush(&buffer);
	
	b = BufferCount(&buffer);//0
	
	BufferPush(&buffer, 211);
	BufferPush(&buffer, 64);
	BufferPush(&buffer, 145);
	BufferPush(&buffer, 18);
	
	b = BufferCount(&buffer);//3
	
	a = BufferPull(&buffer);//211
	a = BufferPop(&buffer);//18
	a = BufferPull(&buffer);//64
	
	b = BufferCount(&buffer);//1
}