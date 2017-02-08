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
#include "Interface.h"
#include "ControllerMixins.h"


#define TEMP_SENSOR_DISCONNECTED temp_t::invalid()

class TempSensor : public Interface, virtual public TempSensorMixin
{
public:
    TempSensor() = default;
	virtual ~TempSensor() = default;
	
	virtual bool isConnected(void) const = 0;
	
	/*
	 * Attempt to (re-)initialize the sensor. 	 
	 */
	virtual bool init() =0;

    void fastUpdate() final {}; // fast update not needed for temp sensors

	/*
	 * Read the sensor, returns cached value set in update()
	 */
	virtual temp_t read() const = 0;
};


