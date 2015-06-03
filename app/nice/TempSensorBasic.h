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

#pragma once

#include "TemperatureFormats.h"
#include "Values.h"

#define TEMP_SENSOR_DISCONNECTED INVALID_TEMP

/**
 * A temperature sensor. The value for the temperature is exposed at index 0.
 * The connected status is exposed at index 1. 
 */
// todo - rename this TemperatureSensor
// rename the old TempSensor as FilteredTemperatureSensor
class BasicTempSensor : public Container
{
	BasicReadValue<temperature> tempValue;
	BasicReadValue<bool> connected;
	
public:
	virtual ~BasicTempSensor() { }
	
	/**
	 * Fetches the values from the temp sensor and assigns them to local values.
	 */
	void update() {
		connected.assign(isConnected());
		tempValue.assign(read());
	}
	
	Object* item(container_id id) {
		switch (id) {
			case 0:
				return &tempValue;
			case 1:
				return &connected;			
		}
		return NULL;
	}
	
	
	virtual bool isConnected(void) = 0;		
	
	/*
	 * Attempt to (re-)initialize the sensor. 	 
	 */
	virtual bool init() =0;

	/*
	 * Fetch a new reading from the sensor
	 */
	virtual temperature read() = 0;
	
};
