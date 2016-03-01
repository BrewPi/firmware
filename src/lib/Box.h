/*
 * Copyright 2014-2015 Matthew McGowan.
 *
 * This file is part of Nice Firmware.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Ticks.h"
#include "EepromAccess.h"
#include "Comms.h"
#include "Commands.h"
#include "SystemProfile.h"


/**
 * Top-level object for running a controlbox
 */
class Box
{
	EepromAccess& eepromAccess_;
	Ticks& ticks_;
	Comms& comms_;
	SystemProfile systemProfile_;
	Commands commands_;

public:
	Box(EepromAccess& eepromAccess, Ticks& ticks, Comms& comms, CommandCallbacks& callbacks, Object** values, size_t size)
	: eepromAccess_(eepromAccess), ticks_(ticks), comms_(comms),
	  systemProfile_(eepromAccess, size, values), commands_(comms, systemProfile_, callbacks, eepromAccess)
	{

	}
};

class AllCallbacks : public CommandCallbacks, public Ticks, public EepromAccess, public Comms
{
};

/**
 * Convenience class that provides virtual methods for all pluggable functions.
 */
class AllInOneBox : public Box
{
public:
	AllInOneBox(AllCallbacks& cb, Object** values=nullptr, size_t size=0)
		: Box(cb, cb, cb, cb, values, size)
	{}


};

