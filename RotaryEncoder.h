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

// Values returned by 'process'
// No complete step yet.
#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Anti-clockwise step.
#define DIR_CCW 0x20

class RotaryEncoder
{
	public:
	void init(void);
	void setRange(int start, int min, int max);
	void process(void);
			
	bool changed(void); // returns one if the value changed since the last call of changed.
	int read(void);
	
	int readsteps(void){
		return steps;
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
	volatile int steps;
	volatile bool pushFlag;
	const uint8_t ** table;
	uint8_t state;
};

extern RotaryEncoder rotaryEncoder;

#endif /* ROTARYENCODER_H_ */