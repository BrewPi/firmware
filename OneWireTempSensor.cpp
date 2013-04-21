/*
 * OneWireTempSensor.cpp
 */ 

#include <limits.h>
#include "Arduino.h"
#include "OneWireTempSensor.h"
#include "DallasTemperature.h"
#include "OneWire.h"
#include "PiLink.h"
#include "Ticks.h"

fixed7_9 OneWireTempSensor::init(){
	
	if (oneWire==NULL)
	{
		oneWire = new OneWire(pinNr);
		sensor = new DallasTemperature(oneWire);
		if (oneWire==NULL || sensor==NULL) {			
			if (oneWire) delete oneWire;
			DEBUG_MSG(PSTR("Not enough SRAM for temp sensors"));
			return DEVICE_DISCONNECTED;
		}			
	}
	
	// give reset pulse to temp sensors
	oneWire->reset();

	DEBUG_MSG(PSTR("Fetching address for sensor %d"), pinNr);

	// get sensor address
	if (!sensor->getAddress(sensorAddress, 0)){
		// error no sensor found
		DEBUG_MSG(PSTR("No address for sensor %d"), pinNr);
		if(ticks.seconds() < 4){
			// only log this debug message at startup
			piLink.debugMessage(PSTR("Unable to find address for sensor on pin %d"), pinNr);
		}
		return DEVICE_DISCONNECTED;
	}
	sensor->setResolution(sensorAddress, 12);
	sensor->setWaitForConversion(false);
	
	sensor->requestTemperatures();
	lastRequestTime = ticks.millis();
	wait.millis(750); // delay 750ms for conversion time
	
	// todo - DEVICE_DISCONNECTED is -127, but does this need scaling?
	DEBUG_MSG("fetching initial temperature %d", pinNr);
	
	fixed7_9 temperature = DEVICE_DISCONNECTED;
	while(temperature == DEVICE_DISCONNECTED){
		temperature = sensor->getTempRaw(sensorAddress);
		DEBUG_MSG("Raw temp on pin %d %d", pinNr, temperature);
		if(ticks.millis() - lastRequestTime > 4000){
			connected = false; // sensor disconnected
			DEBUG_MSG("Reporting device disconnected on pin %d", pinNr);
			return DEVICE_DISCONNECTED;
		}
	}
	sensor->requestTemperatures();
	wait.millis(750);
	temperature = sensor->getTempRaw(sensorAddress); // read again. First read is not accurate
	connected = true;
	temperature = constrain(temperature, ((int) INT_MIN)>>5, ((int) INT_MAX)>>5)<<5; // sensor returns 12 bits with 4 fraction bits. Store with 9 fraction bits
	DEBUG_MSG(PSTR("Sensor initialized: pin %d"), pinNr);
	return temperature;
}


fixed7_9 OneWireTempSensor::read(){
	
	if (!connected)
		return DEVICE_DISCONNECTED;
	
	if((ticks.millis()-lastRequestTime) > 5000){ // if last request is longer than 5 seconds ago, request again and delay
		sensor->requestTemperatures();
		lastRequestTime = ticks.millis();
		wait.millis(750); // wait 750 ms (18B20 max conversion time)
	}
	fixed7_9 temperature = sensor->getTempRaw(sensorAddress);
	if(temperature == DEVICE_DISCONNECTED){
		// device disconnected. Don't update filters.  Log a debug message.
		if(connected == true){
			piLink.debugMessage(PSTR("Temperature sensor on pin %d disconnected"), pinNr);
		}
		connected = false;
		return DEVICE_DISCONNECTED;
	}
	else{
		if(connected == false){
			wait.millis(2000); // delay for two seconds to be sure sensor is correctly inserted
			init(); // was disconnected, initialize again
			piLink.debugMessage(PSTR("Temperature sensor on pin %d reconnected"), pinNr);
			temperature = sensor->getTempRaw(sensorAddress); // re-read temperature after proper initialization
		}
	}
	temperature = constrain(temperature, ((int) INT_MIN)>>5, ((int) INT_MAX)>>5)<<5; // sensor returns 12 bits with 4 fraction bits. Store with 9 fraction bits

	// already send request for next read
	sensor->requestTemperatures();
	lastRequestTime = ticks.millis();
	return temperature;
}
