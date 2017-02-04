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
#include "RefTo.h"


class ActuatorToggleDelegate : public virtual ActuatorDigitalInterface
    , public virtual ActuatorToggleDelegateMixin
{
public:
    ActuatorToggleDelegate() = default;
    ActuatorToggleDelegate(std::function<Interface * ()> lookup) {
        actuator.setLookup(lookup);
    }
    virtual ~ActuatorToggleDelegate() = default;

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

    void setActive(bool active, int8_t priority = 127) final {
        actuator().setActive(active, priority);
    }

    bool isActive() const final {
        return actuator().isActive();
    }

private:
    RefTo<ActuatorDigitalInterface> actuator;

    friend class ActuatorToggleDelegateMixin;
};
