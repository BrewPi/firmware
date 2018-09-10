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

#include <cstdint>

/*
 * An ActuatorDigital simply turns something on or off.
 */
class ActuatorDigital {
public:
    enum State : uint8_t {
        Inactive,
        Active,
        Unknown
    };

    ActuatorDigital() = default;
    virtual ~ActuatorDigital() = default;

    virtual void setState(const State& state) = 0;

    // no bool return type offered, because this forces classes to implement handling the Unknown state
    virtual State getState() const = 0;
};
