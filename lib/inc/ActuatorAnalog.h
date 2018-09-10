/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of the BrewBlox Control Library.
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

#include "FixedPoint.h"
#include "ProcessValue.h"
#include <cstdint>

using ActuatorAnalogValue = safe_elastic_fixed_point<7, 8, int16_t>;

/*
 * An ActuatorAnalog has a range output between min and max
 */
class ActuatorAnalog : public ProcessValue<ActuatorAnalogValue> {
public:
    using value_t = ActuatorAnalogValue;

    ActuatorAnalog() = default;
    virtual ~ActuatorAnalog() = default;
};
