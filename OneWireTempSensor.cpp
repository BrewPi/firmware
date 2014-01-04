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
bool OneWireTempSensor::init(){

	// save address and pinNr for log messages
	char addressString[17];
	printBytes(sensorAddress, 8, addressString);
	uint8_t pinNr = oneWire->pinNr();

	bool success = false;

	if (sensor==NULL) {
		sensor = new DallasTemperature(oneWire);
		if (sensor==NULL) {
			logErrorString(ERROR_SRAM_SENSOR, addressString);
		}
	}
	
	// This quickly tests if the sensor is connected and initializes the reset detection.
	// During the main TempControl loop, we don't want to spend many seconds
	// scanning each sensor since this brings things to a halt.
	if (sensor && sensor->initConnection(sensorAddress) && requestConversion()) {
		waitForConversion();
		temperature temperature = readAndConstrainTemp();
		DEBUG_ONLY(logInfoIntStringTemp(INFO_TEMP_SENSOR_INITIALIZED, pinNr, addressString, temperature));
		success = temperature!=DEVICE_DISCONNECTED && requestConversion();
	}	
	setConnected(success);
	return success;
}

bool OneWireTempSensor::requestConversion()
{	
	bool ok = sensor->requestTemperaturesByAddress(sensorAddress);
	setConnected(ok);
	return ok;
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

temperature OneWireTempSensor::read(){
	
	if (!connected)
		return TEMP_SENSOR_DISCONNECTED;
	
	temperature temperature = readAndConstrainTemp();
	requestConversion();
	return temperature;
}

temperature OneWireTempSensor::readAndConstrainTemp()
{
	temperature temperature = sensor->getTempRaw(sensorAddress);
	if(temperature == DEVICE_DISCONNECTED){
		setConnected(false);
		return TEMP_SENSOR_DISCONNECTED;
	}
	temperature = constrainTemp(temperature+calibrationOffset+(C_OFFSET>>5), ((int) MIN_TEMP)>>5, ((int) MAX_TEMP)>>5)<<5;
	return temperature;
}
