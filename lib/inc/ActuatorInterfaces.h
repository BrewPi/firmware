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
#include "temperatureFormats.h"
#include "json_adapter.h"
#include "ControllerMixins.h"
#include "ControlLib.h"

CONTROL_LIB_BEGIN

enum {
    ACTUATOR_RANGE,
    ACTUATOR_TOGGLE,
    ACTUATOR_THRESHOLD,
    ACTUATOR_TOGGLE_MUTEX
};


class ActuatorDigital;
/*
 * An actuator can be driven by other classes and acts on something.
 * Actuators can also drive other actuators, getDeviceTarget finds the lowest level actuator recursively
 */
class Actuator: public ActuatorMixin
{
public:
    Actuator() = default;
    virtual ~Actuator() = default;

    virtual uint8_t type() const = 0;
    virtual void update() = 0;

	// next 3 functions are implemented by ActuatorDriver or ActuatorBottom

	// recursive call for composite driver classes, until a non-driver class is reached
	virtual Actuator * getBareActuator() = 0;
	// install pin/mock actuator at the lowest level, returns Actuator that was installed
	    // Returns true if a device was uninstalled, so the driver knows to update its own pointer
	virtual bool installActuatorFinalTarget(ActuatorDigital * a) = 0;
	// uninstall pi n/mock actuator at the lowest level, return success (true = an actuator was uninstalled)
	virtual bool uninstallActuatorFinalTarget() = 0;

	friend class ActuatorMixin;
};


/*
 * An ActuatorDigital simply turns something on or off.
 */
class ActuatorDigital : public virtual Actuator, public ActuatorDigitalMixin
{
public:
    ActuatorDigital() = default;
    virtual ~ActuatorDigital() = default;
    virtual uint8_t type() const override { return ACTUATOR_TOGGLE; };
    virtual void setActive(bool active) = 0;
    virtual bool isActive() const = 0;

    friend class ActuatorDigitalMixin;
};


/*
 * An ActuatorRange has a range output between min and max
 */
class ActuatorRange : public virtual Actuator, public ActuatorRangeMixin
{
public:
    ActuatorRange() = default;
    virtual ~ActuatorRange() = default;
    virtual uint8_t type() const override { return ACTUATOR_RANGE; };
    virtual void setValue(temp_t const& val) = 0;
    virtual temp_t getValue() const = 0; // get set value
    virtual temp_t readValue() const = 0; // read actual achieved value
    virtual temp_t min() const = 0;
    virtual temp_t max() const = 0;
};

/*
 * An ThresholdActuator has switches on at a certain threshold. TODO: add hysteresis
 */
class ActuatorThreshold : public virtual Actuator

{
ActuatorThreshold() = default;
    virtual ~ActuatorThreshold() = default;
    virtual uint8_t type() const override { return ACTUATOR_THRESHOLD; };
    virtual void setValue(temp_t const& val) = 0;
    virtual temp_t readValue() const = 0;
    virtual temp_t onValue() const = 0;
    virtual temp_t offValue() const = 0;
};


CONTROL_LIB_END

