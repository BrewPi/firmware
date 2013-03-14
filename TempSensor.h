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

#include "CascadedFilter.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "temperatureFormats.h"
#include "pins.h"
#include <stdlib.h>

class TempSensor{
	public:
	TempSensor(const uint8_t pinNumber) : pinNr(pinNumber){
		lastRequestTime = 0;
		connected = 0;
		updateCounter = 255; // first update for slope filter after (255-13s)
		oneWire = new OneWire(pinNr);
		sensor = new DallasTemperature(oneWire);
	};
		
	~TempSensor(){
		delete oneWire;
		delete sensor;
	};
		
	void init();
	
	bool isConnected(void){
		return connected;
	}
	
	void update(void);
	fixed7_9 read(void);
	fixed7_9 readFastFiltered(void);

	fixed7_9 readSlowFiltered(void){
		return slowFilter.readOutput(); //return most recent unfiltered value
	}
	fixed7_9 readSlope(void);
	
	fixed7_9 detectPosPeak(void){
		return slowFilter.detectPosPeak();
	}
	
	fixed7_9 detectNegPeak(void){
		return slowFilter.detectNegPeak();
	}
	
	void setFastFilterCoefficients(uint8_t b){
		fastFilter.setCoefficients(b);
	}
	
	void setSlowFilterCoefficients(uint8_t b){
		slowFilter.setCoefficients(b);
	}

	void setSlopeFilterCoefficients(uint8_t b){
		slopeFilter.setCoefficients(b);
	}			
	
	private:
	const uint8_t pinNr;
	bool connected;
	unsigned long lastRequestTime; // in milliseconds
	unsigned char updateCounter;
	fixed7_25 prevOutputForSlope;	
	
	CascadedFilter fastFilter;
	CascadedFilter slowFilter;
	CascadedFilter slopeFilter;
	
	OneWire * oneWire;
	DallasTemperature * sensor;
	DeviceAddress sensorAddress;
};



#endif /* SENSORS_H_ */