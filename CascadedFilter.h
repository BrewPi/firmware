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


#ifndef CASCADEDFILTER_H_
#define CASCADEDFILTER_H_


#include "temperatureFormats.h"
#include "FixedFilter.h"

// Use 3 filter sections. This gives excellent filtering, without adding too much delay.
// For 3 sections the stop band attenuation is 3x the single section attenuation in dB.
// The delay is also tripled.
#define NUM_SECTIONS 3

class CascadedFilter{
	public:
	// CascadedFilter implements a filter that consists of multiple second order secions.
	FixedFilter sections[NUM_SECTIONS];
		
	public:
	CascadedFilter();
	~CascadedFilter() {}
	void init(fixed7_9 val);
	void setCoefficients(uint8_t bValue);
	fixed7_9 add(fixed7_9 val); // adds a value and returns the most recent filter output
	fixed7_25 addDoublePrecision(fixed7_25 val);
	fixed7_9 readInput(void); // returns the most recent filter input

	fixed7_9 readOutput(void){
		return sections[NUM_SECTIONS-1].readOutput(); // return output of last section
	}
	fixed7_25 readOutputDoublePrecision(void);
	fixed7_25 readPrevOutputDoublePrecision(void);
	
	fixed7_9 detectPosPeak(void){
		return sections[NUM_SECTIONS-1].detectPosPeak(); // detect peaks in last section
	}
	fixed7_9 detectNegPeak(void){
		return sections[NUM_SECTIONS-1].detectNegPeak(); // detect peaks in last section
	}
};


#endif /* CASCADEDFILTER_H_ */