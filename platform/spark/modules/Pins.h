/*
 * Copyright 2015 BrewPi/Elco Jacobs/Matthew McGowan.
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

#include "Brewpi.h"

#define oneWireAddress 0x0

#ifndef actuatorPin1
#define actuatorPin1 A0
#endif

#ifndef actuatorPin2
#define actuatorPin2 A1
#endif

#ifndef actuatorPin3
#define actuatorPin3 A6
#endif

#define alarmPin A2

#define BREWPI_INVERT_ACTUATORS 0

// Spark Core shield has no digital input pins
#ifndef USE_INTERNAL_PULL_UP_RESISTORS
#define USE_INTERNAL_PULL_UP_RESISTORS 0
#endif