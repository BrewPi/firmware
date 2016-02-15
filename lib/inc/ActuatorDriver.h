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
#include "ActuatorInterfaces.h"
#include "json_adapter.h"
#include "defaultDevices.h"

/*
 * A DriverActuator drivers another digital actuator, for example a PWM actuator can drive a pin actuator
 */
class ActuatorDriver : public virtual Actuator
{
protected:
    std::shared_ptr<ActuatorDigital> target;

public:
    ActuatorDriver(std::shared_ptr<ActuatorDigital> _target) : target(_target){}
protected:
    ~ActuatorDriver() = default; // should not be destructed through this base class

public:
    virtual void update() override {
        target->update();
    }

    std::shared_ptr<ActuatorDigital> & getTarget() { return target; }

    const Actuator * getBareActuator() const final {
        if( target->getBareActuator() == target.get()){
            return target.get(); // my target is bottom
        }
        else{
            return target->getBareActuator(); // my target is not bottom
        }
    }

    bool installActuatorFinalTarget(const std::shared_ptr<ActuatorDigital> & a) final{
        if(target->getBareActuator() == target.get()){
            // I am the lowest level driver. my target is the bottom target
            if(target == a){
                return false; // actuator was already installed
            }
            target = a;
            return true; // installed new actuator
        }
        else{
            return target->installActuatorFinalTarget(a); // my target is not bottom
        }
    }

    bool uninstallActuatorFinalTarget() final {
        return installActuatorFinalTarget(defaultActuator());
    }
};

