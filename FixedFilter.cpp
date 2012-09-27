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

#include "FixedFilter.h"
#include <stdlib.h>
#include <limits.h>
#include "temperatureFormats.h"

FixedFilter::FixedFilter(){
	setCoefficients(SETTLING_TIME_50_SAMPLES); // default to 50 samples settling time
}

FixedFilter::~FixedFilter(){
	
}

void FixedFilter::setCoefficients(uint8_t aValue, uint8_t bValue){
	a = aValue;
	b = bValue;	
}

void FixedFilter::setCoefficients(uint16_t ab){
	a = (ab & 0xFF00)>>8;
	b = ab & 0x00FF;
}	

fixed7_9 FixedFilter::add(fixed7_9 val){
	fixed7_25 returnVal = addDoublePrecision( ((fixed7_25) val) << 16);
	return returnVal>>16;
}

fixed7_25 FixedFilter::addDoublePrecision(fixed7_25 val){
	xv[2] = xv[1];
	xv[1] = xv[0];
	xv[0] = val;
	
	yv[2] = yv[1];
	yv[1] = yv[0];
	
	/* Implementation that prevents overflow as much as possible by order of operations: */
	yv[0] = ((yv[1] - yv[2]) + yv[1]) // expected value + 1*
	- (yv[1]>>b) + (yv[2]>>b) + // expected value +0*
	+ (xv[0]>>a) + (xv[1]>>(a-1)) + (xv[2]>>a) // expected value +(1>>(a-2))
	- (yv[2]>>(a-2)); // expected value -(1>>(a-2))
	
	return yv[0];
}

fixed7_9 FixedFilter::readOutput(void){
	return yv[0]>>16; // return 16 most significant bits of most recent output
}

fixed7_9 FixedFilter::readInput(void){
	return xv[0]>>16; // return 16 most significant bits of most recent input
}

fixed7_25 FixedFilter::readOutputDoublePrecision(void){
	return yv[0];
}

fixed7_25 FixedFilter::readPrevOutputDoublePrecision(void){
	return yv[1];
}

void FixedFilter::init(fixed7_9 val){
		xv[0] = val;
		xv[0] = xv[0]<<16; // 16 extra bits are used in the filter for the fraction part

		xv[1] = xv[0];
		xv[2] = xv[0];
	
		yv[0] = xv[0];
		yv[1] = xv[0];
		yv[2] = xv[0];
}

fixed7_9 FixedFilter::detectPosPeak(void){
	if(yv[0] < yv[1] && yv[1] >= yv[2]){
		return yv[1]>>16;
	}
	else{
		return INT_MIN;
	}
}

fixed7_9 FixedFilter::detectNegPeak(void){
	if(yv[0] > yv[1] && yv[1] <= yv[2]){
		return yv[1]>>16;
	}
	else{
		return INT_MIN;
	}
}
