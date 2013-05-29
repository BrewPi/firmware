/*
 * Copyright 2013 Matthew McGowan
 *
 * TicksArduino.h
 *
 * Created: 15/03/2013 09:31:54
 *  Author: mat
 */ 


#ifndef AVR_TICKS_H_
#define AVR_TICKS_H_

#include "Brewpi.h"

/*
 * The Ticks class provides the time period since the device was powered up.
 */
class HardwareTicks {
public:
	ticks_millis_t millis() { return ::millis(); }
	ticks_micros_t micros() { return ::micros(); }	
	ticks_seconds_t seconds();
		
	ticks_seconds_t timeSince(ticks_seconds_t timeStamp);
};


class HardwareDelay {
public:
	HardwareDelay() {}
	void seconds(uint16_t seconds);
	void millis(uint16_t millis);
	void microseconds(uint32_t micros) { ::delayMicroseconds(micros); }	
};




#endif /* AVR_TICKS_H_ */