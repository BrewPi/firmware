/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
 * Copyright 2013 Matthew McGowan 
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
#include "DallasTemperature.h"
#include "Ticks.h"
#include "OneWireBus.h"


class DallasTemperature;
class OneWire;

#define ONEWIRE_TEMP_SENSOR_PRECISION (4)

class OneWireTempSensor : public BasicTempSensor {
public:	

	/**
	 * Creates a one-wire 
	 */
	static Object* create(ObjectDefinition& def) {
		// todo - move length requirements externally?
		Object* result = NULL;
		if (def.len>=10) {		// 8 address + 1 calibration + 1+ for id of bus
			DeviceAddress address;			
			def.in->read(address, 8);
			fixed4_4 calibration = def.in->next();
			OneWireBus* bus = cast_object_ptr(OneWireBus, lookupUserObject(*def.in));
			if (bus)
				result = new OneWireTempSensor(bus->onewire(), address, calibration);
		}
		return result;
	}

	prepare_t prepare() {
		if (!isConnected())
			init();
		
		return 750;			// conversion time
	}

	/**
	 * Constructs a new onewire temp sensor.
	 * /param bus	The onewire bus this sensor is on.
	 * /param address	The onewire address for this sensor. If all bytes are 0 in the address, the first temp sensor
	 *    on the bus is used.
	 * /param calibration	A temperature value that is added to all readings. This can be used to calibrate the sensor.	 
	 */
	OneWireTempSensor(OneWire* bus, DeviceAddress address, fixed4_4 calibrationOffset)
	: sensor(bus) {		
		connected = true;  // assume connected. Transition from connected to disconnected prints a message.
		memcpy(sensorAddress, address, sizeof(DeviceAddress));
		this->calibrationOffset = calibrationOffset;
	};
		
	bool isConnected(void){
		return connected;
	}		
	
	bool init();
	temperature read();
	
	private:

	void setConnected(bool connected);
	bool requestConversion();
	void waitForConversion()
	{
		wait.millis(750);
	}

	
	/**
	 * Reads the temperature. If successful, constrains the temp to the range of the temperature type and
	 * updates lastRequestTime. On successful, leaves lastRequestTime alone and returns DEVICE_DISCONNECTED.
	 */
	temperature readAndConstrainTemp();
	
	DallasTemperature sensor;
	// todo - consider moving this into eeprom
	DeviceAddress sensorAddress;

	fixed4_4 calibrationOffset;		
	bool connected;
	
};
