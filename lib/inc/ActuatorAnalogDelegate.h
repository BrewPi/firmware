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
#include "ActuatorInterfaces.h"
#include "RefTo.h"


/*
 * An ActuatorDigital simply turns something on or off.
 */
class ActuatorAnalogDelegate :
    public virtual ActuatorAnalog,
    public ActuatorAnalogDelegateMixin
{
public:
    ActuatorAnalogDelegate() = default;
    ActuatorAnalogDelegate(std::function<Interface * ()> lookup) {
        actuator.setLookup(lookup);
    }
    virtual ~ActuatorAnalogDelegate() = default;


    void setLookup(std::function<Interface* ()> lookup) {
        actuator.setLookup(lookup);
    }

    void unsetLookup() {
        actuator.setLookup(nullptr);
    }

    std::function<Interface * ()> getLookup() {
        return actuator.getLookup();
    }

    void accept(VisitorBase & v) final {
        v.visit(*this);
    }

    void update() final {
        actuator().update();
    }

    void fastUpdate() final {
        actuator().fastUpdate();
    }

    void set(temp_t const& val) final {
        actuator().set(val);
    }

    temp_t setting() const final {
        return actuator().setting();
    }

    temp_t value() const final {
        return actuator().value();
    }


private:
    RefTo<ActuatorAnalog> actuator;

    friend class ActuatorAnalogDelegateMixin;
};
