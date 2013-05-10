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

void EepromManager::zapEeprom()
{
	for (uint16_t offset=0; offset<EepromFormat::MAX_EEPROM_SIZE; offset++)
		eepromAccess.writeByte(offset, 0xFF);		
}


void EepromManager::initializeEeprom()
{
	// fetch the default values
	tempControl.loadDefaultConstants();
	tempControl.loadDefaultSettings();	
	
#if BREWPI_STATIC_CONFIG==BREWPI_SHIELD_REV_A	
	// default value is off - but for revA the eeprom is ready to go once initialized
	tempControl.setMode(MODE_BEER_CONSTANT);
#elif BREWPI_STATIC_CONFIG==BREWPI_SHIELD_REV_C
	// default value is off - but for revC user will install sensors and may need to test values etc.
	tempControl.setMode(MODE_TEST);
#endif
	
	// write the default constants 
	for (uint8_t c=0; c<EepromFormat::MAX_CHAMBERS; c++) {
		eptr_t pv = pointerOffset(chambers)+(c*sizeof(ChamberBlock)) ;
		tempControl.storeConstants(pv+offsetof(ChamberBlock, chamberSettings.cc));
		pv += offsetof(ChamberBlock, beer)+offsetof(BeerBlock, cs);
		for (uint8_t b=0; b<ChamberBlock::MAX_BEERS; b++) {
			tempControl.storeSettings(pv);	
			pv += sizeof(BeerBlock);		// advance to next beer
		}
	}

	// clear the rest of eeprom 	
	for (uint16_t offset=pointerOffset(devices); offset<EepromFormat::MAX_EEPROM_SIZE; offset++)
		eepromAccess.writeByte(offset, 0);	
	
	// set the version flag - so that storeDevice will work
	eepromAccess.writeByte(0, EEPROM_FORMAT_VERSION);
		
	saveDefaultDevices();		
}

uint8_t EepromManager::saveDefaultDevices() 
{
	DeviceConfig config;
	clear((uint8_t*)&config, sizeof(config));
		
#if BREWPI_STATIC_CONFIG==BREWPI_SHIELD_REV_A
	// single-chamber single beer config from original shield
	
	config.chamber = 1;			// all devices are in chamber 1
	config.hw.invert = 1;		// all pin devices inverted
	
	config.deviceHardware = DEVICE_HARDWARE_PIN;
	
	config.deviceFunction = DEVICE_CHAMBER_DOOR;
	config.hw.pinNr = doorPin;
	eepromManager.storeDevice(config, 0);

	config.deviceFunction = DEVICE_CHAMBER_HEAT;
	config.hw.pinNr = heatingPin;
	eepromManager.storeDevice(config, 1);

	config.deviceFunction = DEVICE_CHAMBER_COOL;
	config.hw.pinNr = heatingPin;
	eepromManager.storeDevice(config, 2);
	
	config.deviceHardware = DEVICE_HARDWARE_ONEWIRE_TEMP;
	config.hw.pinNr = fridgeSensorPin;
	config.deviceFunction = DEVICE_CHAMBER_TEMP;
	eepromManager.storeDevice(config, 3);

	config.beer = 1;
	config.hw.pinNr = beerSensorPin;
	config.deviceFunction = DEVICE_BEER_TEMP;
	eepromManager.storeDevice(config, 4);
		
	return 5;
	
#if BREWPI_STATIC_CONFIG==BREWPI_SHIELD_REV_C
	// the only component that's not dynamic is the door
	return 0;	
#endif	

#else
	return 0;
#endif
}

#define arraySize(x) (sizeof(x)/sizeof(x[0]))


bool EepromManager::applySettings()
{	
	if (!hasSettings())
		return false;

	// start from a clean state		
	deviceManager.setupUnconfiguredDevices();
		
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

void fill(int8_t* p, uint8_t size) {
	while (size-->0) *p++ = -1;
}
void clear(uint8_t* p, uint8_t size) {
	while (size-->0) *p++ = 0;
}
