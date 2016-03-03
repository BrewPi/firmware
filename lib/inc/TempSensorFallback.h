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
    TempSensorFallback() : main(defaultTempSensorBasic()), backup(defaultTempSensorBasic())
    {
    };

    TempSensorFallback(TempSensorBasic * m, TempSensorBasic * b) : main(m), backup(b)
    {
    };
    virtual ~TempSensorFallback(){};

    /**
     * Check if sensor is connected
     * @return bool: true if main or backup sensor is connected
     */
    inline bool isConnected(void) const final {
        return main->isConnected() || backup->isConnected();
    }

    /**
     * Attempt to (re-)initialize the sensor.
     * If it does not initialize correctly, use the backup sensor
     *
     * @return bool: true if either sensor or backup sensor was initialized correctly
     */
    inline bool init() final {
        return main->init() || backup->init();
    }

    /**
     * Read the main sensor, but if it is unavailable/invalid, read the backup sensor instead
     * @return temp_t: temperature of main sensor if available, otherwise temperature of backups sensor
     */
    inline temp_t read() const final {
        temp_t val;
        val = main->read();
        if(val.isDisabledOrInvalid()){
            val = backup->read();
        }
        return val;
    }

    /**
     * Update function is a no-op for the fallback sensor.
     * The main and backup sensor are updated elsewhere.
     * Updating them here too would create more updates than needed
     */
    inline void update() final {
        // update does nothing
    }

private:
    TempSensorBasic * main;
    TempSensorBasic * backup;
};

