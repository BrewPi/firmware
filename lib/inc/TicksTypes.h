/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of the BrewBlox Control Library.
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

#include <cstdint>

using duration_millis_t = uint32_t;
using duration_micros_t = uint32_t;
using duration_seconds_t = uint32_t;
using ticks_millis_t = uint32_t;
using ticks_micros_t = uint32_t;
using ticks_seconds_t = uint32_t;

// return time that has passed since timeStamp, take overflow into account
duration_seconds_t
secondsSince(ticks_seconds_t currentTime, ticks_seconds_t previousTime);

// return time that has passed since timeStamp, take overflow into account
duration_millis_t
millisSince(ticks_millis_t currentTime, ticks_millis_t previousTime);