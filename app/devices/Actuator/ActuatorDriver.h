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
    ActuatorDigital * target;

public:
    ActuatorDriver(ActuatorDigital * _target) : target(_target){}
    virtual ~ActuatorDriver(){};

    virtual void update(){
        target->update();
    }

    ActuatorDigital * getTarget(){ return target; }

    virtual Actuator * doGetBareActuator(){
        if( target->getBareActuator() == target){
            return target; // my target is bottom
        }
        else{
            return target->getBareActuator(); // my target is not bottom
        }
    }

    virtual bool doInstallActuatorFinalTarget(ActuatorDigital * a){
        if(target->getBareActuator() == target){
            // I am the lowest level driver. my target is the bottom target
            if(target == a){
                return false; // actuator was already installed
            }
            if(target != defaultActuator()){
                delete target; // target is only referenced here and should be deleted
            }
            target = a;
            return true; // installed new actuator
        }
        else{
            return target->installActuatorFinalTarget(a); // my target is not bottom
        }
    }

    bool doUnInstallActuatorFinalTarget(){
        return installActuatorFinalTarget(defaultActuator());
    }
};

