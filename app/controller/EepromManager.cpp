/*
 * Copyright 2013 BrewPi/Elco Jacobs.
 * Copyright 2013 Matthew McGowan 
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Brewpi.h"
#include <stddef.h>

#include "EepromManager.h"
#include "TempControl.h"
#include "EepromFormat.h"
#include "PiLink.h"

#define pointerOffset(x) offsetof(EepromFormat, x)

EepromManager::EepromManager()
{
    eepromSizeCheck();
}

void EepromManager::init() 
{    
}


bool EepromManager::hasSettings()
{
    uint8_t version = eepromAccess.readByte(pointerOffset(version));
    return (version==EEPROM_FORMAT_VERSION);
}

void EepromManager::zapEeprom()
{
	eepromAccess.clear();
}


void EepromManager::initializeEeprom()
{
    // clear all eeprom
    zapEeprom();

    deviceManager.setupUnconfiguredDevices(true); // remove devices and erase eeprom

    // fetch the default values
    tempControl.loadDefaultConstants();
    tempControl.loadDefaultSettings();	

    // write the default constants 
    for (uint8_t c=0; c<EepromFormat::MAX_CHAMBERS; c++) {
        eptr_t pv = pointerOffset(chambers)+(c*sizeof(ChamberBlock)) ;
        tempControl.storeConstants(pv+offsetof(ChamberBlock, chamberSettings.cc));
        pv += offsetof(ChamberBlock, beer)+offsetof(BeerBlock, cs);
        for (uint8_t b=0; b<ChamberBlock::MAX_BEERS; b++) {
//          logDeveloper("EepromManager - saving settings for beer %d at %d", b, (uint16_t)pv);
            tempControl.storeSettings(pv);	
            pv += sizeof(BeerBlock);		// advance to next beer
        }
    }

    // set the version flag - so that storeDevice will work
    eepromAccess.writeByte(pointerOffset(version), EEPROM_FORMAT_VERSION);

    saveDefaultDevices();
}

uint8_t EepromManager::saveDefaultDevices() 
{
	return 0;
}

bool EepromManager::applySettings()
{	
	if (!hasSettings())
		return false;

	// start from a clean state
	deviceManager.setupUnconfiguredDevices(false);
		
	logDebug("Applying settings");

	// load the one chamber and one beer for now
	eptr_t pv = pointerOffset(chambers);
	tempControl.loadConstants(pv+offsetof(ChamberBlock, chamberSettings.cc));	
	tempControl.loadSettings(pv+offsetof(ChamberBlock, beer[0].cs));
	
	logDebug("Applied settings");
	
	
	DeviceConfig deviceConfig;
	for (uint8_t index = 0; fetchDevice(deviceConfig, index); index++)
	{	
		if (deviceManager.isDeviceValid(deviceConfig, deviceConfig, index))
			deviceManager.createAndInstallDevice(deviceConfig, index);
		else {
			clear((uint8_t*)&deviceConfig, sizeof(deviceConfig));
			eepromManager.storeDevice(deviceConfig, index);
		}			
	}
	return true;
}

void EepromManager::storeTempConstantsAndSettings()
{
	uint8_t chamber = 0;
	eptr_t pv = pointerOffset(chambers);
	pv += sizeof(ChamberBlock)*chamber;
	tempControl.storeConstants(pv+offsetof(ChamberBlock, chamberSettings.cc));
		
	storeTempSettings();
}

void EepromManager::storeTempSettings()
{
	uint8_t chamber = 0;
	eptr_t pv = pointerOffset(chambers);
	pv += sizeof(ChamberBlock)*chamber;
	// for now assume just one beer. 
	tempControl.storeSettings(pv+offsetof(ChamberBlock, beer[0].cs));	
}

bool EepromManager::fetchDevice(DeviceConfig& config, uint8_t deviceIndex)
{
	bool ok = (hasSettings() && deviceIndex<EepromFormat::MAX_DEVICES);
	if (ok){
		eepromAccess.get(pointerOffset(devices)+sizeof(DeviceConfig)*deviceIndex, config);
	}
	return ok;
}	

bool EepromManager::storeDevice(const DeviceConfig& config, uint8_t deviceIndex)
{
	bool ok = (hasSettings() && deviceIndex<EepromFormat::MAX_DEVICES);
	if (ok){
		eepromAccess.put(pointerOffset(devices)+sizeof(DeviceConfig)*deviceIndex, config);
	}
	return ok;
}

void fill(int8_t* p, uint8_t size) {
	while (size-->0) *p++ = -1;
}
void clear(uint8_t* p, uint8_t size) {
	while (size-->0) *p++ = 0;
}

EepromManager eepromManager;
