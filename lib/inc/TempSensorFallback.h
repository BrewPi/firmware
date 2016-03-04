/*
 * Copyright 2016 BrewPi/Elco Jacobs.
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
#include "ControllerMixins.h"
#include "defaultDevices.h"

/**
 * Class that forwards all calls to another temperature sensor,
 * but when it is unavailable falls back onto a backup sensor
 */
class TempSensorFallback : public TempSensorBasic{
public:
    TempSensorFallback() : main(defaultTempSensorBasic()), backup(defaultTempSensorBasic()), onBackupSensor(false)
    {
    };

    TempSensorFallback(TempSensorBasic * m, TempSensorBasic * b) : main(m), backup(b), onBackupSensor(false)
    {
    };
    virtual ~TempSensorFallback(){};

    /**
     * Check if sensor is connected
     * @return bool: true if active sensor is connected
     */
    inline bool isConnected(void) const final {
        if(onBackupSensor){
            return backup->isConnected();
        }
        else{
            return main->isConnected();
        }
    }

    /**
     * Attempt to (re-)initialize the active sensor.
     *
     * @return bool: true if active sensor was initialized correctly
     */
    inline bool init() final {
        if(onBackupSensor){
            return backup->init();
        }
        else{
            return main->init();
        }
    }

    /**
     * Read the currently used sensor (main or backup)
     * @return temp_t: temperature of sensor
     */
    temp_t read() const final;

    /**
     * update() checks if the main sensor is connected and switches between the main and backup sensor
     */
    void update() final;

private:
    TempSensorBasic * main;
    TempSensorBasic * backup;
    bool onBackupSensor;
};

