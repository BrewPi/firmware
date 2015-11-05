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
#include "json_writer.h"
#include "Nameable.h"

class TempSensor: public TempSensorBasic, public Nameable {
public:
    TempSensor() :
            sensor(defaultTempSensorBasic()) {
    }
    TempSensor(TempSensorBasic * s) :
            sensor(s) {
    }
    TempSensor(const char * initialName) :
            sensor(defaultTempSensorBasic()) {
        setName(initialName);
    }
    TempSensor(TempSensorBasic * s, const char * initialName) :
            sensor(s) {
        setName(initialName);
    }

    ~TempSensor() {
    }

    void installSensor(TempSensorBasic * s) {
        uninstallSensor();
        sensor = s;
    }

    TempSensorBasic * getSensor() {
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

    inline bool isConnected(void) {
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
    inline temp_t read() {
        return sensor->read();
    }

    void serialize(JSON::Adapter& adapter) {
        JSON::Class root(adapter, "TempSensor");
        std::string name(getName()); // get name as std string for json_writer
        JSON_E(adapter, name);
        JSON_T(adapter, sensor);
    }

private:
    TempSensorBasic * sensor;
};

