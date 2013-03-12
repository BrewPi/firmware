/*
 * Ticks.h
 *
 * Created: 12/03/2013 05:16:50
 *  Author: mat
 */ 


#ifndef TICKS_H_
#define TICKS_H_

#include "Arduino.h"

typedef unsigned long ticks_millis_t;
typedef unsigned long ticks_micros_t;
typedef unsigned int ticks_seconds_t;
typedef unsigned char ticks_seconds_tiny_t;

/*
 * The Ticks class provides the time period since the device was powered up.
 */
class Ticks {
public:
	ticks_millis_t millis() { return ::millis(); }
	ticks_micros_t micros() { return ::micros(); }	
	ticks_seconds_t seconds() { return ::millis()/1000; }
};


class Delay {
public:
	static void seconds(uint16_t seconds)	{ delay(seconds<<10); }
	static void millis(uint32_t millis)	{ ::delay(millis); }
	
};

extern Delay wait;
extern Ticks ticks;


#endif /* TICKS_H_ */
