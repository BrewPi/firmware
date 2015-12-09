/*
 * TicksHardware.cpp
 *
 * Created: 07/01/2015 05:01:40
 *  Author: mat
 */ 

#include "Ticks.h"

#include "TicksWiring.h"



// return time that has passed since timeStamp, take overflow into account
ticks_seconds_t HardwareTicks::timeSinceSeconds(ticks_seconds_t previousTime){
	ticks_seconds_t currentTime = TICKS.seconds();
	return ::timeSinceSeconds(currentTime, previousTime);
}

// return time that has passed since timeStamp, take overflow into account
ticks_millis_t HardwareTicks::timeSinceMillis(ticks_millis_t previousTime){
    ticks_millis_t currentTime = TICKS.millis();
    return ::timeSinceMillis(currentTime, previousTime);
}

ticks_seconds_t HardwareTicks::seconds() { return ::millis()/1000; }


void HardwareDelay::millis(uint16_t millis) { ::delay(millis); }

void HardwareDelay::seconds(uint16_t seconds)	{ millis(seconds<<10); }

