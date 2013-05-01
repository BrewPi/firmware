/*
 * EepromManager.h
 *
 * Created: 22/04/2013 11:49:54
 *  Author: mat
 */ 

#pragma once

#include <avr/eeprom.h>
#include "Arduino.h"

typedef uint16_t eptr_t;
#define INVALID_EPTR (0)

class EepromAccess
{
public:
	uint8_t readByte(eptr_t offset) {
		return eeprom_read_byte((uint8_t*)offset);
	}
	void writeByte(eptr_t offset, uint8_t value) {
		eeprom_write_byte((uint8_t*)offset, value);
	}
	
	void readBlock(void* target, eptr_t offset, uint16_t size) {
		eeprom_read_block(target, (uint8_t*)offset, size);
	}
	void writeBlock(eptr_t target, const void* source, uint16_t size) {
		eeprom_write_block(source, (void*)target, size);
	}
	
	void dumpBlock(Print& stream, eptr_t offset, eptr_t end) {
		while (offset<end) {
			uint8_t eValue = readByte(offset);
			stream.print(eValue, HEX);
		}
	}
};

extern EepromAccess eepromAccess;

class AnyDeviceConfig;


// todo - the Eeprom manager should avoid too frequent saves to the eeprom since it supports 100,000 writes. 
class EepromManager {
public:		
		
	EepromManager();
	
	void resetEeprom();
		
	void init() {}
	
	bool hasSettings();

	/**
	 * Applies the settings from the eeprom
	 */
	void applySettings();
		
	void dumpEeprom(Print& stream);

	/**
	 * Save the chamber constants and beer settings to eeprom for the currently active chamber.
	 */
	void storeTempConstantsAndSettings();

	/**
	 * Save just the beer temp settings.
	 */
	void storeTempSettings();


	bool fetchDevice(AnyDeviceConfig& config, uint8_t deviceIndex);
	bool storeDevice(const AnyDeviceConfig& config, uint8_t deviceIndex);
};

class EepromStream 
{
	eptr_t pv;
	
	void writeByte(uint8_t value) {
		eepromAccess.writeByte(pv++, value);				
	}
	void writeBlock(void* source, uint16_t size)
	{
		eepromAccess.writeBlock(pv, source, size);
		pv += size;
	}		
			
};

extern EepromManager eepromManager;