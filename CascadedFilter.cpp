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
#include "CascadedFilter.h"
#include <stdlib.h>
#include <limits.h>
#include "temperatureFormats.h"

CascadedFilter::CascadedFilter() {
	for(uint8_t i=0; i<NUM_SECTIONS; i++){
		sections[i].setCoefficients(2); // default to a b value of 2
	}	
}

void CascadedFilter::setCoefficients(uint8_t bValue){
	for(uint8_t i=0; i<NUM_SECTIONS; i++){
		sections[i].setCoefficients(bValue);
	}
}

fixed7_9 CascadedFilter::add(fixed7_9 val){
	fixed7_25 valDoublePrecision = ((fixed7_25) val) << 16;
	valDoublePrecision = addDoublePrecision(valDoublePrecision);
	// return output, shifted back to single precision
	return valDoublePrecision >> 16;
}

fixed7_25 CascadedFilter::addDoublePrecision(fixed7_25 val){
	fixed7_25 input = val;
	// input is input for next section, which is the output of the previous section
	for(uint8_t i=0; i<NUM_SECTIONS; i++){
		input = sections[i].addDoublePrecision(input);
	}
	return input;
}


fixed7_9 CascadedFilter::readInput(void){
	return sections[0].readInput(); // return input of first section
}

fixed7_25 CascadedFilter::readOutputDoublePrecision(void){
	return sections[NUM_SECTIONS-1].readOutputDoublePrecision(); // return output of last section
}

fixed7_25 CascadedFilter::readPrevOutputDoublePrecision(void){
	return sections[NUM_SECTIONS-1].readPrevOutputDoublePrecision(); // return previous output of last section
}

void CascadedFilter::init(fixed7_9 val){
	for(uint8_t i=0; i<NUM_SECTIONS; i++){
		sections[i].init(val);
	}
}


