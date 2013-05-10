/*
 * EepromManager.h
 *
 * Created: 22/04/2013 11:49:54
 *  Author: mat
 */ 

#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

#include "brewpi_avr.h"
#include <avr/eeprom.h>

typedef uint16_t eptr_t;
#define INVALID_EPTR (0)


void fill(int8_t* p, uint8_t size);
void clear(uint8_t* p, uint8_t size);

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

extern EepromAccess eepromAccess;

class DeviceConfig;


// todo - the Eeprom manager should avoid too frequent saves to the eeprom since it supports 100,000 writes. 
class EepromManager {
public:		
		
	EepromManager();
	
	static void init() {}
	
	static void resetEeprom();
	
	/**
	 * Determines if this eeprom has settings.
	 */
	static bool hasSettings();

	/**
	 * Applies the settings from the eeprom
	 */
	static bool applySettings();
		
	static void dumpEeprom(Print& stream, uint16_t offset);

	/**
	 * Save the chamber constants and beer settings to eeprom for the currently active chamber.
	 */
	static void storeTempConstantsAndSettings();

	/**
	 * Save just the beer temp settings.
	 */
	static void storeTempSettings();

	static bool fetchDevice(DeviceConfig& config, uint8_t deviceIndex);
	static bool storeDevice(const DeviceConfig& config, uint8_t deviceIndex);
	
	static uint8_t saveDefaultDevices();
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

#endif