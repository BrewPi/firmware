/*
 * Copyright 2012 BrewPi/Elco Jacobs.
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
#include "TempSensor.h"

/**
 * A temp sensor whose value is not read from the device, but set in code.
 * This is used by the simulator.
 */
class ExternalTempSensor : public BasicTempSensor
{
	public:
	ExternalTempSensor(bool connected=false) : _temperature(0), _connected(connected) { }

	void setConnected(bool connected)
	{
		_connected = connected;
	}

	bool isConnected() { return _connected; }

	fixed7_9 init() {
		return read();
	}
	
	fixed7_9 read() {
		if (!isConnected())
			return DEVICE_DISCONNECTED;				
		return _temperature;
	}
	
	void setValue(fixed7_9 newTemp) {
		_temperature = newTemp;		
	}
	
	private:
	fixed7_9 _temperature;
	bool _connected;
};
