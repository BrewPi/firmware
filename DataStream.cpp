/*
 * DataStream.cpp
 *
 * Created: 30/03/2014 15:33:43
 *  Author: mat
 */ 

#include "Brewpi.h"
#include "DataStream.h"

	
bool BufferDataOut::write(uint8_t data) {
	if (pos<size) {
		buffer[pos++] = data;
		return true;
	}
	return false;
}

/**
 * Writes bytes in little endian order.
 * @param data	Pointer to the buffer, stored in the endian-ness for the platform.
 *
 */
void writePlatformEndianBytes(void* data, uint8_t size, DataOut& out)
{
	#if PLATFORM_BIG_ENDIAN
	uint8_t* buf = (uint8_t*)data;
	for (int i=size; i-->0; )
		out.write(buf[i]);
	#else	
	out.writeBuffer((uint8_t*)data, size);
	#endif
}


void readPlatformEndianMaskedBytes(void* _data, uint8_t size, DataIn& in, DataIn& mask)
{
	uint8_t* data = (uint8_t*)_data;
	#if PLATFORM_BIG_ENDIAN
	for (uint8_t i=size; i-->0; i++)
	#else
	for (uint8_t i=0; i<size; i++)
	#endif
	{
		uint8_t d = in.next();
		uint8_t m = mask.next();
		data[i] = (d&m) | (data[i] & ~m);	
	}
}