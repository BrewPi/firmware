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

class SetpointDelegate :
    public Setpoint,
    public SetpointDelegateMixin
{
public:
    SetpointDelegate(BaseLookup const& lookup) : delegate(lookup){}
    ~SetpointDelegate() = default;

    virtual void accept(VisitorBase & v) override final {
        v.visit(*this);
    }

    virtual temp_t read() const override final {
        return delegate().read();
    }

    virtual void write(temp_t val) override final {
        delegate().write(val);
    }

    virtual update_t update(const update_t& now) override final {
        return update_t_max(); // no updates needed
    }

private:
    RefTo<Setpoint> delegate;

friend class SetpointDelegateMixin;
};
