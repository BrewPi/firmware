/*
 * DeviceManager.cpp
 *
 * Created: 22/04/2013 22:02:14
 *  Author: mat
 */ 

#include "DeviceManager.h"
#include "TempControl.h"
#include "OneWireTempSensor.h"
#include "OneWireActuator.h"
#include "Actuator.h"
#include "Sensor.h"
#include "ExternalTempSensor.h"
#include "PiLink.h"

DeviceManager deviceManager;

// todo - allow bus config to be moved to eeprom?
OneWire* DeviceManager::beerSensorBus = new OneWire(beerSensorPin);
OneWire* DeviceManager::fridgeSensorBus = new OneWire(fridgeSensorPin);

/**
 * Defaults for sensors, actuators and temperature sensors when not defined in the eeprom.
 */
ValueSensor<bool> defaultSensor(false);			// off
ValueActuator defaultActuator;
ExternalTempSensor defaultTempSensor(false);	// disconnected

OneWire* DeviceManager::oneWireBus(uint8_t pin) {
	if (pin==beerSensorPin)
		return beerSensorBus;
	if (pin==fridgeSensorPin)
		return fridgeSensorBus;
	return NULL;
}

// set all devices to no-ops.
void DeviceManager::loadDefaultDevices()
{	
	// todo - this doesn't really belong here, but part of the initialization for a chamber.
	
	if (tempControl.beerSensor==NULL)
		tempControl.beerSensor = new TempSensor(&defaultTempSensor);
	if (tempControl.fridgeSensor==NULL)
		tempControl.fridgeSensor = new TempSensor(&defaultTempSensor);
		
	tempControl.cooler = tempControl.heater = tempControl.light = &defaultActuator;
	tempControl.door = &defaultSensor;
	
#if 0
	tempControl.beerSensor = new TempSensor(new OneWireTempSensor(oneWireBus(beerSensorPin)));
	tempControl.fridgeSensor = new TempSensor(new OneWireTempSensor(oneWireBus(fridgeSensorPin)));
		
	tempControl.cooler = new DigitalConstantPinActuator<coolingPin, SHIELD_INVERT>();
	tempControl.heater = new DigitalConstantPinActuator<heatingPin, SHIELD_INVERT>();
	tempControl.door = new DigitalConstantPinSensor<doorPin, SHIELD_INVERT, USE_INTERNAL_PULL_UP_RESISTORS>();
	tempControl.light = tempControl.heater;
#endif		
	//tempControl.loadSettingsAndConstants(); //read previous settings from EEPROM

	tempControl.init();
	tempControl.updatePID();
	tempControl.updateState();
		
#if 0	// code from multichamber
	#if BREWPI_EMULATE	// use in-memory/emulated devices
	MockTempSensor directFridgeSensor(10,10);
	MockTempSensor directBeerSensor(5,5);
	#elif BREWPI_SIMULATE
	ExternalTempSensor directFridgeSensor(true);
	ExternalTempSensor directBeerSensor(true);
	#else  // non emulation - use real hardware devices
	OneWireTempSensor directFridgeSensor(fridgeSensorPin);
	OneWireTempSensor directBeerSensor(beerSensorPin);
	#endif

	#if BREWPI_EMULATE || BREWPI_SIMULATE
	ValueActuator heater;
	ValueActuator cooler;
	ValueSensor<bool> doorSensor((bool)false);
	#else
	DigitalPinActuator<heatingPin, SHIELD_INVERT> heater;
	DigitalPinActuator<coolingPin, SHIELD_INVERT> cooler;
	ValueSensor<bool> doorSensor((bool)false);
	//DigitalPinSensor<doorPin, SHIELD_INVERT, USE_INTERNAL_PULL_UP_RESISTORS> doorSensor;
	#endif

	#if LIGHT_AS_HEATER
	Actuator& light = heater;
	#else
	ValueActuator lightOn;	// eventually map the light to a real pin
	Actuator& light = lightOn;
	#endif

	TempSensor fridgeSensor(&directFridgeSensor);
	TempSensor beerSensor(&directBeerSensor);

	#if MULTICHAMBER
	MockTempSensor directFridgeSensor2(10,10);
	MockTempSensor directBeerSensor2(5,5);
	#if 1
	TempSensor fridgeSensor2(directFridgeSensor2);
	TempSensor beerSensor2(directBeerSensor2);
	#if 1 || BREWPI_EMULATE		// use emulator for now until we get multi-define one wire
	ValueActuator heat2;
	ValueActuator cool2;
	#endif
	#endif
	Chamber c1(fridgeSensor, beerSensor, cooler, heater, light, door);
	Chamber c2(fridgeSensor2, beerSensor2, cool2, heat2, light, door);

	Chamber* chambers[] = {
		&c1
		, &c2
	};

	ChamberManager chamberManager(chambers, sizeof(chambers)/sizeof(chambers[0]));
	#endif

	#if MULTICHAMBER
	chamberManager.init();
	for (chamber_id i=0; i<chamberManager.chamberCount(); i++) {
		chamberManager.initChamber(i);
		piLink.printFridgeAnnotation(PSTR("Arduino restarted. Chamber %d ready!"), i+1);
	}
	chamberManager.switchChamber(0);
	#else
	// setup state
	tempControl.beerSensor = &beerSensor;
	tempControl.fridgeSensor = &fridgeSensor;
	tempControl.cooler = &cooler;
	tempControl.heater = &heater;
	tempControl.door = &doorSensor;
	tempControl.light = &light;

	tempControl.loadSettingsAndConstants(); //read previous settings from EEPROM
	tempControl.init();
	tempControl.updatePID();
	tempControl.updateState();
		
	#endif
	#endif
}

