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
#include "Delegate.h"
#include "ProcessValue.h"
#include "VisitorBase.h"

class ProcessValueDelegate :
    public ProcessValue,
    public Delegate<ProcessValue>,
    public ProcessValueDelegateMixin
{
public:
    ProcessValueDelegate() = default;
    ProcessValueDelegate(std::function<Interface * ()> lookup) : Delegate<ProcessValue>(lookup){}
    ~ProcessValueDelegate() = default;

    void accept(VisitorBase & v) final {
        v.visit(*this);
    }

    void update() final {
        delegate().update();
    }

    void fastUpdate() final {
        delegate().fastUpdate();
    }

    // set the setting for the process value
    void set(temp_t const& setting) override final {
        delegate().set(setting);
    }
    // get the setting for the process value
    temp_t setting() const override final {
        return delegate().setting();
    }
    // read the actual value of the process value
    temp_t value() const override final {
        return delegate().value();
    }
    
private:
    friend class ProcessValueDelegateMixin;
};
