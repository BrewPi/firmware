/*
 * EepromManager.h
 *
 * Created: 22/04/2013 11:49:54
 *  Author: mat
 */ 

#pragma once

#include <avr/eeprom.h>
#include "DeviceManager.h"

/**
 * The eeprom persistence format is like this:
 *
 * 0x00		version number		This is incremented for each non-backwards-compatible change in the eeprom format. 
 * ...		chamberblock0		variable size - first byte is actual size for quick traversal
 * ...		chamberblock1
 * ...		chamberblockN
 * ...		0 byte (chamber block len)
 *
 * Each chamberblock is made up of
 * 0x00 chamber block length		1 (includes this 1 byte in size.) The block list is terminated by a 0 byte.
 * 0x01 ChamberSettings block		fixed size
 * ...	ChamberDevices block		Devices (null terminated.)
 * ...  Beer block
 *
 * ChamberSettings 
 *		ControlConstants			applies to all beers in this chamber
 * 
 * Devices:
 * 0x00 DeviceID
 * 0x01 DeviceType
 * ...  DeviceConfig				size dependent upon DeviceType
 * [repeat]
 * ...  terminated by 0 byte
 
 * BeerBlock: 
 *		ControlSettings				fixed size
 *		BeerDevices					devices
 *	
 
 
 * If the eeprom data is not initialized or is not the same version as expected, all chambers go until the valid data is provided. This is done by making the default mode offline. 
 * The external script will either reset the eeprom settings or manage the upgrade between versions. 
 * Note that this is typically only necessary after flashing new firmware, which is usually watched by an operator.
 * If the arduino restarts after a power failure, the settings will have been upgraded
 * and operation can continue from the saved settings.
 */

#define EEPROM_CONTROL_BLOCK_SIZE sizeof(uint8_t)+(sizeof(ControlSettings)+sizeof(ControlConstants))
#define EEPROM_DEVICE_CONFIG_SIZE sizeof(AnyDeviceConfig)

/*
 * Increment this value each time a change is made that is not backwardly-compatible.
 * Either the eeprom will be reset to defaults, or external code will re-establish the values via the piLink interface. 
 */
#define EEPROM_FORMAT_VERSION 2

typedef uint8_t* eptr_t;

class EepromAccess
{
public:
	uint8_t readByte(eptr_t offset) {
		return eeprom_read_byte(offset);
	}
	void writeByte(eptr_t offset, uint8_t value) {
		eeprom_write_byte(offset, value);
	}
	
	void readBlock(void* target, eptr_t offset, uint16_t size) {
		eeprom_read_block(target, offset, size);
	}
	void writeBlock(eptr_t target, void* source, uint16_t size) {
		eeprom_write_block(source, target, size);
	}
	
	void dumpBlock(Print& stream, eptr_t offset, eptr_t end) {
		while (offset<end) {
			uint8_t eValue = readByte(offset);
			stream.print(eValue, HEX);
		}
	}
	
	// provide functions for externally reading/writing to the eeprom to allow external scripts to upgrade.
	
	/*
		if ((EEPROM_CONTROL_BLOCK_SIZE*CHAMBER_COUNT)>1024) {
			DEBUG_MSG(PSTR("EEPROM space exhausted - required %d bytes"),EEPROM_CONTROL_BLOCK_SIZE*CHAMBER_COUNT+1);
		}
	*/
};

extern EepromAccess eepromAccess;

class EepromManager {
public:		
	
	/**
	 * Applies the settings from the eeprom
	 */
	void applySettings(eptr_t pv);
	
	/*
	 * Fetches the next settings block
	 * /param pv The current settings block
	 * /return a pointer to the next settings block (the block size header) or NULL if there are no more blocks
	 */
	eptr_t nextSettings(eptr_t pv) 
	{		
		eptr_t result = NULL;
		if (pv)							// pointer is valid
		{
			auto blocklen = eepromAccess.readByte(pv);		// size of block
			pv += blocklen;
			blocklen = eepromAccess.readByte(pv);
			if (blocklen)
				result = pv;
		}
		return result;
	}

	eptr_t startSettings() 
	{
		uint8_t version = eepromAccess.readByte(0);
		eptr_t result = NULL;
		if (version==EEPROM_FORMAT_VERSION) {
			result = eptr_t(1);		// assume that if data is provided, then at least one block is defined.
		}
		return result;
	}
	
	void dumpEeprom(Print& stream) {
		eptr_t p = startSettings();		
		while (p)
		{
			uint8_t blockLen = eepromAccess.readByte(p);
			eepromAccess.dumpBlock(stream, p, p+blockLen);
			p = nextSettings(p);
		}
	}
	

	void init() 
	{
	}
void storeTempConstantsAndSettings();
void storeTempSettings();

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