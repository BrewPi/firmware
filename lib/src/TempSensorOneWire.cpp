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

#include "../inc/Logger.h"

#include "../inc/DallasTemperature.h"
#include "../inc/OneWire.h"
#include "../inc/OneWireAddress.h"
#include "../inc/TempSensorOneWire.h"
#include "../inc/Temperature.h"

/**
 * Initializes the temperature sensor.
 * This method is called when the sensor is first created and also any time the sensor reports it's disconnected.
 * If the result is TEMP_SENSOR_DISCONNECTED then subsequent calls to read() will also return TEMP_SENSOR_DISCONNECTED.
 * Clients should attempt to re-initialize the sensor by calling init() again. 
 */
void
TempSensorOneWire::init()
{
    // This quickly tests if the sensor is connected and initializes the reset detection if necessary.

    // If this is the first conversion after power on, the device will return DEVICE_DISCONNECTED_RAW
    // Because HIGH_ALARM_TEMP will be copied from EEPROM
    int16_t temp = m_sensor.getTempRaw(getAddress().asUint8ptr());
    if (temp == DEVICE_DISCONNECTED_RAW) {
        // Device was just powered on and should be initialized
        if (m_sensor.initConnection(getAddress().asUint8ptr())) {
            requestConversion();
        }
    }
}

void
TempSensorOneWire::requestConversion()
{
    m_sensor.requestTemperaturesByAddress(getAddress().asUint8ptr());
}

void
TempSensorOneWire::connected(bool _connected)
{
    if (m_connected == _connected) {
        return; // state stays the same
    }
    m_connected = _connected;
    if (m_connected) {
        // CL_LOG_WARN("OneWire temp sensor connected: ") << getAddress().toString();
    } else {
        // CL_LOG_WARN("OneWire temp sensor disconnected: ") << getAddress().toString();
    }
}

temp_t
TempSensorOneWire::value() const
{
    if (!m_connected) {
        return 0;
    }

    return m_cachedValue;
}

void
TempSensorOneWire::update()
{
    m_cachedValue = readAndConstrainTemp();
    requestConversion();
}

temp_t
TempSensorOneWire::readAndConstrainTemp()
{
    int16_t tempRaw;
    bool success;

    tempRaw = m_sensor.getTempRaw(getAddress().asUint8ptr());
    success = tempRaw != DEVICE_DISCONNECTED_RAW;

    if (!success) {
        // retry re-init
        init();
    }

    connected(success);

    if (!success) {
        return 0;
    }

    // difference in precision between DS18B20 format and temperature format
    constexpr auto shift = (-temp_t::exponent) - ONEWIRE_TEMP_SENSOR_PRECISION;
    temp_t temp = cnl::wrap<temp_t>(tempRaw << shift);
    return temp + m_calibrationOffset;
}
