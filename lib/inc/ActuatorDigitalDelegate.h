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


class ActuatorDigitalDelegate :
    public ActuatorDigital,
    public Delegate<ActuatorDigital>,
    public ActuatorDigitalDelegateMixin
{
public:
    ActuatorDigitalDelegate() = default;
    ActuatorDigitalDelegate(std::function<Interface * ()> lookup) {
        delegate.setLookup(lookup);
    }
    virtual ~ActuatorDigitalDelegate() = default;

    void accept(VisitorBase & v) final {
        v.visit(*this);
    }

    void update() final {
        delegate().update();
    }

    void fastUpdate() final {
        delegate().fastUpdate();
    }

    void setState(State state, int8_t priority = 127) final {
        delegate().setState(state, priority);
    }

    State getState() const final {
        return delegate().getState();
    }


friend class ActuatorDigitalDelegateMixin;
};
