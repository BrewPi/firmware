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

#include "temperatureFormats.h"
#include "OneWireTempSensor.h"
#include "OneWireAddress.h"
#include "DallasTemperature.h"
#include "OneWire.h"
#include "Logger.h"

/**
 * Initializes the temperature sensor.
 * This method is called when the sensor is first created and also any time the sensor reports it's disconnected.
 * If the result is TEMP_SENSOR_DISCONNECTED then subsequent calls to read() will also return TEMP_SENSOR_DISCONNECTED.
 * Clients should attempt to re-initialize the sensor by calling init() again. 
 */
void OneWireTempSensor::init() {
    bool success = false;

    // This quickly tests if the sensor is connected and initializes the reset detection if necessary.

    // If this is the first conversion after power on, the device will return DEVICE_DISCONNECTED_RAW
    // Because HIGH_ALARM_TEMP will be copied from EEPROM
    int16_t temp = sensor.getTempRaw(sensorAddress.asUint8ptr());
    if (temp == DEVICE_DISCONNECTED_RAW) {
        // Device was just powered on and should be initialized
        if (sensor.initConnection(sensorAddress.asUint8ptr())) {
            requestConversion();
        }
    }
}

void OneWireTempSensor::requestConversion() {
    sensor.requestTemperaturesByAddress(sensorAddress.asUint8ptr());
}

void OneWireTempSensor::setConnected(bool _connected) {
    if (connected == _connected){
        return; // state stays the same
    }
    if (connected) {
        logInfoAddress(INFO_TEMP_SENSOR_CONNECTED, sensorAddress);
    } else {
        logWarningAddress(WARNING_TEMP_SENSOR_DISCONNECTED, sensorAddress);
    }
}

temp_t OneWireTempSensor::read() const {
    if (!connected){
        return TEMP_SENSOR_DISCONNECTED;
    }

    return cachedValue;
}

update_t OneWireTempSensor::update(const update_t & now){
    cachedValue = readAndConstrainTemp();
    requestConversion();
    return now + 1000;
}

temp_t OneWireTempSensor::readAndConstrainTemp() {
    int16_t tempRaw;
    bool success;

    tempRaw = sensor.getTempRaw(sensorAddress.asUint8ptr());
    success = tempRaw != DEVICE_DISCONNECTED_RAW;

    if (!success) {
        // retry re-init
        init();
    }

    setConnected(success);

    if (!success) {
        return temp_t::invalid();
    }

    // difference in precision between DS18B20 format and temperature format
    const uint8_t shift = temp_t::fractional_bit_count - ONEWIRE_TEMP_SENSOR_PRECISION;
    temp_t temp;
    temp.setRaw(tempRaw << shift);
    return temp + calibrationOffset;
}
