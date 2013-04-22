/*
 * EepromManager.h
 *
 * Created: 22/04/2013 11:49:54
 *  Author: mat
 */ 

#pragma once

#include "TempControl.h"
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
 * 0x00 chamber block length		1
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


class EepromAccess
{
public:
	uint8_t readByte(uint16_t offset);
	void writeByte(uint16_t offset, uint8_t value);
	
	void* readBlock(void* target, uint16_t offset, uint16_t size);
	void* writeBlock(uint16_t target, void* source, uint16_t size);
	
	// provide functions for externally reading/writing to the eeprom to allow external scripts to upgrade.
	
	/*
		if ((EEPROM_CONTROL_BLOCK_SIZE*CHAMBER_COUNT)>1024) {
			DEBUG_MSG(PSTR("EEPROM space exhausted - required %d bytes"),EEPROM_CONTROL_BLOCK_SIZE*CHAMBER_COUNT+1);
		}
	*/
};

class EepromManager {
public:		
	void applySettings( void* pv ) 
	{
	}

	void* nextSettings() 
	{
		return NULL;
	}

	void* chamberIterator() 
	{
		return NULL;
	}

	void init() 
	{
	}





};

extern EepromManager eepromManager;