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
#include "TempSensorBasic.h"
#include "defaultDevices.h"
#include "ControllerMixins.h"
#include "ControlLib.h"

CONTROL_LIB_BEGIN


class TempSensor: public TempSensorBasic, public TempSensorMixin {
public:
    TempSensor() :
            sensor(defaultTempSensorBasic()) {
    }
    TempSensor(TempSensorBasic * s) :
            sensor(s) {
    }

    ~TempSensor() {
    }

    void installSensor(TempSensorBasic * s) {
        uninstallSensor();
        sensor = s;
    }

    TempSensorBasic * getSensor() const {
        return sensor;
    }

    bool uninstallSensor(){
        if(sensor == defaultTempSensorBasic()){
            return false;
        }
        else{
            delete sensor;
            sensor = defaultTempSensorBasic();
            return true;
        }
    }

    inline bool isConnected(void) const {
        return sensor->isConnected();
    }

    /*
     * Attempt to (re-)initialize the sensor.
     */
    inline bool init() {
        return sensor->init();
    }

    /*
     * Update the sensor if the value is cached
     */
    void update() {
        sensor->update();
    }

    /*
     * Fetch a new reading from the sensor
     */
    inline temp_t read() const {
        return sensor->read();
    }

protected:
    TempSensorBasic * sensor;
};

CONTROL_LIB_END
