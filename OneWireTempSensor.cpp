/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
 * Copyright 2013 Matthew McGowan.
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

#include "Brewpi.h"
#include "OneWireTempSensor.h"
#include "DallasTemperature.h"
#include "OneWire.h"
#include "OneWireDevices.h"
#include "PiLink.h"
#include "Ticks.h"

OneWireTempSensor::~OneWireTempSensor(){
	delete sensor;
};

/**
 * Initializes the temperature sensor.
 * This method is called when the sensor is first created and also any time the sensor reports it's disconnected.
 * If the result is TEMP_SENSOR_DISCONNECTED then subsequent calls to read() will also return TEMP_SENSOR_DISCONNECTED.
 * Clients should attempt to re-initialize the sensor by calling init() again. 
 */
fixed7_9 OneWireTempSensor::init(){

	// save address and pinNr for debug messages
	char addressString[17];
	printBytes(sensorAddress, 8, addressString);

	uint8_t pinNr = oneWire->pinNr();

	if (sensor==NULL) {
		sensor = new DallasTemperature(oneWire);
		if (sensor==NULL) {
			logErrorString(ERROR_SRAM_SENSOR, addressString);
			setConnected(false);
			return TEMP_SENSOR_DISCONNECTED;
		}
	}
	
	// get sensor address - todo this is deprecated and will be phased out. Needed to support revA shields
#if BREWPI_STATIC_CONFIG==BREWPI_SHIELD_REV_A	
	if (!sensorAddress[0]) {
		if (!sensor->getAddress(sensorAddress, 0)) {
			// error no sensor found
			logErrorInt(ERROR_SENSOR_NO_ADDRESS_ON_PIN, pinNr);
			setConnected(false);
			return TEMP_SENSOR_DISCONNECTED;
		}
		else {
			#if (BREWPI_DEBUG > 0)
			printBytes(sensorAddress, 8, addressString);
			#endif	
		}
	}
#endif

	// This quickly tests if the sensor is connected. Suring the main TempControl loop, we don't want to spend many seconds
	// scanning each sensor since this brings things to a halt.
	if (!sensor->isConnected(sensorAddress)) {
		setConnected(false);
		return TEMP_SENSOR_DISCONNECTED;		
	}
		
	logDebug("Fetching initial temperature of sensor %s", addressString);
	
	sensor->setResolution(sensorAddress, 12);
	sensor->setWaitForConversion(false);
		
	// read initial temperature twice - first read is inaccurate
	fixed7_9 temperature;
	for (int i=0; i<2; i++) {
		temperature = DEVICE_DISCONNECTED;
		lastRequestTime = ticks.seconds();
		while(temperature == DEVICE_DISCONNECTED){
			sensor->requestTemperatures();
			waitForConversion();
			temperature = sensor->getTempRaw(sensorAddress);
			logDebug("Sensor initial temp read: pin %d %s %d", this->oneWire->pinNr(), addressString, temperature);
			if(ticks.timeSince(lastRequestTime) > 4) {
				setConnected(false);
				return TEMP_SENSOR_DISCONNECTED;
			}
		}
	}
	// sensor returns 12 bits with 4 fraction bits. Store with 9 fraction bits and add the offset for storage
	temperature = constrainTemp(temperature+calibrationOffset+(C_OFFSET>>5), ((int) INT_MIN)>>5, ((int) INT_MAX)>>5)<<5;
	DEBUG_ONLY(logInfoIntStringTemp(INFO_TEMP_SENSOR_INITIALIZED, pinNr, addressString, temperature);)
	
	setConnected(true);
	return temperature;
}

void OneWireTempSensor::waitForConversion()
{
	wait.millis(750);
}

void OneWireTempSensor::setConnected(bool connected) {
	if (this->connected==connected)
		return; // state is stays the same
		
	char addressString[17];
	printBytes(sensorAddress, 8, addressString);
	this->connected = connected;
	if(connected){
		logInfoIntString(INFO_TEMP_SENSOR_CONNECTED, this->oneWire->pinNr(), addressString);
	}
	else{
		logWarningIntString(WARNING_TEMP_SENSOR_DISCONNECTED, this->oneWire->pinNr(), addressString);
	}
}

fixed7_9 OneWireTempSensor::read(){
	if (!connected)
		return TEMP_SENSOR_DISCONNECTED;
	
	if(ticks.timeSince(lastRequestTime) > 5){ // if last request is longer than 5 seconds ago, request again and delay
		sensor->requestTemperatures();
		lastRequestTime = ticks.seconds();
		waitForConversion();
	}
	fixed7_9 temperature = sensor->getTempRaw(sensorAddress);
	if(temperature == DEVICE_DISCONNECTED){
		setConnected(false);
		return TEMP_SENSOR_DISCONNECTED;
	}	
	// sensor returns 12 bits with 4 fraction bits. Store with 9 fraction bits and add the offset for storage
	temperature = constrainTemp(temperature+calibrationOffset+(C_OFFSET>>5), ((int) INT_MIN)>>5, ((int) INT_MAX)>>5)<<5;

	// already send request for next read
	sensor->requestTemperatures();
	lastRequestTime = ticks.seconds();
	return temperature;
}
