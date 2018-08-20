/*
 * TicksHardware.cpp
 *
 * Created: 07/01/2015 05:01:40
 *  Author: mat
 */ 

#include "TicksWiring.h"
#include "Ticks.h"


ticks_seconds_t HardwareTicks::seconds() { return ::millis()/1000; }
ticks_millis_t HardwareTicks::millis() { return ::millis(); }
ticks_micros_t HardwareTicks::micros() { return ::micros(); }


void HardwareDelay::millis(uint16_t millis) { ::delay(millis); }
void HardwareDelay::microseconds(uint32_t micros) { ::delayMicroseconds(micros); }
void HardwareDelay::seconds(uint16_t seconds) { millis(seconds*1000); }

