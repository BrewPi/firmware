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

#include "DallasTemperature.h"
#include "OneWireAddress.h"
#include "TempSensor.h"
#include "temperatureFormats.h"

class OneWire;

#define ONEWIRE_TEMP_SENSOR_PRECISION (4)

class TempSensorOneWire final : public TempSensor {
public:
private:
    DallasTemperature sensor;
    OneWireAddress sensorAddress;
    temp_t calibrationOffset;
    temp_t cachedValue;
    bool connected;

public:
    /**
	 * Constructs a new onewire temp sensor.
	 * /param bus	The onewire bus this sensor is on.
	 * /param address	The onewire address for this sensor. If all bytes are 0 in the address, the first temp sensor
	 *    on the bus is used.
	 * /param calibration	A temperature value that is added to all readings. This can be used to calibrate the sensor.	 
	 */
    TempSensorOneWire(OneWire& bus, OneWireAddress _address, const temp_t& _calibrationOffset)
        : sensor(&bus)
        , sensorAddress(_address)
        , calibrationOffset(_calibrationOffset)
        , cachedValue(TEMP_SENSOR_DISCONNECTED)
        , connected(false)

    {
        init();
    }

    TempSensorOneWire(OneWire& bus)
        : TempSensorOneWire(bus, 0, temp_t(0.0))
    {
    }

    ~TempSensorOneWire() = default;

    virtual bool isConnected(void) const override final
    {
        return connected;
    }

    virtual temp_t read() const override final; // return cached value
    void update();                              // read from hardware sensor

    void setAddress(OneWireAddress const& addr)
    {
        sensorAddress = addr;
    }
    void setCalibration(temp_t const& calib)
    {
        calibrationOffset = calib;
    }
    OneWireAddress getAddress() const
    {
        return sensorAddress;
    }
    temp_t getCalibration() const
    {
        return calibrationOffset;
    }

private:
    void init();

    void setConnected(bool connected);

    void requestConversion();

    /**
	 * Reads the temperature. If successful, constrains the temp to the range of the temperature type and
	 * updates lastRequestTime. On successful, leaves lastRequestTime alone and returns DEVICE_DISCONNECTED.
	 */
    temp_t readAndConstrainTemp();
};
