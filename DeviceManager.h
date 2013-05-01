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


void parseBytes(uint8_t* data, const char* s, uint8_t len);
void printBytes(uint8_t* data, uint8_t len, char* buf);



/**
 * A user has freedom to connect various devices to the arduino, either via extending the oneWire bus, or by assigning to specific pins, e.g. actuators, switch sensors.
 * Rather than make this compile-time, the configuration is stored at runtime. 
 * Also, the availability of various sensors will change. E.g. it's possible to have a fridge constant mode without a beer sensor.
 *
 * Since the data has to be persisted to EEPROM, references to the actual uses of the devices have to be encoded.  This is the function of the deviceID.
 */

class AnyDeviceConfig;


/*
 * Describes the logical function of each device. 
 */
enum DeviceFunction { 
	DEVICE_NONE = 0,														// used as a sentry to mark end of list
	// chamber devices
	DEVICE_CHAMBER_DOOR,													// switch sensor
	DEVICE_CHAMBER_HEAT, DEVICE_CHAMBER_COOL, DEVICE_CHAMBER_LIGHT,	// actuator	
	DEVICE_CHAMBER_TEMP, DEVICE_CHAMBER_ROOM_TEMP,						// temp sensors
	DEVICE_CHAMBER_FAN,						// a fan in the chamber
	DEVICE_CHAMBER_RESERVED1,					// reserved for future use	
	// carboy devices
	DEVICE_BEER_FIRST,
	DEVICE_BEER_TEMP=DEVICE_BEER_FIRST,									// primary beer temp sensor
	DEVICE_BEER_TEMP2,								// secondary beer temp sensor 
	DEVICE_BEER_HEAT, DEVICE_BEER_COOL,				// individual actuators
	DEVICE_BEER_SG,									// SG sensor
	DEVICE_BEER_RESERVED1, DEVICE_BEER_RESERVED2,	// reserved	
	DEVICE_BEER_RESERVED3,							// 8 in total	
	DEVICE_MAX
};

/**
 * Describes where the device is most closely associated.
 */
enum DeviceOwner {
	DEVICE_OWNER_NONE,	
	DEVICE_OWNER_CHAMBER,
	DEVICE_OWNER_BEER
};

enum DeviceType {
	DEVICETYPE_NONE,
	DEVICETYPE_TEMP_SENSOR,		/* BasicTempSensor - OneWire */
	DEVICETYPE_SWITCH_SENSOR,		/* SwitchSensor - direct pin */
	DEVICETYPE_SWITCH_ACTUATOR	/* Actuator - both direct pin and onewire are supported */	
};

/**
 * Determines the class of device for the given DeviceID.
 */
inline DeviceType deviceType(DeviceFunction id) {
	switch (id) {
	case DEVICE_CHAMBER_DOOR:
		return DEVICETYPE_SWITCH_SENSOR;
		
	case DEVICE_CHAMBER_HEAT:
	case DEVICE_CHAMBER_COOL:
	case DEVICE_CHAMBER_LIGHT:
	case DEVICE_CHAMBER_FAN:
	case DEVICE_BEER_HEAT:
	case DEVICE_BEER_COOL:
		return DEVICETYPE_SWITCH_ACTUATOR;
		
	case DEVICE_CHAMBER_TEMP:
	case DEVICE_CHAMBER_ROOM_TEMP:
	case DEVICE_BEER_TEMP:
	case DEVICE_BEER_TEMP2:
		return DEVICETYPE_TEMP_SENSOR;
		
	default: 
		return DEVICETYPE_NONE;
	}
}	

/**
 * Determines where this devices belongs.
 */
inline DeviceOwner deviceOwner(DeviceFunction id) {
	return id==0 ? DEVICE_OWNER_NONE : id>=DEVICE_BEER_FIRST ? DEVICE_OWNER_BEER : DEVICE_OWNER_CHAMBER;
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

/*
 * The concrete type of the device. 
 */
enum DeviceHardware { 
	DEVICE_HARDWARE_NONE=0,
	DEVICE_HARDWARE_PIN,			// a digital pin, either input or output
	DEVICE_HARDWARE_ONEWIRE_TEMP,	// a onewire temperature sensor
	DEVICE_HARDWARE_ONEWIRE_2413	// a onewire 2-channel PIO input or output.
};
		
/*
 * A union of all device types.
 */	
struct AnyDeviceConfig {
	
	uint8_t chamber;			// 0 means no chamber. 1 is the first chamber.	
	uint8_t beer;				// 0 means no beer, 1 is the first beer
	
	DeviceFunction deviceFunction;			// The function of the device to configure
											// this used to be a union, but now the fields are simply laid out bare
	DeviceHardware deviceHardware;			// flag to indicate the runtime type of device
	struct Hardware {																						
		uint8_t pinNr;							// the arduino pin nr this device is connected to
		bool invert;							// for actuators/sensors, controls if the signal value is inverted.
		DeviceAddress address;					// for onewire devices, if address[0]==0 then use the first matching device type, otherwise use the device with the specific address
		uint8_t pio;							// pio address
	} hw;		
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
	
	// a buffer of at least 17 characters in size
	void toString(char* buf) {		
		switch (type) {
			case DA_PIN:
				itoa(pinNr, buf, 10);
				break;
			case DA_ADDRESS:
				printBytes(address, 8, buf);
				break;
			case DA_PIO:
				itoa(pio, buf, 10);
				break;
			case DA_BOOLVALUE:
				strcpy(buf, boolValue ? "1" : "0");
				break;
			case DA_INVERT:
				strcpy(buf, invert ? "1" : "0");
				break;			
		}
	}
	
	void parse(AlternativeType type, const char* s) {
		this->type = type;
		switch (type) {
			case DA_PIN:
				pinNr = atoi(s);
				break;
			case DA_ADDRESS:
				parseBytes(address, s, sizeof(address));
				break;
			case DA_PIO:
				pio = atoi(s);
				break;
			case DA_BOOLVALUE:
				boolValue = !strcmp("on", s);
				break;			
			case DA_INVERT:
				invert = !strcmp("invert", s);
				break;
		}
	}				
};


class DeviceManager
{
public:


	static void loadDefaultDevices();
	
	/*
	 * Determines if the given device config is complete. 
	 */
	static bool firstUndefinedAlternative(AnyDeviceConfig& config, DeviceAlternatives& alternatives);
	
	
	/**
	 * Creates and Installs a device from the given device config.
	 * /return true if a device was installed. false if the config is not complete.
	 */
	static void installDevice(AnyDeviceConfig& config);
	
	static void parseDeviceDefinition(Stream& p, char end, AnyDeviceConfig& target);
	static void printDevice(AnyDeviceConfig& config, Print& p);
		
	/**
	 * Iterate over the defined devices.
	 * Caller first calls with deviceIndex 0. If the return value is true, config is filled out with the 
	 * config for the device. The caller can then increment deviceIndex and try again.
	 */
	static bool allDevices(AnyDeviceConfig& config, uint8_t deviceIndex);
	
private:
	static OneWire* oneWireBus(uint8_t pin);
	static void* createDevice(AnyDeviceConfig& config, DeviceType dc);
	
	static OneWire* beerSensorBus;
	static OneWire* fridgeSensorBus;
};

extern DeviceManager deviceManager;



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


