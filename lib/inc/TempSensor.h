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
#include "TempSensorInterface.h"
#include "defaultDevices.h"
#include "ControllerMixins.h"

class TempSensor final : public TempSensorInterface, public TempSensorMixin {
public:
    TempSensor() :
            sensor(defaultTempSensor()) {
    }
    TempSensor(TempSensorInterface * s) :
            sensor(s) {
    }

    ~TempSensor() {
    }

    void installSensor(TempSensorInterface * s) {
        uninstallSensor();
        sensor = s;
    }

    TempSensorInterface * getSensor() {
        return sensor;
    }

    bool uninstallSensor(){
        if(sensor == defaultTempSensor()){
            return false;
        }
        else{
            delete sensor;
            sensor = defaultTempSensor();
            return true;
        }
    }

    inline bool isConnected(void) const override final {
        return sensor->isConnected();
    }

    /*
     * Attempt to (re-)initialize the sensor.
     */
    inline bool init() override final {
        return sensor->init();
    }

    /*
     * Update the sensor if the value is cached
     */
    void update() override final {
        sensor->update();
    }

    /*
     * Fetch a new reading from the sensor
     */
    inline temp_t read() const override final {
        return sensor->read();
    }

private:
    TempSensorInterface * sensor;
    friend class TempSensorMixin;
};

