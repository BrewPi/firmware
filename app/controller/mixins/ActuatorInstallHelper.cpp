/*
 * Copyright 2016 BrewPi/Elco Jacobs.
 * Copyright 2016 Matthew McGowan.
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

#include "ActuatorInstallHelper.h"
#include "ActuatorInterfaces.h"
#include "defaultDevices.h"
#include "ActuatorForwarder.h"

ActuatorForwarder * ActuatorInstallHelperForwarder::cast(){
    return static_cast<ActuatorForwarder *>(this);
}

ActuatorInstallHelper * ActuatorInstallHelperForwarder::getNonForwarder() {
    ActuatorForwarder * af = cast();
    if( af->getTarget()->getNonForwarder() == af->getTarget()){
        return af->getTarget(); // my target is bottom
    }
    else{
        return af->getTarget()->getNonForwarder(); // my target is not bottom
    }
}

bool ActuatorInstallHelperForwarder::replaceNonForwarder(ActuatorDigital * a) {
    ActuatorForwarder * af = cast();
    if(af->getTarget()->getNonForwarder() == af->getTarget()){
        // I am the lowest level driver. my target is the bottom target
        if(af->getTarget() == a){
            return false; // actuator was already installed
        }
        if(af->getTarget() != defaultActuator()){
            delete af->getTarget(); // target is only referenced here and should be deleted
        }
        af->setTarget(a);
        return true; // installed new actuator
    }
    else{
        return af->getTarget()->replaceNonForwarder(a); // my target is not bottom
    }
}

bool ActuatorInstallHelperForwarder::removeNonForwarder() {
    return replaceNonForwarder(defaultActuator());
}
