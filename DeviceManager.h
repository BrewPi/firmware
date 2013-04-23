/*
 * DeviceManager.h
 *
 * Created: 21/04/2013 23:53:36
 *  Author: mat
 */ 


#pragma once

#include "Actuator.h"
#include "Sensor.h"
#include "TempSensor.h"


/**
 * A user has freedom to connect various devices to the arduino, either via extending the oneWire bus, or by assigning to specific pins, e.g. actuators, switch sensors.
 * Rather than make this compile-time, the configuration is stored at runtime. 
 * Also, the availability of various sensors will change. E.g. it's possible to have a fridge constant mode without a beer sensor.
 *
 * Since the data has to be persisted to EEPROM, references to the actual uses of the devices have to be encoded.  This is the function of the deviceID.
 */

class DeviceManager
{
public:

	void loadDefaultDevices();
};

extern DeviceManager deviceManager;

#define MAX_BEERS 4

/*
 * Describes the logical function of each device. 
 */
enum class DeviceID : uint8_t { 
	none = 0,								// used as a sentry to mark end of list
	chamberDoor,							// binary sensor
	chamberHeat, chamberCool, chamberLight,	// binary actuator	
	chamberTemp, externalTemp,				// temp sensors
	chamberReserved1, chamberReserved2,		// reserved for future use	
	// for each carboy
	beerTemp,								// primary beer temp sensor
	beerTemp2,								// secondary beer temp sensor 
	beerHeat, beerCool,						// individual actuators
	beerSG,									// SG sensor
	beerReserved1, beerReserved2,			// reserved	
	beerReserved3							// 8 in total	
};

enum DeviceClass {
	NONE,
	TEMP_SENSOR,		/* BasicTempSensor */
	SWITCH_SENSOR,		/* SwitchSensor */
	SWITCH_ACTUATOR,	/* Actuator */
	SG_SENSOR			/* Should this be the same as a temp sensor? */	
};

inline DeviceClass deviceClass(DeviceID id) {
	switch (id) {
	case DeviceID::chamberDoor:
		return SWITCH_SENSOR;
		
	case DeviceID::chamberHeat:
	case DeviceID::chamberCool:
	case DeviceID::chamberLight:
	case DeviceID::beerHeat:
	case DeviceID::beerCool:
		return SWITCH_ACTUATOR;
		
	case DeviceID::chamberTemp:
	case DeviceID::externalTemp:
	case DeviceID::beerTemp:
		return TEMP_SENSOR;
		
	default: 
		return NONE;
	}
}	
		
enum ConnectType { 
	/** the device is controlled by a digital pin, e.g. digital sensors actuators.
	  * Could also include analog devices in future. 
	  */
	direct, 
	/** The device is controlled via an appropriate one-wire protocol over the bus on the assigned pin. 
	  */
	onewire 
};

/**
 * This combined with the DeviceID leads to a specific device type being used. 
 */

enum Polarity { normal, invert };

/*
 * 
 */
enum DeviceType { 
	
	DIGITAL_DEVICE=1, 
	
	ONEWIRE_TEMP, 
	
	ONEWIRE_SWITCH_2413 
};
	
	
/**
 * The DeviceConfig struct describes the details needed to instantiate and configure a device at runtime. 
 */
class AbstractDeviceConfig {
public:	
	uint8_t pinNo;				// the pin nr this device is connected to
	
};

class AbstractOneWireDeviceConfig : public AbstractDeviceConfig {
public:	
	DeviceAddress address;		// if address[0]==0 then use the first matching device type	
	
};

class DigitalPinDeviceConfig : public AbstractDeviceConfig {
public:
	static const DeviceType id = DIGITAL_DEVICE;
	bool invert;
};


// Concrete instances map to actual device types instantiated
class DS2413DeviceConfig : public AbstractOneWireDeviceConfig {
	
public:		
	static const DeviceType id = ONEWIRE_SWITCH_2413;
	byte pio;
	bool invert;
	
};

class DSTempSensorDeviceConfig : public AbstractOneWireDeviceConfig {
public:
	
	// fixed7_9 calibration;	// for example. 
	// precision? parasite?
	
	static const DeviceType id = ONEWIRE_TEMP;
};


class AbstractAnalogDeviceConfig : public AbstractDeviceConfig {
	
};

class LM37TempSensorDeviceConfig : public AbstractAnalogDeviceConfig {
public:	
	
};

/*
 * A union of all device types.
 * Note that in eeprom only the actual size of the selected union member is persisted, in contrast to the size of the union, which is the size of the largest member.
 */	
// should I really do this with virtual classes?
struct AnyDeviceConfig {
	
	DeviceID deviceID;			// The ID of the device to configure
	DeviceType deviceType;		// flag to indicate the runtime type of device	
	// todo - could combine the above two into a single encoded byte.
	union {
		DigitalPinDeviceConfig digitalPin;
		DSTempSensorDeviceConfig dsTempSensor;
		DS2413DeviceConfig ds2413;
	};
};


template<class T> class Converter {
	
};



/**
 * Factory class to create the different types of abstract devices. As new devices are added,
 * they are provided here.
 * The device config may take up
 */
class ChamberDeviceFactory {
public:
	BasicTempSensor* createTempSensor(DeviceID id);
	SwitchSensor* createSwitchSensor(DeviceID id);
	Actuator* createActuator(DeviceID id);
};

#if 0
/*
 * Manages the device configurations.
 */
class ChamberDevicesConfig
{
	// need to talk to EEPROM manager to save device config for each chamber.
	// 
	
public:
	int deviceCount();
	const AnyDevice const* devices();
	
	const AnyDevice* deviceWithId(DeviceID id) {
		// scan all devices and pull out the first matching one, or return NULL if no device config for the device.
	}

	const AnyDevice* defaultForId(DeviceID id) {
		switch (id) {
			case chamberHeat : 
		}
	}
	
	
	

	/*
	 * Configure a device using the given config.
	 */
	void* configure(AnyDevice& config, void* target);	
	
	// creates a basic temp sensor, or a actuator
	// dspose with delete.
	void* create(AnyDevice& config);
	
	// device id to string and back
	// connection type to string and back
	// pin no to string and back	
};
#endif


