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
#include "Interface.h"
#include "ControllerMixins.h"

/*
 * A process value has a setting and an current value
 */
class ProcessValue :
    public virtual Interface,
    public virtual ProcessValueMixin
{
public:
    ProcessValue() = default;
    virtual ~ProcessValue() = default;

    // set the setting for the process value
    virtual void set(temp_t const& setting) = 0;
    // get the setting for the process value
    virtual temp_t setting() const = 0;
    // read the actual value of the process value
    virtual temp_t value() const = 0;
    

private:
    friend class ProcessValueMixin;
};
