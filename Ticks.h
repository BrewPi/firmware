/*
 * Copyright 2012 BrewPi/Elco Jacobs.
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
class HardwareTicks {
public:
	ticks_millis_t millis() { return ::millis(); }
	ticks_micros_t micros() { return ::micros(); }	
	ticks_seconds_t seconds() { return ::millis()/1000; }
};


class HardwareDelay {
public:
	void seconds(uint16_t seconds)	{ delay(seconds<<10); }
	void millis(uint32_t millis)	{ ::delay(millis); }
	
};

class NoOpTicks {
	ticks_millis_t millis() { return 0; }
	ticks_micros_t micros() { return 0; }	
	ticks_seconds_t seconds() { return 0; }	
};

class NoOpDelay {
public:	
	void seconds(uint16_t seconds)	{ delay(seconds<<10); }
	void millis(uint32_t millis)	{ }	
};

typedef HardwareDelay DelayImpl;
typedef HardwareTicks TicksImpl;

extern DelayImpl wait;
extern TicksImpl ticks;


#endif /* TICKS_H_ */
