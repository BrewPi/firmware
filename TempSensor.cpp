/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
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
#include "TempSensor.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "PiLink.h"
#include "Ticks.h"

void TempSensor::init()
{		
	fixed7_9 temperature = _sensor->init();
	if (temperature!=DEVICE_DISCONNECTED) {
		fastFilter.init(temperature);
		slowFilter.init(temperature);
		slopeFilter.init(0);
		prevOutputForSlope = slowFilter.readOutputDoublePrecision();		
	}
}

void TempSensor::update()
{	
	if (!_sensor) return;
	
	fixed7_9 temperature = _sensor->read();
	if (temperature==DEVICE_DISCONNECTED)
		return;
		
	fastFilter.add(temperature);
	slowFilter.add(temperature);
		
	// update slope filter every 12 samples.
	// averaged differences will give the slope. Use the slow filter as input
	updateCounter--;
	// initialize first read for slope filter after (255-13) seconds. This prevents an influence for the startup inaccuracy.
	if(updateCounter == 13){
		// only happens once after startup.
		prevOutputForSlope = slowFilter.readOutputDoublePrecision();
	}
	if(updateCounter == 0){
		slopeFilter.addDoublePrecision(slowFilter.readOutputDoublePrecision() - prevOutputForSlope);
		prevOutputForSlope = slowFilter.readOutputDoublePrecision();
		updateCounter = 12;
	}		
}

fixed7_9 TempSensor::readFastFiltered(void){
	return fastFilter.readOutput(); //return most recent unfiltered value
}

fixed7_9 TempSensor::readSlope(void){
	// return slope per hour. Multiply by 300 (1h/12s), shift to single precision
	fixed7_25 doublePrecision = slopeFilter.readOutputDoublePrecision();
	return (doublePrecision*300)>>16;
}

fixed7_9 TempSensor::detectPosPeak(void){
	return slowFilter.detectPosPeak();
}
	
fixed7_9 TempSensor::detectNegPeak(void){
	return slowFilter.detectNegPeak();
}
	
void TempSensor::setFastFilterCoefficients(uint8_t b){
	fastFilter.setCoefficients(b);
}
	
void TempSensor::setSlowFilterCoefficients(uint8_t b){
	slowFilter.setCoefficients(b);
}

void TempSensor::setSlopeFilterCoefficients(uint8_t b){
	slopeFilter.setCoefficients(b);
}

BasicTempSensor& TempSensor::sensor() {
	return *_sensor;
}
