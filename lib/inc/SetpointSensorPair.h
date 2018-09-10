/*
 * Copyright 2017 BrewPi/Elco Jacobs.
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

#include "ProcessValue.h"
#include "Setpoint.h"
#include "TempSensor.h"
#include <functional>
#include <memory>

/*
 * A process value has a setting and an current value
 */
class SetpointSensorPair : public ProcessValue {

private:
    const std::function<std::shared_ptr<TempSensor>()> sensor;
    const std::function<std::shared_ptr<Setpoint>()> setpoint;

public:
    explicit SetpointSensorPair(
        std::function<std::shared_ptr<TempSensor>()>&& _sensor,
        std::function<std::shared_ptr<Setpoint>()>&& _setpoint)
        : sensor(_sensor)
        , setpoint(_setpoint)
    {
    }

    ~SetpointSensorPair() = default;

    virtual void set(temp_t const& setting) override final
    {
        if (auto sp = setpoint()) {
            sp->write(setting);
        }
    }

    virtual temp_t setting() const override final
    {
        if (auto sp = setpoint()) {
            return sp->read();
        } else {
            return 0;
        }
    }

    virtual temp_t value() const override final
    {
        if (auto sens = sensor()) {
            return sens->read();
        } else {
            return 0;
        }
    }

    virtual bool valid() const override final
    {
        if (auto sens = sensor()) {
            if (auto sp = setpoint()) {
                return sens->valid() && sp->valid();
            }
        }
        return false;
    }
};
