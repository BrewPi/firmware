#pragma once

#include "DataStream.h"

void readPlatformEndianMaskedBytes(void* data, uint8_t size, DataIn& in, DataIn& mask);

void writePlatformEndianBytes(void* data, uint8_t size, DataOut& out);


inline uint8_t readMaskedByte(DataIn& in, DataIn& mask)
{
	return in.next() & mask.next();
}
