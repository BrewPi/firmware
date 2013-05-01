/*
 * EepromManager.cpp
 *
 * Created: 23/04/2013 03:15:44
 *  Author: mat
 */ 

#include <stddef.h>

#include "EepromManager.h"
#include "TempControl.h"
#include "EepromFormat.h"

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
	for (uint16_t i=1; i<sizeof(EepromFormat); i++)
		eepromAccess.writeByte(i, 0);	
	eepromAccess.writeByte(0, EEPROM_FORMAT_VERSION);
}

#define arraySize(x) (sizeof(x)/sizeof(x[0]))

// assumes the current chamber is already activated
void EepromManager::applySettings()
{	
	eptr_t pv = pointerOffset(chambers);
	tempControl.loadConstants(pv+offsetof(ChamberBlock, chamberSettings.cc));	
	tempControl.loadSettings(pv+offsetof(ChamberBlock, beer[0].cs));
	
	uint8_t device = 0;
	AnyDeviceConfig deviceConfig;
	while (fetchDevice(deviceConfig, device++))
	{		
		deviceManager.installDevice(deviceConfig);		
	}
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

void EepromManager::dumpEeprom(Print& stream) {
	eepromAccess.dumpBlock(stream, 0, sizeof(EepromFormat));
}

bool EepromManager::fetchDevice(AnyDeviceConfig& config, uint8_t deviceIndex)
{
	bool ok = (hasSettings() && deviceIndex<EepromFormat::MAX_DEVICES);
	if (ok)
		eepromAccess.readBlock(&config, pointerOffset(devices)+sizeof(AnyDeviceConfig)*deviceIndex, sizeof(AnyDeviceConfig));
	return ok;
}	

bool EepromManager::storeDevice(const AnyDeviceConfig& config, uint8_t deviceIndex)
{
	bool ok = (hasSettings() && deviceIndex<EepromFormat::MAX_DEVICES);
	if (ok)
		eepromAccess.writeBlock(pointerOffset(devices)+sizeof(AnyDeviceConfig)*deviceIndex, &config, sizeof(AnyDeviceConfig));	
	return ok;
}
