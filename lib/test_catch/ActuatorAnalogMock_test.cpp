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

#include <catch.hpp>

#include "../inc/ActuatorAnalogMock.h"

SCENARIO("ActuatorAnalogMock test")
{
    WHEN("An ActuatorAnalogMock is constructed, initial values are corrent and min/max is applied")
    {
        auto act = ActuatorAnalogMock();

        CHECK(act.setting() == 0);
        CHECK(act.value() == 0);
        CHECK(act.valid() == true);

        act = ActuatorAnalogMock(50);

        CHECK(act.setting() == 50);
        CHECK(act.value() == 50);
        CHECK(act.valid() == true);

        act = ActuatorAnalogMock(110, 0, 100);

        CHECK(act.setting() == 100);
        CHECK(act.value() == 100);
        CHECK(act.valid() == true);

        act = ActuatorAnalogMock(-10, 5, 100);

        CHECK(act.setting() == 5);
        CHECK(act.value() == 5);
        CHECK(act.valid() == true);
    }
}
