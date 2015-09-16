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
#include "Platform.h"
#include "RotaryEncoder.h"

#include "Board.h"
#include <limits.h>
#include "Brewpi.h"

#if BREWPI_STATIC_CONFIG!=BREWPI_SHIELD_DIY
	#if rotarySwitchPin != 7
		#error Review interrupt vectors when not using pin 7 for menu push
	#endif
	#if rotaryAPin != 8
		#error Review interrupt vectors when not using pin 8 for menu right
	#endif
	#if rotaryBPin != 9
		#error Review interrupt vectors when not using pin 9 for menu left
	#endif
#else
	#if rotarySwitchPin != 0
	#error Review interrupt vectors when not using pin 0 for menu push
	#endif
	#if rotaryAPin != 2
	#error Review interrupt vectors when not using pin 2 for menu right
	#endif
	#if rotaryBPin != 1
	#error Review interrupt vectors when not using pin 1 for menu left
	#endif
#endif


#if BREWPI_BOARD!=BREWPI_BOARD_LEONARDO && BREWPI_BOARD!=BREWPI_BOARD_STANDARD
	#error Rotary encoder code is not compatible with boards other than leonardo or uno yet.
#endif


#include "util/atomic.h"
#include "FastDigitalPin.h"


#if BREWPI_STATIC_CONFIG==BREWPI_SHIELD_DIY
ISR(INT2_vect) {
	rotaryEncoder.setPushed();
}
ISR(INT3_vect) {
	uint8_t currPinA = !bitRead(PIND,2);
	uint8_t currPinB = !bitRead(PIND,3);
	rotaryEncoder.process(currPinA, currPinB);
}
ISR(INT1_vect) {
	uint8_t currPinA = !bitRead(PIND,2);
	uint8_t currPinB = !bitRead(PIND,3);
	rotaryEncoder.process(currPinA, currPinB);
}
#elif BREWPI_BOARD == BREWPI_BOARD_LEONARDO
ISR(INT6_vect){
	rotaryEncoder.setPushed();
}
ISR(PCINT0_vect){
	uint8_t currPinA = !bitRead(PINB,4);
	uint8_t currPinB = !bitRead(PINB,5);
	rotaryEncoder.process(currPinA, currPinB);
}
#elif BREWPI_BOARD == BREWPI_BOARD_STANDARD
ISR(PCINT2_vect){
	if(!bitRead(PIND,7)){
		// high to low transition
		rotaryEncoder.setPushed();
	}
}
ISR(PCINT0_vect){
	uint8_t currPinA = !bitRead(PINB,0);
	uint8_t currPinB = !bitRead(PINB,1);
	rotaryEncoder.process(currPinA, currPinB);
}
#else
	#error board/processor not supported by rotary encoder code. Disable or fix the rotary encoder.
#endif

void RotaryEncoder::init(void){
#if BREWPI_ROTARY_ENCODER
	#define BREWPI_INPUT_PULLUP (USE_INTERNAL_PULL_UP_RESISTORS ? INPUT_PULLUP : INPUT)
	fastPinMode(rotaryAPin, BREWPI_INPUT_PULLUP);
	fastPinMode(rotaryBPin, BREWPI_INPUT_PULLUP);
	fastPinMode(rotarySwitchPin, BREWPI_INPUT_PULLUP);
	
	#if BREWPI_STATIC_CONFIG==BREWPI_SHIELD_DIY
		EICRA |= (1<<ISC21) | (1<<ISC10) | (1<<ISC30);; // any logical change for encoder pins, falling edge for switch
		EIMSK |= (1<<INT2) | (1<<INT1) | (1<<INT3); // enable interrupts for each pin	
	#elif BREWPI_BOARD == BREWPI_BOARD_LEONARDO
		// falling edge interrupt for switch on INT6
		EICRB |= (1<<ISC61) | (0<<ISC60);
		// enable interrupt for INT6
		EIMSK |= (1<<INT6);
		// enable pin change interrupts
		PCICR |= (1<<PCIE0);
		// enable pin change interrupt on Arduino pin 8 and 9
		PCMSK0 |= (1<<PCINT5) | (1<<PCINT4);
	#elif BREWPI_BOARD == BREWPI_BOARD_STANDARD
		// enable PCINT0 (PCINT0 and PCINT1 pin) and PCINT2 vector (PCINT23 pin)
		PCICR |= (1<<PCIE2) | (1<<PCIE0);
		// enable mask bits for PCINT0 and PCINT1
		PCMSK0 |= (1<<PCINT0) | (1<<PCINT1);
		// enable mask bit for PCINT23
		PCMSK2 |= (1<<PCINT23);
	#endif
#endif	
}

void RotaryEncoder::setRange(int16_t start, int16_t minVal, int16_t maxVal){
#if BREWPI_ROTARY_ENCODER    
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		// this part cannot be interrupted
		// Multiply by two to convert to half steps
		steps = start;
		minimum = minVal;
		maximum = maxVal; // +1 to make sure that one step is still two half steps at overflow
	}		
#endif        
}

int16_t RotaryEncoder::read(void){
#if BREWPI_ROTARY_ENCODER
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		return steps;		
	}
#endif
	return 0;		
}
