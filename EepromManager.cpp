/*
 * EepromManager.cpp
 *
 * Created: 23/04/2013 03:15:44
 *  Author: mat
 */ 

#include "brewpi_avr.h"
#include <stddef.h>

#include "EepromManager.h"
#include "TempControl.h"
#include "EepromFormat.h"
#include "PiLink.h"

EepromManager eepromManager;
EepromAccess eepromAccess;

#define pointerOffset(x) offsetof(EepromFormat, x)

EepromManager::EepromManager()
{
	eepromSizeCheck();
}

bool EepromManager::hasSettings()
{
	uint8_t version = eepromAccess.readByte(pointerOffset(version));	
	return (version==EEPROM_FORMAT_VERSION);
}

void EepromManager::resetEeprom()
{
	tempControl.loadDefaultConstants();
	tempControl.loadDefaultSettings();
	
	for (uint8_t c=0; c<EepromFormat::MAX_CHAMBERS; c++) {
		eptr_t pv = pointerOffset(chambers)+(c*sizeof(ChamberBlock)) ;
		tempControl.storeConstants(pv+offsetof(ChamberBlock, chamberSettings.cc));
		pv += offsetof(ChamberBlock, beer)+offsetof(BeerBlock, cs);
		for (uint8_t b=0; b<ChamberBlock::MAX_BEERS; b++) {
			tempControl.storeSettings(pv);	
			pv += sizeof(BeerBlock);		// advance to next beer
		}
	}
		
	for (uint16_t i=pointerOffset(devices); i<EepromFormat::MAX_EEPROM_SIZE; i++)
		eepromAccess.writeByte(i, 0);	
		
	eepromAccess.writeByte(0, EEPROM_FORMAT_VERSION);
}

#define arraySize(x) (sizeof(x)/sizeof(x[0]))

// assumes the current chamber is already activated
bool EepromManager::applySettings()
{	
	if (!hasSettings())
		return false;
		
	DEBUG_MSG(PSTR("Applying settings"));

	// load the one chamber and one beer for now
	eptr_t pv = pointerOffset(chambers);
	tempControl.loadConstants(pv+offsetof(ChamberBlock, chamberSettings.cc));	
	tempControl.loadSettings(pv+offsetof(ChamberBlock, beer[0].cs));
	
	DEBUG_MSG(PSTR("Applied settings"));
	
	DeviceConfig deviceConfig;
	for (uint8_t index = 0; fetchDevice(deviceConfig, index); index++)
	{	
		if (deviceManager.isDeviceValid(deviceConfig, deviceConfig, index))		 
			deviceManager.installDevice(deviceConfig);
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
	if (ok)
		eepromAccess.readBlock(&config, pointerOffset(devices)+sizeof(DeviceConfig)*deviceIndex, sizeof(DeviceConfig));
	return ok;
}	

bool EepromManager::storeDevice(const DeviceConfig& config, uint8_t deviceIndex)
{
	bool ok = (hasSettings() && deviceIndex<EepromFormat::MAX_DEVICES);
	if (ok)
		eepromAccess.writeBlock(pointerOffset(devices)+sizeof(DeviceConfig)*deviceIndex, &config, sizeof(DeviceConfig));	
	return ok;
}

