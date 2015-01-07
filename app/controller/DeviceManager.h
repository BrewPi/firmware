/*
 * Copyright 2013 Matthew McGowan 
 * Copyright 2013 BrewPi/Elco Jacobs.
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

#pragma once

#include "Brewpi.h"

#include "Actuator.h"
#include "Sensor.h"
#include "TempSensor.h"
#include "OneWireDevices.h"
#include "Pins.h"

#ifdef ARDUINO
#include "DallasTemperature.h"	// for DeviceAddress
#endif

/**
 * A user has freedom to connect various devices to the arduino, either via extending the oneWire bus, or by assigning to specific pins, e.g. actuators, switch sensors.
 * Rather than make this compile-time, the configuration is stored at runtime. 
 * Also, the availability of various sensors will change. E.g. it's possible to have a fridge constant mode without a beer sensor.
 *
 * Since the data has to be persisted to EEPROM, references to the actual uses of the devices have to be encoded.  This is the function of the deviceID.
 */

class DeviceConfig;

typedef int8_t device_slot_t;
inline bool isDefinedSlot(device_slot_t s) { return s>=0; }
const device_slot_t MAX_DEVICE_SLOT = 16;		// exclusive
const device_slot_t INVALID_SLOT = -1;

/*
 * Describes the logical function of each device. 
 */
enum DeviceFunction { 
	DEVICE_NONE = 0,														// used as a sentry to mark end of list
	// chamber devices
	DEVICE_CHAMBER_DOOR=1,													// switch sensor
	DEVICE_CHAMBER_HEAT=2, DEVICE_CHAMBER_COOL=3, DEVICE_CHAMBER_LIGHT=4,	// actuator	
	DEVICE_CHAMBER_TEMP=5, DEVICE_CHAMBER_ROOM_TEMP=6,						// temp sensors
	DEVICE_CHAMBER_FAN=7,						// a fan in the chamber
	DEVICE_CHAMBER_RESERVED1=8,					// reserved for future use	
	// carboy devices
	DEVICE_BEER_FIRST=9,
	DEVICE_BEER_TEMP=DEVICE_BEER_FIRST,									// primary beer temp sensor
	DEVICE_BEER_TEMP2=10,								// secondary beer temp sensor 
	DEVICE_BEER_HEAT=11, DEVICE_BEER_COOL=12,				// individual actuators
	DEVICE_BEER_SG=13,									// SG sensor
	DEVICE_BEER_RESERVED1=14, DEVICE_BEER_RESERVED2=15,	// reserved	
	DEVICE_MAX=16
};

/**
 * Describes where the device is most closely associated.
 */
enum DeviceOwner {
	DEVICE_OWNER_NONE=0,	
	DEVICE_OWNER_CHAMBER=1,
	DEVICE_OWNER_BEER=2
};

enum DeviceType {
	DEVICETYPE_NONE=0,
	DEVICETYPE_TEMP_SENSOR=1,		/* BasicTempSensor - OneWire */
	DEVICETYPE_SWITCH_SENSOR=2,		/* SwitchSensor - direct pin and onewire are supported */
	DEVICETYPE_SWITCH_ACTUATOR=3	/* Actuator - both direct pin and onewire are supported */	
};

/*
 * The concrete type of the device. 
 */
enum DeviceHardware { 
	DEVICE_HARDWARE_NONE=0,
	DEVICE_HARDWARE_PIN=1,			// a digital pin, either input or output
	DEVICE_HARDWARE_ONEWIRE_TEMP=2,	// a onewire temperature sensor
#if BREWPI_DS2413
	DEVICE_HARDWARE_ONEWIRE_2413=3	// a onewire 2-channel PIO input or output.
#endif	
};

