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

#include "temperatureFormats.h"
#include "json_writer.h"

#define TEMP_SENSOR_DISCONNECTED temp_t::invalid()

class TempSensorBasic
{
public:
	virtual ~TempSensorBasic() { }
	
	virtual bool isConnected(void) = 0;
	
	/*
	 * Attempt to (re-)initialize the sensor. 	 
	 */
	virtual bool init() =0;

	/*
	 * Fetch a new reading from the sensor
	 */
	virtual temp_t read() = 0;
	
	virtual void serialize(JSON::Adapter& adapter) = 0;
};


