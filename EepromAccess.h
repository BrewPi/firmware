/* 
 * File:   EepromAccess.h
 * Author: mat
 *
 * Created on 19 August 2013, 20:45
 */

#pragma once

#ifdef ARDUINO

#include <avr/eeprom.h>

class EepromAccess
{
public:
	static uint8_t readByte(eptr_t offset) {
		return eeprom_read_byte((uint8_t*)offset);
	}
	static void writeByte(eptr_t offset, uint8_t value) {
		eeprom_write_byte((uint8_t*)offset, value);
	}
	
	static void readBlock(void* target, eptr_t offset, uint16_t size) {
		eeprom_read_block(target, (uint8_t*)offset, size);
	}
	static void writeBlock(eptr_t target, const void* source, uint16_t size) {
		eeprom_update_block(source, (void*)target, size);
	}	
};

#else


class EepromAccess
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


#endif