/**
 * Creates a new device for the given config.
 */
void* DeviceManager::createDevice(AnyDeviceConfig& config, DeviceType dt)
{
	switch (config.deviceHardware) {
		case DEVICE_HARDWARE_NONE:
			break;
		case DEVICE_HARDWARE_PIN:
			if (dt==DEVICETYPE_SWITCH_SENSOR)
				return new DigitalPinSensor(config.hw.pinNr, config.hw.invert);
			else
				return new DigitalPinActuator(config.hw.pinNr, config.hw.invert);
		case DEVICE_HARDWARE_ONEWIRE_TEMP:
			return new OneWireTempSensor(oneWireBus(config.hw.pinNr), config.hw.address);
		case DEVICE_HARDWARE_ONEWIRE_2413:
			return new OneWireActuator(oneWireBus(config.hw.pinNr), config.hw.address, config.hw.pio, config.hw.invert);
	}	
	return NULL;
}

/**
 * Returns the pointer to where the device pointer resides. This can be used to delete the current device and install a new one. 
 * For Temperature sensors, the returned pointer points to a TempSensor*. The basic device can be fetched by calling
 * TempSensor::getSensor().
 */
inline void** deviceTarget(AnyDeviceConfig& config)	
{
	// for multichamber, will write directly to the multi-chamber managed storage.
	// later...	
	if (config.chamber>1 || config.beer>1)
		return NULL;
	
	void** ppv;
	switch (config.deviceFunction) {
	case DEVICE_CHAMBER_DOOR:
		ppv = (void**)&(tempControl.door);
		break;
	case DEVICE_CHAMBER_LIGHT:
		ppv = (void**)&(tempControl.light);
		break;
	case DEVICE_CHAMBER_HEAT:
		ppv = (void**)&(tempControl.heater);
		break;
	case DEVICE_CHAMBER_COOL:
		ppv = (void**)&(tempControl.cooler);
		break;
	case DEVICE_CHAMBER_TEMP:
		ppv = (void**)&(tempControl.fridgeSensor);
		break;
	case DEVICE_BEER_TEMP:
		ppv = (void**)&(tempControl.beerSensor);
		break;	
	default:
		ppv = NULL;
	}
	return ppv;
}

/**
 * Creates and installs a device in the current chamber. 
 */
void DeviceManager::installDevice(AnyDeviceConfig& config)
{
	// where the newly constructed device will be placed.
	void** ppv = deviceTarget(config);
		
	DeviceType dc = deviceType(config.deviceFunction);
	
	if (ppv!=NULL) {
		switch(dc) {
			case DEVICETYPE_NONE:
				break;
			case DEVICETYPE_TEMP_SENSOR:				
				if (ppv==NULL)
					*ppv = new TempSensor();				// this is only allocated once. 
				((TempSensor*)*ppv)->setSensor((BasicTempSensor*)createDevice(config, dc));
				break;
			case DEVICETYPE_SWITCH_ACTUATOR:
				delete (Actuator*)*ppv;
				*ppv = createDevice(config, dc);
				break;
			case DEVICETYPE_SWITCH_SENSOR:
				delete (SwitchSensor*)*ppv;
				*ppv = createDevice(config, dc);				
				break;
		}
	}
}	

struct DeviceDefinition {
	int8_t id;
	int8_t chamber;
	int8_t beer;
	int8_t deviceFunction;
	int8_t deviceHardware;
	int8_t pinNr;
	int8_t invert;
	int8_t pio;
	DeviceAddress address;
		
	/**
	 * Lists the first letter of the key name for each attribute.
	 */
	static const char ORDER[10];
};

const char DeviceDefinition::ORDER[10] = "icbfhnxpa";

const char DEVICE_ATTRIB_INDEX = 'i';
const char DEVICE_ATTRIB_CHAMBER = 'c';
const char DEVICE_ATTRIB_BEER = 'b';
const char DEVICE_ATTRIB_FUNCTION = 'f';
const char DEVICE_ATTRIB_HARDWARE = 'h';
const char DEVICE_ATTRIB_PIN = 'n';
const char DEVICE_ATTRIB_INVERT = 'x';
const char DEVICE_ATTRIB_PIO = 'p';
const char DEVICE_ATTRIB_TYPE = 't';

inline uint8_t indexOf(const char* s, char c)
{
	char c2;
	uint8_t idx = -1;
	while ((c2=s[++idx]))
	{
		if (c==c2)	return idx;
	}
	return -1;
}

