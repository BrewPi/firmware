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
#include "TempSensor.h"
#include "ControllerMixins.h"
#include "defaultDevices.h"
#include "RefTo.h"

/**
 * Class that forwards all calls to another temperature sensor,
 * but when it is unavailable falls back onto a backup sensor
 */
class TempSensorFallback : public TempSensor, public TempSensorFallbackMixin {
public:
    TempSensorFallback(TempSensor & m, TempSensor & b) :
        main(m),
        backup(b),
        onBackupSensor(false)
    {
    };
    ~TempSensorFallback() = default;

    /**
     * Returns currently active sensor
     * @return TempSensor *: currently active sensor, main or backup
     */
    TempSensor & activeSensor() const {
        return onBackupSensor ? backup : main;
    }

    /**
     * Check if sensor is connected
     * @return bool: true if active sensor is connected
     */
    virtual bool isConnected(void) const override final {
        return activeSensor().isConnected();
    }

    /**
     * Read the currently used sensor (main or backup)
     * @return temp_t: temperature of sensor
     */
    virtual temp_t read() const override final;

    /**
     * update() checks if the main sensor is connected and switches between the main and backup sensor
     */
    virtual update_t update(const update_t & t) override final;

private:
    TempSensor& main;
    TempSensor& backup;
    bool onBackupSensor;

friend class TempSensorFallbackMixin;
};

