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

#include "FixedFilter.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "temperatureFormats.h"
#include "pins.h"
#include <stdlib.h>

class TempSensor{
	public:
	TempSensor(const int pinNr) : pin(pinNr){
		lastRequestTime = 0;
		connected = 0;
		updateCounter = 255; // first update for slope filter after (255-31s)
		oneWire = new OneWire(pin);
		sensor = new DallasTemperature(oneWire);
	};
		
	~TempSensor(){
		delete oneWire;
		delete sensor;
	};
		
	void init();
	bool isConnected(void);
	void update(void);
	fixed7_9 read(void);
	fixed7_9 readFastFiltered(void);
	fixed7_9 readSlowFiltered(void);
	fixed7_9 readSlope(void);
	fixed7_9 detectPosPeak(void); //returns positive peak or INT_MIN when no peak has been found
	fixed7_9 detectNegPeak(void); //returns negative peak or INT_MIN when no peak has been found
	void setFastFilterCoefficients(uint16_t ab);
	void setSlowFilterCoefficients(uint16_t ab);
	void setSlopeFilterCoefficients(uint16_t ab);
			
	private:
	const int pin;
	bool connected;
	unsigned long lastRequestTime; // in milliseconds
	unsigned char updateCounter;
	fixed7_25 prevOutputForSlope;	
	
	FixedFilter fastFilter;
	FixedFilter slowFilter;
	FixedFilter slopeFilter;
	
	OneWire * oneWire;
	DallasTemperature * sensor;
	DeviceAddress sensorAddress;
};



#endif /* SENSORS_H_ */