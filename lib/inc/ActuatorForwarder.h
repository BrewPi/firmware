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

class ActuatorDigitalInterface;

/*
 * A forwarding actuator drives another digital actuator, for example a PWM actuator can drive a pin actuator
 */
class ActuatorForwarder : public ActuatorForwarderMixin
{
protected:
    ActuatorDigitalInterface * target;

public:
    ActuatorForwarder(ActuatorDigitalInterface * _target) : target(_target){}
protected:
    ~ActuatorForwarder() = default; // should not be destructed through this base class

public:
    ActuatorDigitalInterface * getTarget(){
        return target;
    }

    void setTarget(ActuatorDigitalInterface * target_){
        target = target_;
    }
};