void handleDeviceDefinition(const char* key, const char* val, void* pv)
{
	DeviceDefinition* def = (DeviceDefinition*) pv;

	// the characters are listed in the same order as the DeviceDefinition struct.
	uint8_t idx = indexOf(DeviceDefinition::ORDER, key[0]);
	if (idx==sizeof(DeviceDefinition::ORDER-2))	// last character
		parseBytes(def->address, val, 8);
	else if (idx>=0) 
		((uint8_t*)def)[idx] = strtoul(val, NULL, 10);	
}

inline void fill(int8_t* p, uint8_t size) {
	while (size-->0) *p++ = -1;	
}
inline void clear(int8_t* p, uint8_t size) {
	while (size-->0) *p++ = 0;
}

void DeviceManager::parseDeviceDefinition(Stream& p, char end, AnyDeviceConfig& target)
{	
	static DeviceDefinition dev;
	fill((int8_t*)&dev, sizeof(dev));	
	
	piLink.parseJson(&handleDeviceDefinition, &dev);
	
	// todo - should initialize eeprom if not already done.
	
	eepromManager.fetchDevice(target, dev.id);
	
	if (dev.chamber>=0) target.chamber = dev.chamber;
	if (dev.beer>=0) target.beer = dev.beer;
	if (dev.deviceFunction>=0 && dev.deviceFunction!=target.deviceFunction) {
		target.deviceFunction = DeviceFunction(dev.deviceFunction);
		target.deviceHardware = DEVICE_HARDWARE_NONE;
	}		
	if (dev.deviceHardware>=0) {
		target.deviceHardware = DeviceHardware(dev.deviceHardware);
		clear((int8_t*)&target.hw, sizeof(target.hw));
	}

	if (dev.pinNr>=0) target.hw.pinNr = dev.pinNr;
	if (dev.pio>=0) target.hw.pio = dev.pio;
	if (dev.invert>=0) target.hw.invert = dev.invert;
	if (dev.address[0]!=0xFF) memcpy(target.hw.address, dev.address, 8);
	
	eepromManager.storeDevice(target, dev.id);
}


void print_P(Print& p, const char* data)
{
	char c;
	while((c=pgm_read_byte(data++)))
		p.print(c);
}

inline void print_attrib(Print& p, char c, int8_t val, bool first=false)
{	
	if (!first)
		p.print(',');
	p.print(c);
	p.print(':');
	p.print(val);
}

inline bool hasInvert(DeviceHardware hw)
{
	return hw==DEVICE_HARDWARE_ONEWIRE_2413 || hw==DEVICE_HARDWARE_PIN;
}

inline bool hasOnewire(DeviceHardware hw)
{
	return hw==DEVICE_HARDWARE_ONEWIRE_2413 || hw==DEVICE_HARDWARE_ONEWIRE_TEMP;
}

void DeviceManager::printDevice(AnyDeviceConfig& config, Print& p)
{	
	DeviceType dt = deviceType(config.deviceFunction);
		
	print_attrib(p, DEVICE_ATTRIB_CHAMBER, config.chamber, true);
	print_attrib(p, DEVICE_ATTRIB_BEER, config.beer);
	print_attrib(p, DEVICE_ATTRIB_FUNCTION, config.deviceFunction);
	print_attrib(p, DEVICE_ATTRIB_TYPE, dt);
	print_attrib(p, DEVICE_ATTRIB_HARDWARE, config.deviceHardware);		
	print_attrib(p, DEVICE_ATTRIB_PIN, config.hw.pinNr);
	
	char buf[17];

	if (hasInvert(config.deviceHardware))	
		print_attrib(p, DEVICE_ATTRIB_INVERT, config.hw.invert);
	if (hasOnewire(config.deviceHardware)) {
		p.print(",a:");
		printBytes(config.hw.address, 8, buf);
		p.print(buf);
	}		
	if (config.deviceHardware==DEVICE_HARDWARE_ONEWIRE_2413) {
		print_attrib(p, DEVICE_ATTRIB_PIO, config.hw.pio);		
	}		
					
}	
	
bool DeviceManager::allDevices(AnyDeviceConfig& config, uint8_t deviceIndex)
{	
	return eepromManager.fetchDevice(config, deviceIndex);
}	

void parseBytes(uint8_t* data, const char* s, uint8_t len) {
	char c;
	while ((c=*s++)) {
		uint8_t d = (c>='A' ? c-'A'+10 : c-'0')<<4;
		c = *s++;
		d |= (c>='A' ? c-'A'+10 : c-'0');
		*data++ = d;
	}
}

void printBytes(uint8_t* data, uint8_t len, char* buf) // prints 8-bit data in hex
{
	for (int i=0; i<len; i++) {
		uint8_t b = (data[i] >> 4) & 0x0f;
		*buf++ = b + (b>9 ? 48+39 : 48);
		b = data[i] & 0x0f;
		*buf++ = b + (b>9 ? 48+39 : 48);
	}
	*buf = 0;
}


