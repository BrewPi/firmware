/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
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

#include "Brewpi.h"
#include "TempSensorBasic.h"
#include "ControllerMixins.h"
#include "ControlLib.h"

CONTROL_LIB_BEGIN

/**
 * A temp sensor whose value is not read from the device, but set in code.
 * This is used by the simulator.
 */
class TempSensorExternal : public TempSensorBasic, public TempSensorExternalMixin
{
	public:
	TempSensorExternal(bool connected=false) : value(0.0), connected(false)
	{
		setConnected(connected);
	}

	void setConnected(bool _connected)
	{
		this->connected = _connected;
	}

	bool isConnected() const { return connected; }

	bool init() final {
		return read()!=TEMP_SENSOR_DISCONNECTED;
	}
	
    void update() final {
        // nop for this mock sensor
    }

	temp_t read() const final {
		if (!isConnected())
			return TEMP_SENSOR_DISCONNECTED;
		return value;
	}
	
	void setValue(temp_t newTemp) {
		value = newTemp;		
	}

	protected:
	temp_t value;
	bool connected;

	friend class TempSensorExternalMixin;
};


CONTROL_LIB_END