inline bool isAssignable(DeviceType type, DeviceHardware hardware)
{
	return (hardware==DEVICE_HARDWARE_PIN && (type==DEVICETYPE_SWITCH_ACTUATOR || type==DEVICETYPE_SWITCH_SENSOR))
#if BREWPI_DS2413
	|| (hardware==DEVICE_HARDWARE_ONEWIRE_2413 && (type==DEVICETYPE_SWITCH_ACTUATOR || (DS2413_SUPPORT_SENSE && type==DEVICETYPE_SWITCH_SENSOR)))
#endif	
	|| (hardware==DEVICE_HARDWARE_ONEWIRE_TEMP && type==DEVICETYPE_TEMP_SENSOR)
	|| (hardware==DEVICE_HARDWARE_NONE && type==DEVICETYPE_NONE);
}

inline bool isOneWire(DeviceHardware hardware) {	
	return 
#if BREWPI_DS2413
	hardware==DEVICE_HARDWARE_ONEWIRE_2413 || 
#endif	
	hardware==DEVICE_HARDWARE_ONEWIRE_TEMP;
}

inline bool isDigitalPin(DeviceHardware hardware) {
	return hardware==DEVICE_HARDWARE_PIN;
}

extern DeviceType deviceType(DeviceFunction id);


/**
 * Determines where this devices belongs.
 */
inline DeviceOwner deviceOwner(DeviceFunction id) {
	return id==0 ? DEVICE_OWNER_NONE : id>=DEVICE_BEER_FIRST ? DEVICE_OWNER_BEER : DEVICE_OWNER_CHAMBER;
}	

		
/*
 * A union of all device types.
 */	
struct DeviceConfig {
	
	uint8_t chamber;			// 0 means no chamber. 1 is the first chamber.	
	uint8_t beer;				// 0 means no beer, 1 is the first beer
	
	DeviceFunction deviceFunction;				// The function of the device to configure												
	DeviceHardware deviceHardware;				// flag to indicate the runtime type of device
	struct Hardware {
		uint8_t pinNr;							// the arduino pin nr this device is connected to
		bool invert;							// for actuators/sensors, controls if the signal value is inverted.
		bool deactivate;							// disable this device - the device will not be installed.
		DeviceAddress address;					// for onewire devices, if address[0]==0 then use the first matching device type, otherwise use the device with the specific address
		
		/* The pio and sensor calibration are never needed at the same time so they are a union. 
		 * To ensure the eeprom format is stable when including/excluding DS2413 support, ensure all fields are the same size.
		 */
		union {									
#if BREWPI_DS2413
			uint8_t pio;						// for ds2413 (deviceHardware==3) : the pio number (0,1)
#endif			
			int8_t /* fixed4_4 */ calibration;	// for temp sensors (deviceHardware==2), calibration adjustment to add to sensor readings
												// this is intentionally chosen to match the raw value precision returned by the ds18b20 sensors
		};
		bool reserved;								// extra space so that additional fields can be added without breaking layout
	} hw;
	bool reserved2;
};

/**
 * Provides a single alternative value for a given definition point in a device.
 */
struct DeviceAlternatives {
	enum AlternativeType { 		
		DA_PIN, DA_ADDRESS, DA_PIO, DA_INVERT, DA_BOOLVALUE 
	};
	AlternativeType type;
	union {
		uint8_t pinNr;					// type == DA_PIN
		uint8_t pio;					// type == DA_PIO
		DeviceAddress address;			// type == DA_ADDRESS
		bool invert;					// type == DA_INVERT
		bool boolValue;					// type == DA_BOOLVALUE
	};
	
};


typedef void (*EnumDevicesCallback)(DeviceConfig*, void* pv);
class EnumerateHardware;

struct DeviceOutput
{
	device_slot_t	slot;
	char value[10];
	Print* pp;
};

struct DeviceDisplay {
	int8_t id;		// -1 for all devices, >=0 for specific device	
	int8_t value;	// set value
	int8_t write;	// write value		
	int8_t empty;	// show unused devices when id==-1, default is 0
};

void HandleDeviceDisplay(const char* key, const char* value, void* pv);

/**
 * Reads or writes a value to a device.
 */
void UpdateDeviceState(DeviceDisplay& dd, DeviceConfig& dc, char* val);

class OneWire;

class DeviceManager
{
public:
	
	bool isDefaultTempSensor(BasicTempSensor* sensor);
	
