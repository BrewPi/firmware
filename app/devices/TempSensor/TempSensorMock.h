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

class TempSensorMock : public TempSensorBasic
{
public:	
	TempSensorMock(temp_t initial) : _temperature(initial), _connected(true) { }
	
	void setConnected(bool connected)
	{
		_connected = connected;
	}
	
	bool isConnected() { return _connected; }

	bool init() {
		return read().isDisabledOrInvalid();
	}
	
	void add(temp_t delta){
	    _temperature += delta;
	}

	temp_t read()
	{
		if (!isConnected())
			return temp_t::invalid_val;
		return _temperature;
	}
	
	void setTemp(temp_t val){
	    _temperature = val;
	}

	private:
	temp_t _temperature;
	bool _connected;
};

