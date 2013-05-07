/*
 * Copyright 2012 BrewPi/Elco Jacobs.
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


#ifndef SENSORS_H_
#define SENSORS_H_

#include "brewpi_avr.h"
#include "CascadedFilter.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "temperatureFormats.h"
#include "pins.h"
#include <stdlib.h>

#ifndef TEMP_SENSOR_CASCADED_FILTER 
#define TEMP_SENSOR_CASCADED_FILTER 1
#endif

#if TEMP_SENSOR_CASCADED_FILTER 
typedef CascadedFilter TempSensorFilter;
#else
typedef FixedFilter TempSensorFilter;
#endif

class BasicTempSensor
{
public:
	virtual ~BasicTempSensor() { }
	
	virtual bool isConnected(void) = 0;
	
	/*
	 * Attempt to (re-)initialize the sensor and fetch a sensor reading
	 * Returns a temperature reading, or DEVICE_DISCONNECTED
	 */
	virtual fixed7_9 init() =0;

	/*
	 * Fetch a new reading from the sensor
	 */
	virtual fixed7_9 read() = 0;
	
};

enum TempSensorType {
	TEMP_SENSOR_TYPE_FRIDGE=1,
	TEMP_SENSOR_TYPE_BEER
};


class TempSensor {
	public:	
	TempSensor(TempSensorType sensorType, BasicTempSensor* sensor =NULL) : _sensor(sensor)  {
		updateCounter = 255; // first update for slope filter after (255-13s)
	 }	 	 
	 
	 void setSensor(BasicTempSensor* sensor) {
		 _sensor = sensor;
	 }

	bool hasSlowFilter() { return true; }
	bool hasFastFilter() { return true; }
	bool hasSlopeFilter() { return true; }
	
	void init();
	
	bool isConnected() { return _sensor->isConnected(); }
	
	void update();
	
	fixed7_9 readFastFiltered(void);

	fixed7_9 readSlowFiltered(void){
		return slowFilter.readOutput(); //return most recent unfiltered value
	}
	
	fixed7_9 readSlope(void);
	
	fixed7_9 detectPosPeak(void);
	
	fixed7_9 detectNegPeak(void);
	
	void setFastFilterCoefficients(uint8_t b);
	
	void setSlowFilterCoefficients(uint8_t b);

	void setSlopeFilterCoefficients(uint8_t b);
	
	BasicTempSensor& sensor();
	 
	private:	
	BasicTempSensor* _sensor;
	TempSensorFilter fastFilter;
	TempSensorFilter slowFilter;
	TempSensorFilter slopeFilter;
	unsigned char updateCounter;
	fixed7_25 prevOutputForSlope;
			
	friend class ChamberManager;
	friend class Chamber;
	friend class DeviceManager;
};


#endif /* SENSORS_H_ */