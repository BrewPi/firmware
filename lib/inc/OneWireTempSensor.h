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

#include "TempSensor.h"
#include "OneWireAddress.h"
#include "DallasTemperature.h"
#include "Ticks.h"

class DallasTemperature;
class OneWire;

#define ONEWIRE_TEMP_SENSOR_PRECISION (4)

class OneWireTempSensor final : public TempSensor, public OneWireTempSensorMixin {
public:
	struct Settings {
		DeviceAddress sensorAddress;
		temp_t calibrationOffset;
	};

	struct State {
		temp_t cachedValue;
		bool connected;
	};

private:
	DallasTemperature sensor;
	Settings settings;
	State state;

public:	
	/**
	 * Constructs a new onewire temp sensor.
	 * /param bus	The onewire bus this sensor is on.
	 * /param address	The onewire address for this sensor. If all bytes are 0 in the address, the first temp sensor
	 *    on the bus is used.
	 * /param calibration	A temperature value that is added to all readings. This can be used to calibrate the sensor.	 
	 */
	OneWireTempSensor(OneWire* bus, DeviceAddress address, temp_t calibrationOffset)
	: sensor(bus)
    {
        memcpy(settings.sensorAddress, address, sizeof(DeviceAddress));
        settings.calibrationOffset = calibrationOffset;
        state.connected = false;
        state.cachedValue = TEMP_SENSOR_DISCONNECTED;
        init();
    };
	
	OneWireTempSensor(OneWire* bus)
    : sensor(bus) {
        memset(settings.sensorAddress, 0, sizeof(DeviceAddress));
        settings.calibrationOffset = temp_t(0.0);
        state.connected = false;
        state.cachedValue = TEMP_SENSOR_DISCONNECTED;
        init();
    };

	~OneWireTempSensor() = default;

    /**
     * Accept function for visitor pattern
     * @param dispatcher Visitor to process this class
     */
    virtual void accept(VisitorBase & v) override final {
    	v.visit(*this);
    }

	virtual bool isConnected(void) const override final {
		return state.connected;
	}		
	
	virtual bool init() override final ;
	virtual temp_t read() const override final ; // return cached value
	virtual void update() override final ; // read from hardware sensor
	

    void setSettings(Settings const & from){
        settings = from;
    }

    Settings const& getSettings(){
        return settings;
    }

    State const& getState(){
        return state;
    }


private:

	void setConnected(bool connected);
	void requestConversion();
	void waitForConversion()
	{
		wait.millis(750);
	}
	
	/**
	 * Reads the temperature. If successful, constrains the temp to the range of the temperature type and
	 * updates lastRequestTime. On successful, leaves lastRequestTime alone and returns DEVICE_DISCONNECTED.
	 */
	temp_t readAndConstrainTemp();

public:
	friend class OneWireTempSensorMixin;
};
