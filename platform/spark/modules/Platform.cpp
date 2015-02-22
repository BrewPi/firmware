#include "Brewpi.h"
#include "PiLinkHandlers.h"
#include "DeviceManager.h"
#include "Pins.h"
#include "ymodem/ymodem.h"

SYSTEM_MODE(SEMI_AUTOMATIC)

void handleReset() 
{ 
	System.reset();
}

void flashFirmware()
{
    System.serialFirmwareUpdate(&Serial);
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
