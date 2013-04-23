/*
 * DeviceManager.cpp
 *
 * Created: 22/04/2013 22:02:14
 *  Author: mat
 */ 

#include "DeviceManager.h"
#include "TempControl.h"
#include "OneWireTempSensor.h"

DeviceManager deviceManager;

void DeviceManager::loadDefaultDevices()
{
	tempControl.beerSensor = new TempSensor(new OneWireTempSensor(beerSensorPin));
	tempControl.fridgeSensor = new TempSensor(new OneWireTempSensor(fridgeSensorPin));
		
	tempControl.cooler = new DigitalConstantPinActuator<coolingPin, SHIELD_INVERT>();
	tempControl.heater = new DigitalConstantPinActuator<heatingPin, SHIELD_INVERT>();
	tempControl.door = new DigitalConstantPinSensor<doorPin, SHIELD_INVERT, USE_INTERNAL_PULL_UP_RESISTORS>();
	tempControl.light = tempControl.heater;				
		
	//tempControl.loadSettingsAndConstants(); //read previous settings from EEPROM
	tempControl.init();
	tempControl.updatePID();
	tempControl.updateState();

		
		
	#if 0	// working code from multichamber
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
