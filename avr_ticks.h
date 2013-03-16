/*
 * avr_ticks.h
 *
 * Created: 15/03/2013 09:31:54
 *  Author: mat
 */ 


#ifndef AVR_TICKS_H_
#define AVR_TICKS_H_

#include "Arduino.h"

/*
 * The Ticks class provides the time period since the device was powered up.
 */
class HardwareTicks {
public:
	ticks_millis_t millis() { return ::millis(); }
	ticks_micros_t micros() { return ::micros(); }	
	ticks_seconds_t seconds() { return ::millis()/1000; }
};


class HardwareDelay {
public:
	HardwareDelay() {}
	void seconds(uint16_t seconds)	{ millis(seconds<<10); }
	void millis(uint32_t millis)	{ ::delay(millis); }
	void microseconds(uint32_t micros) { ::delayMicroseconds(micros); }	
};




#endif /* AVR_TICKS_H_ */