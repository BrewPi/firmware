/*
 * Copyright 2013 BrewPi/Elco Jacobs.
 * Copyright 2013 Matthew McGowan.
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

#pragma once

#include "Platform.h"
#include "TicksImpl.h"
#include <stdint.h>


/**
 * Ticks - interface to a millisecond timer
 *
 * With more code space, Ticks would have been a virtual base class, so all implementations can easily provide the same interface.
 * Here, the different implementations have no common (virtual) base class to save code space. 
 * Instead, a typedef is used to compile-time select the implementation to use.
 * If that implementation doesn't implement the Ticks interface as expected, it will fail to compile.
 */

/**
 * A Ticks implementation that increments the millis count each time it is called.
 * This is used for testing.
 */
class MockTicks {
public:
	MockTicks(uint8_t increment) : _increment(increment), _ticks(0) { }

	ticks_millis_t millis() { return _ticks+=_increment; }
	ticks_micros_t micros() { return _ticks+=_increment; }	
	ticks_seconds_t seconds() { return millis()>>10; }	
	ticks_seconds_t timeSince(ticks_seconds_t timeStamp) { return timeStamp-seconds(); }
private:

	uint32_t _increment;
	uint32_t _ticks;	
};

/**
 * Externally provided millis timer. The calling code takes care of advancing the timer by calling setMillis or incMillis.
 * This is used for testing and also by the simulator to provide simulated time.
 */
class ExternalTicks {
	public:
	ExternalTicks() : _ticks(0) { }

	ticks_millis_t millis() { return _ticks; }
	ticks_micros_t micros() { return _ticks*1000; }	
	ticks_seconds_t seconds() { return millis()/1000; }	
	ticks_seconds_t timeSince(ticks_seconds_t timeStamp);
			
	void setMillis(ticks_millis_t now)	{ _ticks = now; }
	void incMillis(ticks_millis_t advance)	{ _ticks += advance; }
private:
	ticks_millis_t _ticks;			
};


/**
 * A delay class that does nothing.
 * In the AVR simulator, delays using millis() take a very long time. Using this class makes it possible to step through the code.
 */
class NoOpDelay {
public:	
	void seconds(uint16_t seconds)	{ millis(seconds<<10); }
	void millis(uint32_t millis)	{ }	
	void microseconds(uint32_t micros) { }
};

// return time that has passed since timeStamp, take overflow into account
inline ticks_seconds_t timeSince(ticks_seconds_t currentTime, ticks_seconds_t previousTime) {
	if(currentTime>=previousTime){
		return currentTime - previousTime;
	}
	else{
		// overflow has occurred
		return (currentTime + 1440) - (previousTime +1440); // add a day to both for calculation
	}
}
