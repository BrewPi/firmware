/*
 * arduino_pins_auto.h
 *
 * Created: 25-11-2013 16:06:32
 *  Author: Elco
 */ 


#ifndef PINS_ARDUINO_AUTO_H_
#define PINS_ARDUINO_AUTO_H_

#ifdef _AVR_IOM32U4_H_
	 // Arduino Leonardo
	 #include "leonardo/pins_arduino.h"	 
#endif

#ifdef __AVR_ATmega328P__
	// Arduino UNO or older 328P based
	#include "standard/pins_arduino.h"
#endif

#ifdef __AVR_ATmega2560__
	// Arduino Mega
	#include "mega/pins_arduino.h"
#endif


#endif /* PINS_ARDUINO_AUTO_H_ */