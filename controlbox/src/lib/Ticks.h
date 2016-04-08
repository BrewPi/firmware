/*
 * Copyright 2013 Matthew McGowan.
 *
 * This file is part of Controlbox.
 * 
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Controlbox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>

typedef uint32_t tcduration_t;
typedef uint32_t ticks_millis_t;
typedef uint32_t ticks_micros_t;
typedef uint32_t ticks_seconds_t;
typedef uint8_t ticks_seconds_tiny_t;



#if CONTROLBOX_STATIC
#include "TicksImpl.h"

extern Ticks baseticks;

#else


class Ticks
{
public:
	virtual ticks_millis_t millis()=0;
};

class Delay
{
public:
	virtual void wait(ticks_millis_t delay)=0;
};


#endif
