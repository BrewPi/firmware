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

#include "../inc/TempSensorMock.h"
#include "../inc/Temperature.h"

#include <math.h>

SCENARIO("TempSensorMockTest")
{

    WHEN("A mock sensor is initialized without providing an initial value, it reads as invalid and 0")
    {
        auto mock = TempSensorMock();

        CHECK(mock.value() == temp_t(0));
        CHECK(mock.valid() == false);
    }

    WHEN("A mock sensor is initialized with an initial value, it reads as valid and that value")
    {
        auto mock = TempSensorMock(20.0);

        CHECK(mock.value() == temp_t(20.0));
        CHECK(mock.valid() == true);
    }

    WHEN("A mock sensor is disconnected, valid() returns false")
    {
        auto mock = TempSensorMock(20.0);
        mock.connected(false);

        CHECK(mock.valid() == false);

        AND_WHEN("It is reconnected, valid() returns true and it reads as the set value again")
        {
            mock.connected(true);

            CHECK(mock.valid() == true);
            CHECK(mock.value() == temp_t(20.0));
        }
    }
}
