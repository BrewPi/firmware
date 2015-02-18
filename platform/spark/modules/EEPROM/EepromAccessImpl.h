#pragma once

// TODO - delegate to either EEPROM. emulation or flashee library

class SparkEepromAccess
{
public:
	static uint8_t readByte(eptr_t offset) {
		return 0;
	}
	static void writeByte(eptr_t offset, uint8_t value) {
		
	}
	
	static void readBlock(void* target, eptr_t offset, uint16_t size) {
	}
	static void writeBlock(eptr_t target, const void* source, uint16_t size) {
	}	
};

typedef SparkEepromAccess EepromAccess;
