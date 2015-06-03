/*
 * TicksHardware.cpp
 *
 * Created: 07/01/2015 05:01:40
 *  Author: mat
 */ 

#include "Ticks.h"

#include "TicksWiring.h"



// return time that has passed since timeStamp, take overflow into account
ticks_seconds_t HardwareTicks::timeSince(ticks_seconds_t previousTime){
	ticks_seconds_t currentTime = TICKS.seconds();
	return ::timeSince(currentTime, previousTime);
}

ticks_seconds_t HardwareTicks::seconds() { return ::millis()/1000; }


void HardwareDelay::millis(uint16_t millis) { ::delay(millis); }

void HardwareDelay::seconds(uint16_t seconds)	{ millis(seconds<<10); }

