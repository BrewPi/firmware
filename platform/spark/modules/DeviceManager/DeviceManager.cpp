#include "Brewpi.h"
#include "Platform.h"
#include "DeviceManager.h"


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
	if(shieldIsV1()){
		offset += 1; // V1 does not have actuatorPin0 (A7), skip it
	}
    switch (offset) {
        case 0: return PIN_ACTUATOR0;
        case 1: return PIN_ACTUATOR1;
        case 2: return PIN_ACTUATOR2;
        case 3: return PIN_ACTUATOR3;
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
