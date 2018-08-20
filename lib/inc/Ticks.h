/*
 * Copyright 2017 BrewPi/Elco Jacobs.
 * Copyright 2017 Matthew McGowan.
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
#include <stdint.h>

using tcduration_t = uint32_t;
using ticks_millis_t = uint32_t;
using ticks_micros_t = uint32_t;
using ticks_seconds_t = uint32_t;

/**
 * Ticks - interface to a millisecond timer
 *
 * With more code space, Ticks would have been a virtual base class, so all implementations can easily provide the same interface.
 * Here, the different implementations have no common (virtual) base class to save code space.
 * Instead, a typedef is used to compile-time select the implementation to use.
 * If that implementation doesn't implement the Ticks interface as expected, it will fail to compile.
 */


// return time that has passed since timeStamp, take overflow into account
ticks_seconds_t timeSinceSeconds(ticks_seconds_t currentTime, ticks_seconds_t previousTime);

// return time that has passed since timeStamp, take overflow into account
ticks_millis_t timeSinceMillis(ticks_millis_t currentTime, ticks_millis_t previousTime);
