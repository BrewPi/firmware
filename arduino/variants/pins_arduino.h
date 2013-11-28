/*
 * arduino_pins_auto.h
 *
 * Created: 25-11-2013 16:06:32
 *  Author: Elco
 */ 


#ifndef PINS_ARDUINO_AUTO_H_
#define PINS_ARDUINO_AUTO_H_

#if defined(__AVR_ATmega32U4__)
	 // Arduino Leonardo
	 #include "leonardo/pins_arduino.h"	 
#elif defined(__AVR_ATmega328P__)
	// Arduino UNO or older 328P based
	#include "standard/pins_arduino.h"
#elif defined(__AVR_ATmega2560__)
	// Arduino Mega
	#include "mega/pins_arduino.h"
#else
	#error Unknown processor. Cannot decide which Arduino files to include!
#endif


#endif /* PINS_ARDUINO_AUTO_H_ */