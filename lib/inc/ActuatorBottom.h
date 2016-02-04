/*
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
#include "ActuatorInterfaces.h"
#include "ControlLib.h"

CONTROL_LIB_BEGIN

// Bottom class to be inherited by non-driver actuators
class ActuatorBottom : public virtual Actuator {
public:
    ActuatorBottom() = default;
protected:
    ~ActuatorBottom() = default;
public:
    Actuator * getBareActuator() final {
        return this;  // recursive call for composite driver classes, until a non-driver class is reached
    }
    bool installActuatorFinalTarget(ActuatorDigital * a) final {
        return false; // does nothing for non-driver actuators
    }

    bool uninstallActuatorFinalTarget() final {
        return false; // does nothing for non-driver actuators
    }
};

CONTROL_LIB_END
