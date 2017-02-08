/*
 * Copyright 2015 Matthew McGowan
 * Copyright 2015 BrewPi/Elco Jacobs.
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
#include "temperatureFormats.h"
#include "Interface.h"

/*
 * An actuator can be driven by other classes and acts on something.
 * Actuators can also drive other actuators, getDeviceTarget finds the lowest level actuator recursively
 */
class ActuatorInterface: public virtual Interface
    , public virtual ActuatorInterfaceMixin
{
public:
    ActuatorInterface() = default;
    virtual ~ActuatorInterface() = default;

    friend class ActuatorInterfaceMixin;
};


/*
 * An ActuatorDigital simply turns something on or off.
 */
class ActuatorDigitalInterface : public virtual ActuatorInterface
    , public virtual ActuatorDigitalInterfaceMixin
{
public:
    ActuatorDigitalInterface() = default;
    virtual ~ActuatorDigitalInterface() = default;
    virtual void setActive(bool active, int8_t priority = 127) = 0;
    virtual bool isActive() const = 0;

    friend class ActuatorDigitalMixin;
};


/*
 * An ActuatorRange has a range output between min and max
 */
class ActuatorRangeInterface : public virtual ActuatorInterface
    , public virtual ActuatorRangeInterfaceMixin
{
public:
    ActuatorRangeInterface() = default;
    virtual ~ActuatorRangeInterface() = default;
    virtual void setValue(temp_t const& val) = 0;
    virtual temp_t getValue() const = 0; // get set value
    virtual temp_t readValue() const = 0; // read actual achieved value
    virtual temp_t min() const = 0;
    virtual temp_t max() const = 0;
};

/*
 * An ThresholdActuator has switches on at a certain threshold. TODO: add hysteresis
 */
class ActuatorThresholdInterface : public virtual ActuatorInterface
    , public virtual ActuatorThresholdInterfaceMixin
{
ActuatorThresholdInterface() = default;
    virtual ~ActuatorThresholdInterface() = default;
    virtual void setValue(temp_t const& val) = 0;
    virtual temp_t readValue() const = 0;
    virtual temp_t onValue() const = 0;
    virtual temp_t offValue() const = 0;
};


