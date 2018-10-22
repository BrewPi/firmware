/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of BrewBlox.
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

#include "OneWire.h"
#include "TicksTypes.h"
#include "cbox/Box.h"

#if !defined(SPARK)
#include "cbox/ConnectionsStringStream.h"
cbox::StringStreamConnectionSource&
testConnectionSource();
#endif

// create a static Box object on first use and return a reference to it
cbox::Box&
brewbloxBox();

// create a static OneWire object on first use and return a reference to it
OneWire&
theOneWire();

// have a global variable for the device start time, which is updated with
// the ticks block but referenced by blocks that need the actual time
const ticks_seconds_t&
bootTimeRef();

ticks_seconds_t&
writableBootTimeRef();

void
setBootTime(const ticks_seconds_t& bootTime);

void
updateBrewbloxBox();