/*
 * TicksHardware.cpp
 *
 * Created: 07/01/2015 05:01:40
 *  Author: mat
 */ 

#include "Ticks.h"

#include "TicksWiring.h"


ticks_seconds_t HardwareTicks::seconds() { return ::millis()/1000; }


void HardwareDelay::millis(uint16_t millis) { ::delay(millis); }

void HardwareDelay::seconds(uint16_t seconds)	{ millis(seconds<<10); }

