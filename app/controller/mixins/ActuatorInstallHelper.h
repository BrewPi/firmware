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

class ActuatorDigitalInterface;
class ActuatorForwarder;


class ActuatorInstallHelper {
public:
    ActuatorInstallHelper() = default;
protected:
    ~ActuatorInstallHelper() = default;
public:
    // recursive call for composite driver classes, until a non-driver class is reached
    // Forwarding actuators override this function to forward this call to their target
    virtual ActuatorInstallHelper * getNonForwarder(){
        return this;
    }
    // install pin or mock actuator at the lowest level, returns true if Actuator a was installed
    // Forwarding actuators override this function
    virtual bool replaceNonForwarder(ActuatorDigitalInterface * a){
        return false;
    }
    // uninstall pin or mock actuator at the lowest level, return success (true = an actuator was uninstalled)
    // forwarding actuator override this function to search for and replace a non-forwarding actuator with a default actuator
    virtual bool removeNonForwarder() {
        return false; // does nothing for non-forwarding actuators;
    }
};


class ActuatorInstallHelperForwarder :
        public virtual  ActuatorInstallHelper
    {
public:
    ActuatorInstallHelperForwarder() = default;
protected:
    ~ActuatorInstallHelperForwarder() = default;
public:

    ActuatorForwarder * cast();

    ActuatorInstallHelper * getNonForwarder() override final;

    bool replaceNonForwarder(ActuatorDigitalInterface * a) override final;

    bool removeNonForwarder() override final;
};
