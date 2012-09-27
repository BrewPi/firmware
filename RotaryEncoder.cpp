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

#include "RotaryEncoder.h"

#include <Arduino.h>
#include "pins.h"
#include "util/atomic.h"
#include <limits.h>

// declare static member variables:
int RotaryEncoder::maximum=INT_MAX;
int RotaryEncoder::minimum=INT_MIN;
int RotaryEncoder::prevRead=0;
volatile int RotaryEncoder::halfSteps=0;
volatile bool RotaryEncoder::pushFlag=0;
volatile uint8_t RotaryEncoder::int1Signal=0;
volatile uint8_t RotaryEncoder::int3Signal=0;

ISR(INT1_vect){
	#if ROTARY_A != 2
		#error Review interrupt vectors when not using pin 1 for menu right
	#endif
	rotaryEncoder.int1Handler();		
}

ISR(INT3_vect){
	#if ROTARY_B != 1
		#error Review interrupt vectors when not using pin 2 for menu left
	#endif
	rotaryEncoder.int3Handler();
}

ISR(INT2_vect){
	#if ROTARY_SWITCH != 0
	#error Review interrupt vectors when not using pin 0 for menu push
	#endif
	rotaryEncoder.setPushed();
}

void RotaryEncoder::setPushed(void){
	pushFlag = true;
}

void RotaryEncoder::int1Handler(void){
	static uint16_t int1Time = 0;
	static uint8_t int1History = 0;

	if ( micros() - int1Time < ROTARY_THRESHOLD){
		return;
	}
	int1History = int1Signal;
	int1Signal = bitRead(PIND,1);
	if ( int1History==int1Signal ){
		return;
	}
	int1Time = micros();
	if ( int1Signal == int3Signal ){
		halfSteps--;
	}
	else{
		halfSteps++;
	}
	// loop around at edges
	if(halfSteps >= (maximum+2)){
		halfSteps = minimum;
	}
	if(halfSteps <= (minimum-2)){
		halfSteps = maximum;
	}
}

void RotaryEncoder::int3Handler(void){
	static uint16_t int3Time = 0;
	static uint8_t int3History = 0;
	if ( micros() - int3Time < ROTARY_THRESHOLD ){
		return;
	}
	int3History = int3Signal;
	int3Signal = bitRead(PIND,3);
	if ( int3History==int3Signal ){
		return;
	}
	int3Time = micros();
}

void RotaryEncoder::init(void){
	#if(USE_INTERNAL_PULL_UP_RESISTORS)
	pinMode(ROTARY_A, INPUT_PULLUP);
	pinMode(ROTARY_B, INPUT_PULLUP);
	pinMode(ROTARY_SWITCH, INPUT_PULLUP);
	#else
	pinMode(ROTARY_A, INPUT);
	pinMode(ROTARY_B, INPUT);
	pinMode(ROTARY_SWITCH, INPUT);
	#endif
	
	int1Handler(); // call functions ones here for proper initialization
	int3Handler(); 
	
	EICRA |= (1<<ISC21) | (1<<ISC10) | (1<<ISC30);; // any logical change for encoder pins, falling edge for switch
	EIMSK |= (1<<INT2) | (1<<INT1) | (1<<INT3); // enable interrupts for each pin
}


void RotaryEncoder::setRange(int start, int minVal, int maxVal){
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		// this part cannot be interrupted
		// Multiply by two to convert to half steps
		halfSteps = 2*start;
		minimum = 2*minVal;
		maximum = 2*maxVal; // +1 to make sure that one step is still two half steps at overflow
		prevRead = 2*start;
	}		
}

bool RotaryEncoder::changed(void){
	// returns one if the value changed since the last call of changed.
	static int prevValue = 0;
	if(read() != prevValue){
		prevValue = read();
		return 1;
	}
	if(pushFlag == true){
		return 1;
	}
	return 0;
}

int RotaryEncoder::read(void){
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		if((abs(halfSteps) & 0x01) == 1){
			// This is a half step, return previous read
			return prevRead;
		}
		else{
			// This is a full step, update prevRead and return new value
			prevRead = halfSteps >> 1;
			return prevRead;
		}
	}
	return 0;		
}

int RotaryEncoder::readHalfSteps(void){
	return halfSteps;
}

bool RotaryEncoder::pushed(void){
	return pushFlag;
}

void RotaryEncoder::resetPushed(void){
	pushFlag = false;
}