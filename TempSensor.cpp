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
#include "PiLink.h"
#include "Ticks.h"

void TempSensor::init()
{		
	fixed7_9 temperature = _sensor->init();
	if (temperature!=TEMP_SENSOR_DISCONNECTED) {
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
	if (temperature==TEMP_SENSOR_DISCONNECTED)
		return;
		
	fastFilter.add(temperature);
	slowFilter.add(temperature);
		
	// update slope filter every 3 samples.
	// averaged differences will give the slope. Use the slow filter as input
	updateCounter--;
	// initialize first read for slope filter after (255-4) seconds. This prevents an influence for the startup inaccuracy.
	if(updateCounter == 4){
		// only happens once after startup.
		prevOutputForSlope = slowFilter.readOutputDoublePrecision();
	}
	if(updateCounter == 0){
		fixed7_25 slowFilterOutput = slowFilter.readOutputDoublePrecision();
		fixed7_25 diff =  slowFilterOutput - prevOutputForSlope;
		fixed7_9 diff_upper = diff >> 16;
		if(diff_upper > 27){ // limit to prevent overflow INT_MAX/1200 = 27.14
			diff = (27l << 16);
		}
		else if(diff_upper < -27){
			diff = (-27l << 16);
		}
		slopeFilter.addDoublePrecision(1200*diff); // Multiply by 1200 (1h/4s), shift to single precision
		prevOutputForSlope = slowFilterOutput;
		updateCounter = 3;
	}
}

fixed7_9 TempSensor::readFastFiltered(void){
	return fastFilter.readOutput(); //return most recent unfiltered value
}

fixed7_9 TempSensor::readSlope(void){
	// return slope per hour. 
	fixed7_25 doublePrecision = slopeFilter.readOutputDoublePrecision();
	return doublePrecision>>16; // shift to single precision
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
