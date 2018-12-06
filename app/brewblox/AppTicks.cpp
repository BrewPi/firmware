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

#include "AppTicks.h"

static ticks_seconds_t bootTimeInSeconsSinceEpoch = 0;
TicksClass ticks(bootTimeInSeconsSinceEpoch);

const ticks_seconds_t&
bootTimeRef()
{
    return bootTimeInSeconsSinceEpoch;
}

ticks_seconds_t&
writableBootTimeRef()
{
    return bootTimeInSeconsSinceEpoch;
}

void
setBootTime(const ticks_seconds_t& bootTime)
{
    bootTimeInSeconsSinceEpoch = bootTime;
}