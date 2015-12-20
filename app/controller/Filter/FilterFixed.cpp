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
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Brewpi.h"
#include "temperatureFormats.h"
#include "FilterFixed.h"
#include <stdlib.h>

temp_t FixedFilter::add(temp_t val){
    temp_precise_t p = val;
    // return output, converted back to normal precision
	return add(p);
}

temp_precise_t FixedFilter::add(temp_precise_t val){
	xv[2] = xv[1];
	xv[1] = xv[0];
	xv[0] = val;
	
	yv[2] = yv[1];
	yv[1] = yv[0];
	
	/* Implementation that prevents overflow as much as possible by order of operations: */
	yv[0] = (yv[1] - yv[2]) + yv[1]; // expected value + 1*
	yv[0] -= (yv[1]>>b); // expected value -1>>b*
    yv[0] += (yv[2]>>b); // expected value +1>>b*
	temp_precise_t temporary = (xv[0]>>a) + (xv[1]>>uint8_t(a-1)) + (xv[2]>>a); // expected value +(1>>(a-2))
	temporary -= (yv[2]>>uint8_t(a-2)); // expected value -(1>>(a-2))
	yv[0] += temporary;
	
	return yv[0];
}

void FixedFilter::init(temp_precise_t val){
        xv[0] = val;
        xv[1] = val;
        xv[2] = val;

        yv[0] = val;
        yv[1] = val;
        yv[2] = val;
}

bool FixedFilter::detectPosPeak(temp_precise_t * peak){
	if(yv[0] < yv[1] && yv[1] >= yv[2]){
		*peak = yv[1];
	    return true;
	}
	else{
		return false;
	}
}

bool FixedFilter::detectNegPeak(temp_precise_t * peak){
	if(yv[0] > yv[1] && yv[1] <= yv[2]){
	    *peak = yv[1];
        return true;
	}
	else{
		return false;
	}
}

bool FixedFilter::isRising(){
    return (yv[0] > yv[1]) && (yv[1] > yv[2]);
}

bool FixedFilter::isFalling(){
    return (yv[0] < yv[1]) && (yv[1] < yv[2]);
}
