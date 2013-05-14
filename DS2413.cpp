
#include "Brewpi.h"
#include "OneWire.h"

#include "DS2413.h"

uint8_t DS2413::channelReadAll()
{
	byte result = accessRead();
	return result<0 ? 0xFF : ((result&0x8)>>2 | (result&2)>>1);
}
	
void DS2413::channelWriteAll(uint8_t values)
{
	accessWrite(values);
}

