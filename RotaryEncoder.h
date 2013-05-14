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


#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

#include "Brewpi.h"
#include <inttypes.h>

// 500 us idle time for debouncing
#define ROTARY_THRESHOLD 500u

class RotaryEncoder
{
	public:
	void init(void);
	void setRange(int start, int min, int max);
	
	void pinAHandler(bool pinState);
	void pinBHandler(bool pinState);
	
		
	bool changed(void); // returns one if the value changed since the last call of changed.
	int read(void);
	
	int readHalfSteps(void){
		return halfSteps;
	}

	bool pushed(void){
		return pushFlag;
	}

	void resetPushed(void){
		pushFlag = false;
	}	
	
	void setPushed(void);
	
	private:
	
	int maximum;
	int minimum;
	int prevRead;
	volatile int halfSteps;
	volatile bool pushFlag;
	
	volatile uint8_t pinASignal;
	volatile uint8_t pinBSignal;
	volatile uint8_t pinAHistory;
	volatile uint8_t pinBHistory;
	volatile unsigned long pinATime;
	volatile unsigned long pinBTime;	
};

extern RotaryEncoder rotaryEncoder;

#endif /* ROTARYENCODER_H_ */