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
int RotaryEncoder::maximum;
int RotaryEncoder::minimum;
int RotaryEncoder::prevRead;
volatile int RotaryEncoder::halfSteps;
volatile bool RotaryEncoder::pushFlag;
volatile uint8_t RotaryEncoder::pinASignal;
volatile uint8_t RotaryEncoder::pinBSignal;
volatile uint8_t RotaryEncoder::pinAHistory;
volatile uint8_t RotaryEncoder::pinBHistory;
volatile unsigned long RotaryEncoder::pinATime;
volatile unsigned long RotaryEncoder::pinBTime;


#if ROTARY_SWITCH != 7
	#error Review interrupt vectors when not using pin 7 for menu push
#endif
#if ROTARY_A != 8
	#error Review interrupt vectors when not using pin 8 for menu right
#endif
#if ROTARY_B != 9
	#error Review interrupt vectors when not using pin 9 for menu left
#endif

#if defined(USBCON)
// Arduino Leonardo
ISR(INT6_vect){
	rotaryEncoder.setPushed();
}

ISR(PCINT0_vect){
	//todo rotaryEncoder.rotationHandler();
	static bool prevPinA = 0;
	static bool prevPinB = 0;
	
	bool currPinA = bitRead(PINB,4);
	bool currPinB = bitRead(PINB,5);
	if(currPinA != prevPinA){
		rotaryEncoder.pinAHandler(currPinA);
	}
	if(currPinB != prevPinB){
		rotaryEncoder.pinBHandler(currPinB);
	}
	prevPinA = currPinA;
	prevPinB = currPinB;
}
#else
// Arduino UNO or older
ISR(PCINT2_vect){
	if(!bitRead(PIND,7)){
		// high to low transition
		rotaryEncoder.setPushed();	
	}
}

ISR(PCINT0_vect){
	//todo rotaryEncoder.rotationHandler();
	static bool prevPinA = 0;
	static bool prevPinB = 0;
			
	bool currPinA = bitRead(PINB,0);
	bool currPinB = bitRead(PINB,1);
	if(currPinA != prevPinA){
		rotaryEncoder.pinAHandler(currPinA);
	}
	if(currPinB != prevPinB){
		rotaryEncoder.pinBHandler(currPinB);
	}
	prevPinA = currPinA;
	prevPinB = currPinB;
}


#endif

void RotaryEncoder::setPushed(void){
	pushFlag = true;
}

void RotaryEncoder::pinAHandler(bool pinState){
	if(micros() - pinATime < ROTARY_THRESHOLD){
		return;
	}		
	pinAHistory = pinASignal;
	pinASignal = pinState;
	if ( pinAHistory==pinASignal ){
		return; // not a transition
	}
	pinATime = micros();
	if ( pinASignal == pinBSignal ){
		halfSteps++;
	}
	else{
		halfSteps--;
	}
	// loop around at edges
	if(halfSteps >= (maximum+2)){
		halfSteps = minimum;
	}
	if(halfSteps <= (minimum-2)){
		halfSteps = maximum;
	}		
}

void RotaryEncoder::pinBHandler(bool pinState){
	if ( micros() - pinBTime < ROTARY_THRESHOLD ){
		return;
	}
	pinBHistory = pinBSignal;
	pinBSignal = pinState;
	if ( pinBHistory==pinBSignal ){
		return; // not a transition
	}
	pinBTime = micros();
}

void RotaryEncoder::init(void){
	maximum = INT_MAX;
	minimum = INT_MIN;
	prevRead = 0;
	halfSteps = 0;
	pushFlag = 0;
	pinASignal = 1;
	pinBSignal = 1;
	pinAHistory = 1;
	pinBHistory = 1;
	pinATime = 0;
	pinBTime = 0;
	
	#if(USE_INTERNAL_PULL_UP_RESISTORS)
	pinMode(ROTARY_A, INPUT_PULLUP);
	pinMode(ROTARY_B, INPUT_PULLUP);
	pinMode(ROTARY_SWITCH, INPUT_PULLUP);
	#else
	pinMode(ROTARY_A, INPUT);
	pinMode(ROTARY_B, INPUT);
	pinMode(ROTARY_SWITCH, INPUT);
	#endif
	
	pinAHandler(true); // call functions ones here for proper initialization
	pinBHandler(true); 
	
	#if defined(USBCON) // Arduino Leonardo
		// falling edge interrupt for switch on INT6
		EICRB |= (1<<ISC61) | (0<<ISC60);
		// enable interrupt for INT6
		EIMSK |= (1<<INT6);
		// enable pin change interrupts
		PCICR |= (1<<PCIE0);
		// enable pin change interrupt on Arduino pin 8 and 9
		PCMSK0 |= (1<<PCINT5) | (1<<PCINT4);
	#else // Arduino UNO
		// enable PCINT0 (PCINT0 and PCINT1 pin) and PCINT2 vector (PCINT23 pin)
		PCICR |= (1<<PCIE2) | (1<<PCIE0);
		// enable mask bits for PCINT0 and PCINT1
		PCMSK0 |= (1<<PCINT0) | (1<<PCINT1);
		// enable mask bit for PCINT23
		PCMSK2 |= (1<<PCINT23);
	#endif
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