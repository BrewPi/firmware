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

#include "../inc/SetpointProfile.h"
#include "../inc/Temperature.h"
#include <cstdint>

SCENARIO("SetpointProfile test", "[SetpointProfile]")
{
    auto deviceStartTime = ticks_seconds_t(10);
    auto sp = SetpointProfile(deviceStartTime);

    WHEN("the profile has no values, it is not valid and returns 0")
    {
        CHECK(sp.setting() == temp_t(0));
        CHECK(sp.valid() == false);

        sp.update(0);

        CHECK(sp.setting() == temp_t(0));
        CHECK(sp.valid() == false);
    }

    WHEN("the profile contains a single temp, it is always used")
    {
        sp.addPoint(SetpointProfile::Point{ticks_seconds_t(11), temp_t(10)});

        sp.update(0);
        CHECK(sp.setting() == temp_t(10));

        sp.update(500);
        CHECK(sp.setting() == temp_t(10));

        sp.update(1000);
        CHECK(sp.setting() == temp_t(10));

        sp.update(1500);
        CHECK(sp.setting() == temp_t(10));
    }

    WHEN("the profile contains two temperatures")
    {
        sp.addPoint(SetpointProfile::Point{ticks_seconds_t(11), temp_t(10)});
        sp.addPoint(SetpointProfile::Point{ticks_seconds_t(21), temp_t(20)});

        AND_WHEN("The time is before the first point, the first temp is used")
        {
            sp.update(500);
            CHECK(sp.setting() == temp_t(10));
        }

        AND_WHEN("The time is after the last point, the last temp is used")
        {
            sp.update(12000);
            CHECK(sp.setting() == temp_t(20));
        }

        AND_WHEN("The time between the 2 points, it is correctly interpolated")
        {
            sp.update(2000);
            CHECK(sp.setting() == temp_t(11));

            sp.update(6000);
            CHECK(sp.setting() == temp_t(15));
        }

        AND_WHEN("The time is exactly at a point, that temp is used")
        {
            sp.update(1000);
            CHECK(sp.setting() == temp_t(10));

            sp.update(11000);
            CHECK(sp.setting() == temp_t(20));
        }
    }

    WHEN("the profile contains multiple temperatures")
    {
        sp.addPoint(SetpointProfile::Point{ticks_seconds_t(11), temp_t(10)});
        sp.addPoint(SetpointProfile::Point{ticks_seconds_t(21), temp_t(20)});
        sp.addPoint(SetpointProfile::Point{ticks_seconds_t(31), temp_t(40)});

        AND_WHEN("The time is before the first point, the first temp is used")
        {
            sp.update(500);
            CHECK(sp.setting() == temp_t(10));
        }

        AND_WHEN("The time is after the last point, the last temp is used")
        {
            sp.update(22000);
            CHECK(sp.setting() == temp_t(40));
        }

        AND_WHEN("The time between the 2 points, it is correctly interpolated")
        {
            sp.update(2000);
            CHECK(sp.setting() == temp_t(11));

            sp.update(6000);
            CHECK(sp.setting() == temp_t(15));

            sp.update(12000);
            CHECK(sp.setting() == temp_t(22));

            sp.update(20000);
            CHECK(sp.setting() == temp_t(38));
        }

        AND_WHEN("The time is exactly at a point, that temp is used")
        {
            sp.update(1000);
            CHECK(sp.setting() == temp_t(10));

            sp.update(11000);
            CHECK(sp.setting() == temp_t(20));

            sp.update(21000);
            CHECK(sp.setting() == temp_t(40));
        }
    }

    WHEN("the profile contains two temperatures with the same time")
    {
        sp.addPoint(SetpointProfile::Point{ticks_seconds_t(11), temp_t(10)});
        sp.addPoint(SetpointProfile::Point{ticks_seconds_t(21), temp_t(20)});
        sp.addPoint(SetpointProfile::Point{ticks_seconds_t(21), temp_t(30)});
        sp.addPoint(SetpointProfile::Point{ticks_seconds_t(31), temp_t(40)});

        AND_WHEN("The time is before the step, it is correctly interpolated with the first point of the step")
        {
            sp.update(2000);
            CHECK(sp.setting() == temp_t(11));

            sp.update(6000);
            CHECK(sp.setting() == temp_t(15));
        }

        AND_WHEN("The time is at the step (rounded down to whole seconds), it is equal to the first point of the step")
        {
            sp.update(11000);
            CHECK(sp.setting() == temp_t(20));

            sp.update(11500);
            CHECK(sp.setting() == temp_t(20));
        }

        AND_WHEN("The time is after the step, it is correctly interpolated with the second point of the step")
        {

            sp.update(12000);
            CHECK(sp.setting() == temp_t(31));

            sp.update(21000);
            CHECK(sp.setting() == temp_t(40));
        }
    }

    WHEN("The device start time is still at 0, the setpoint is invalid")
    {
        deviceStartTime = 0;
        sp.addPoint(SetpointProfile::Point{ticks_seconds_t(11), temp_t(10)});
        sp.addPoint(SetpointProfile::Point{ticks_seconds_t(21), temp_t(20)});

        sp.update(1000);
        CHECK(sp.valid() == false);
        CHECK(sp.setting() == 0);

        sp.update(10000);
        CHECK(sp.valid() == false);
        CHECK(sp.setting() == 0);
    }
}
