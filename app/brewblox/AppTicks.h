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

#include "blox/TicksBlock.h"

#if defined(SPARK)
#if PLATFORM_ID != 3 || defined(STDIN_SERIAL)
#include "cbox/spark/ConnectionsSerial.h"
#endif
#include "cbox/spark/ConnectionsTcp.h"
#include "wiring/TicksWiring.h"
using TicksClass = Ticks<TicksWiring>;
#else
#include "cbox/ConnectionsStringStream.h"
#include <MockTicks.h>
using TicksClass = Ticks<MockTicks>;
#endif

#if !defined(PLATFORM_ID) || PLATFORM_ID == 3
#include "OneWireNull.h"
#include "test/MockOneWireScanningFactory.h"
using OneWireDriver = OneWireNull;
#define ONEWIRE_ARG
#else
#include "DS248x.h"
#include "OneWireScanningFactory.h"
using OneWireDriver = DS248x;
#define ONEWIRE_ARG 0x00
#endif

const ticks_seconds_t&
bootTimeRef();

ticks_seconds_t&
writableBootTimeRef();

void
setBootTime(const ticks_seconds_t& bootTime);

extern TicksClass ticks;