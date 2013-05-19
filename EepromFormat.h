/*
 * EepromFormat.h
 *
 * Created: 28/04/2013 22:05:32
 *  Author: mat
 */ 

#pragma once

#include "Brewpi.h"
#include "DeviceManager.h"
#include "TempControl.h"


struct ChamberSettings
{
	ControlConstants cc;
	byte reserved[4];
};

struct BeerBlock {
	ControlSettings cs;
	byte reserved[2];
};

struct ChamberBlock
{
	static const uint8_t MAX_BEERS = 6;	
	ChamberSettings chamberSettings;	
	BeerBlock		beer[MAX_BEERS];	
};

struct EepromFormat
{
	static const uint16_t MAX_EEPROM_SIZE = 1024;
	static const uint8_t MAX_CHAMBERS = 4;
	static const uint8_t MAX_DEVICES = MAX_DEVICE_SLOT;

	byte version;
	byte numChambers;
	byte reserved[4];	
	ChamberBlock chambers[MAX_CHAMBERS];
	DeviceConfig devices[MAX_DEVICES];
};


// check at compile time that the structure will fit into eeprom
void eepromSizeTooLarge()
__attribute__((error("EEPROM data is > 1024 bytes")));

static inline __attribute__((always_inline))
void eepromSizeCheck() {
	if (sizeof(EepromFormat) > EepromFormat::MAX_EEPROM_SIZE) {
		eepromSizeTooLarge();
	}
}


/**  
 * If the eeprom data is not initialized or is not the same version as expected, all chambers go until the valid data is provided. This is done by making the default mode offline. 
 * The external script will either reset the eeprom settings or manage the upgrade between versions. 
 * Note that this is typically only necessary after flashing new firmware, which is usually watched by an operator.
 * If the arduino restarts after a power failure, the settings will have been upgraded
 * and operation can continue from the saved settings.
 */

/*
 * Increment this value each time a change is made that is not backwardly-compatible.
 * Either the eeprom will be reset to defaults, or external code will re-establish the values via the piLink interface. 
 */
#define EEPROM_FORMAT_VERSION 4

/*
 * Version history:
 *
 * rev 1: static config (original avr code)
 * rev 2: initial version dynaconfig
 * rev 3: deactivate flag in DeviceConfig, and additinoal padding to allow for some future expansion.
 * rev 4: added padding at start and reduced device count to 16. We can always increase later.
 */
