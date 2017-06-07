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
#include "temperatureFormats.h"
#include "Interface.h"
#include "ProcessValue.h"

/*
 * An ActuatorDigital simply turns something on or off.
 */
class ActuatorDigital : public virtual Interface, public virtual ActuatorDigitalMixin
{
public:
    ActuatorDigital() = default;
    virtual ~ActuatorDigital() = default;
    virtual void setActive(bool active, int8_t priority = 127) = 0;
    virtual bool isActive() const = 0;

    friend class ActuatorDigitalMixin;
};


/*
 * An ActuatorAnalog has a range output between min and max
 */
class ActuatorAnalog :
        public virtual ProcessValue,
        public virtual ActuatorAnalogMixin
{
public:
    ActuatorAnalog() = default;
    virtual ~ActuatorAnalog() = default;

    friend class ActuatorAnalogMixin;
};


