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

#include <stdint.h>
#include "ControllerMixins.h"
#include "Interface.h"
#include "TempSensor.h"
#include "SetPoint.h"
#include "ProcessValue.h"
#include "VisitorBase.h"

/*
 * A process value has a setting and an current value
 */
class SensorSetPointPair :
    public virtual ProcessValue,
    public SensorSetPointPairMixin
{
public:
    SensorSetPointPair(TempSensor & _sensor, SetPoint & _setpoint) :
        sensor(_sensor),
        setPoint(_setpoint)
    {}

    ~SensorSetPointPair() = default;

    /**
     * Accept function for visitor pattern
     * @param dispatcher Visitor to process this class
     */
    virtual void accept(VisitorBase & v) override final {
        v.visit(*this);
    }

    virtual void update() override final {};
    virtual void fastUpdate() override final {};

    virtual void set(temp_t const& setting) override final {
        setPoint.write(setting);
    }

    virtual temp_t setting() const override final {
        return setPoint.read();
    }

    virtual temp_t value() const override final {
        return sensor.read();
    }
    

private:
    TempSensor & sensor;
    SetPoint & setPoint;
    friend class SensorSetPointPairMixin;
};