	int8_t enumerateActuatorPins(uint8_t offset)
	{
#if BREWPI_ACTUATOR_PINS && defined(ARDUINO)
#if BREWPI_STATIC_CONFIG<=BREWPI_SHIELD_REV_A
		switch (offset) {
			case 0: return heatingPin;
			case 1: return coolingPin;
			default:
				return -1;
		}
#elif BREWPI_STATIC_CONFIG>=BREWPI_SHIELD_REV_C
		switch (offset) {
			case 0: return actuatorPin1;
			case 1: return actuatorPin2;
			case 2: return actuatorPin3;
			case 3: return actuatorPin4;
			default: return -1;
		}
#endif			
#endif
		return -1;
	}

	int8_t enumerateSensorPins(uint8_t offset) {
#if BREWPI_SENSOR_PINS && defined(ARDUINO)
		if (offset==0)
			return doorPin;
#endif			
		return -1;
	}
	
	/* Enumerates the 1-wire pins.
	 * 
	 */
	int8_t enumOneWirePins(uint8_t offset)
	{		
#ifdef ARDUINO            
#if BREWPI_STATIC_CONFIG<=BREWPI_SHIELD_REV_A
		if (offset==0)
			return beerSensorPin;
		if (offset==1)
			return fridgeSensorPin;
#endif
#if BREWPI_STATIC_CONFIG>=BREWPI_SHIELD_REV_C
		if (offset==0)
			return oneWirePin;
#endif
#endif
		return -1;								
	}

	static void setupUnconfiguredDevices();
	
	/*
	 * Determines if the given device config is complete. 
	 */
	static bool firstUndefinedAlternative(DeviceConfig& config, DeviceAlternatives& alternatives);
	
	
	/**
	 * Creates and Installs a device from the given device config.
	 * /return true if a device was installed. false if the config is not complete.
	 */
	static void installDevice(DeviceConfig& config);
	
	static void uninstallDevice(DeviceConfig& config);
	
	static void parseDeviceDefinition(Stream& p);
	static void printDevice(device_slot_t slot, DeviceConfig& config, const char* value, Print& p);
		
	/**
	 * Iterate over the defined devices.
	 * Caller first calls with deviceIndex 0. If the return value is true, config is filled out with the 
	 * config for the device. The caller can then increment deviceIndex and try again.
	 */
	static bool allDevices(DeviceConfig& config, uint8_t deviceIndex);

	static bool isDeviceValid(DeviceConfig& config, DeviceConfig& original, uint8_t deviceIndex);

	/**
	 * read hardware spec from stream and output matching devices
	 */
	static void enumerateHardware(Stream& p);
	
	static bool enumDevice(DeviceDisplay& dd, DeviceConfig& dc, uint8_t idx);

	static void listDevices(Stream& p);
	
private:
	
	static void enumerateOneWireDevices(EnumerateHardware& h, EnumDevicesCallback f, DeviceOutput& output);	
	static void enumeratePinDevices(EnumerateHardware& h, EnumDevicesCallback callback, DeviceOutput& output);
	static void OutputEnumeratedDevices(DeviceConfig* config, void* pv);
	static void handleEnumeratedDevice(DeviceConfig& config, EnumerateHardware& h, EnumDevicesCallback callback, DeviceOutput& out);
	static void readTempSensorValue(DeviceConfig::Hardware hw, char* out);
	

	static void* createDevice(DeviceConfig& config, DeviceType dc);
	static void* createOneWireGPIO(DeviceConfig& config, DeviceType dt);
	
	static void beginDeviceOutput() { firstDeviceOutput = true; }

	static OneWire* oneWireBus(uint8_t pin);

#ifdef ARDUINO
	
#if BREWPI_STATIC_CONFIG<=BREWPI_SHIELD_REV_A	
	static OneWire beerSensorBus;
	static OneWire fridgeSensorBus;	
#endif	
#if BREWPI_STATIC_CONFIG>=BREWPI_SHIELD_REV_C
	static OneWire primaryOneWireBus;	
#endif
        
#endif
	static bool firstDeviceOutput;
};


extern DeviceManager deviceManager;
