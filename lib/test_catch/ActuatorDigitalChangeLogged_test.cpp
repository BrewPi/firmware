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

#include "ActuatorDigitalChangeLogged.h"
#include "ActuatorDigitalMock.h"

SCENARIO("ActuatorDigitalChangeLogged test", "[ActuatorChangeLog]")
{
    using State = ActuatorDigital::State;

    auto mock = ActuatorDigitalMock();
    auto logged = ActuatorDigitalChangeLogged(mock);
    ticks_millis_t now = 1000;

    WHEN("ActuatorDigitalChangeLogged is newly constructed")
    {

        THEN("History is initialized at Unknown in the past and current state since construction")
        {

            auto times = logged.getLastStartEndTime(State::Unknown, now);
            CHECK(times.start == -1);
            CHECK(times.end == 0);

            auto millisSinceUnknown = now - times.end;
            CHECK(millisSinceUnknown == 1000);

            // Active has never occurred, so returns now + 1 (as far in the past as possible)
            times = logged.getLastStartEndTime(State::Active, now);
            CHECK(times.start == now + 1);
            CHECK(times.end == now + 1);

            times = logged.getLastStartEndTime(State::Inactive, now);
            CHECK(times.start == 0);
            CHECK(times.end == now);

            CHECK(times.end - times.start == 1000);
        }

        THEN("Changes are logged")
        {
            now = 3000;
            logged.state(State::Active, now);

            // set to inactive a few times to check that only changes are logged, not each time a value is set
            for (now = 4000; now < 4010; ++now) {
                logged.state(State::Inactive, now);
            }

            auto times = logged.getLastStartEndTime(State::Unknown, now);
            CHECK(times.start == -1);
            CHECK(times.end == 0);

            times = logged.getLastStartEndTime(State::Inactive, now);
            CHECK(times.start == 4000);
            CHECK(times.end == 4010);

            times = logged.getLastStartEndTime(State::Active, now);
            CHECK(times.start == 3000);
            CHECK(times.end == 4000);
        }

        THEN("Durations of current and previous period are correctly calculated")
        {
            for (; now < 10300; ++now) {
                auto newState = ((now / 1000) % 2) == 0 ? State::Active : State::Inactive;
                logged.state(newState, now);
            }

            // when output is still active
            CHECK(logged.state() == State::Active);
            auto durations = logged.activeDurations(now);
            CHECK(durations.currentActive == 300);
            CHECK(durations.currentPeriod == 1300);
            CHECK(durations.previousActive == 1000);
            CHECK(durations.previousPeriod == 2000);

            for (; now < 11300; ++now) {
                auto newState = ((now / 1000) % 2) == 0 ? State::Active : State::Inactive;
                logged.state(newState, now);
            }

            // when output is inactive
            durations = logged.activeDurations(now);
            CHECK(durations.currentActive == 1000);
            CHECK(durations.currentPeriod == 1300);
            CHECK(durations.previousActive == 1000);
            CHECK(durations.previousPeriod == 2000);
        }
    }
}
