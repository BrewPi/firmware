#include "Brewpi.h"
#include "PiLinkHandlers.h"
#include "DeviceManager.h"
#include "Pins.h"
#if PLATFORM_ID==0
#include "Ymodem/Ymodem.h"
#endif
#include "flashee-eeprom.h"
#include "EepromManager.h"

void flashFirmware()
{
    System.firmwareUpdate(&Serial);
}


#if !BREWPI_SIMULATE
OneWire primaryOneWireBus(oneWirePin);
#endif

OneWire* DeviceManager::oneWireBus(uint8_t pin) {
#if !BREWPI_SIMULATE
    if (pin==oneWirePin)
            return &primaryOneWireBus;
#endif
    return NULL;
}


int8_t  DeviceManager::enumerateActuatorPins(uint8_t offset)
{
    switch (offset) {
        case 0: return actuatorPin1;
        case 1: return actuatorPin2;
        case 2: return actuatorPin3;
        default: return -1;
    }
}

int8_t DeviceManager::enumerateSensorPins(uint8_t offset) 
{
    return -1;
}
	
/* 
 * Enumerates the 1-wire pins.
 */
int8_t DeviceManager::enumOneWirePins(uint8_t offset)
{		
    if (offset==0)
        return oneWirePin;
    return -1;								
}


#define EEPROM_MAGIC1 (0xD0)
#define EEPROM_MAGIC2 (0x9E)

void eraseExternalFlash()
{
#if PLATFORM_ID==PLATFORM_SPARK_CORE
    Flashee::Devices::userFlash().eraseAll();    
#endif    
}

bool platform_init()
{            
    bool initialize = (EEPROM.read(0)!=EEPROM_MAGIC1 || EEPROM.read(1)!=EEPROM_MAGIC2);
    if (initialize) {
        
        eraseExternalFlash();
        
        EEPROM.write(0, EEPROM_MAGIC1);
        EEPROM.write(1, EEPROM_MAGIC2);
    }
    eepromAccess.init();
    return initialize;
}
