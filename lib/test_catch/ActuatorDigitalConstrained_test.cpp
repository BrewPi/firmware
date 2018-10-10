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

#include "ActuatorDigitalConstrained.h"
#include "ActuatorDigitalMock.h"

using State = ActuatorDigital::State;

SCENARIO("ActuatorDigitalConstrained", "[constraints]")
{
    auto now = ticks_millis_t(0);
    auto mock = ActuatorDigitalMock();
    auto constrained = ActuatorDigitalConstrained(mock);

    WHEN("A minimum ON time constrained is added, the actuator cannot turn off before it has passed")
    {
        constrained.addConstraint(std::make_unique<ADConstraints::MinOnTime>(1500));
        constrained.state(State::Active, now);
        CHECK(constrained.state() == State::Active);
        CHECK(mock.state() == State::Active);

        now += 1499;
        constrained.state(ActuatorDigital::Inactive, now);
        CHECK(constrained.state() == State::Active);
        CHECK(mock.state() == State::Active);

        now += 1;
        constrained.state(ActuatorDigital::Inactive, now);
        CHECK(constrained.state() == State::Inactive);
        CHECK(mock.state() == State::Inactive);
    }

    WHEN("A minimum OFF time constrained is added, the actuator cannot turn on before it has passed")
    {
        constrained.addConstraint(std::make_unique<ADConstraints::MinOffTime>(1500));
        constrained.state(State::Inactive, now);
        CHECK(constrained.state() == State::Inactive);
        CHECK(mock.state() == State::Inactive);

        now += 1499;
        constrained.state(ActuatorDigital::Active, now);
        CHECK(constrained.state() == State::Inactive);
        CHECK(mock.state() == State::Inactive);

        now += 1;
        constrained.state(ActuatorDigital::Active, now);
        CHECK(constrained.state() == State::Active);
        CHECK(mock.state() == State::Active);
    }

    WHEN("A minimum ON and a minimum OFF time are added, both are honored")
    {
        constrained.state(State::Inactive, now);
        constrained.addConstraint(std::make_unique<ADConstraints::MinOffTime>(1000));
        constrained.addConstraint(std::make_unique<ADConstraints::MinOnTime>(2000));

        while (constrained.state() == State::Inactive) {
            constrained.state(State::Active, ++now);
        }
        auto timesOff = constrained.getLastStartEndTime(State::Inactive, now);
        CHECK(timesOff.end - timesOff.start == 1000);

        while (constrained.state() == State::Active) {
            constrained.state(State::Inactive, ++now);
        }

        auto timesOn = constrained.getLastStartEndTime(State::Active, now);
        CHECK(timesOn.end - timesOn.start == 2000);
    }
}

SCENARIO("Mutex contraint", "[constraints]")
{
    auto now = ticks_millis_t(0);
    auto mock1 = ActuatorDigitalMock();
    auto constrained1 = ActuatorDigitalConstrained(mock1);
    auto mock2 = ActuatorDigitalMock();
    auto constrained2 = ActuatorDigitalConstrained(mock2);
    auto mut = std::make_shared<TimedMutex>();

    constrained1.addConstraint(std::make_unique<ADConstraints::Mutex>(
        [&mut]() {
            return mut;
        }));
    constrained2.addConstraint(std::make_unique<ADConstraints::Mutex>(
        [&mut]() {
            return mut;
        }));

    WHEN("Two actuators share a mutex, they cannot be active at the same time")
    {
        constrained1.state(State::Active, ++now);
        CHECK(constrained1.state() == State::Active);
        constrained2.state(State::Active, ++now);
        CHECK(constrained2.state() == State::Inactive);

        constrained1.state(State::Inactive, ++now);
        constrained2.state(State::Active, ++now);
        CHECK(constrained2.state() == State::Active);

        constrained1.state(State::Active, ++now);
        CHECK(constrained1.state() == State::Inactive);
    }

    WHEN("A minimum OFF time constraint holds an actuator low, it doesn't lock the mutex")
    {
        constrained1.addConstraint(std::make_unique<ADConstraints::MinOffTime>(1000));
        constrained1.state(State::Active, ++now);
        CHECK(constrained1.state() == State::Inactive);
        constrained2.state(State::Active, ++now);
        CHECK(constrained2.state() == State::Active);
    }

    WHEN("An actuator doesn't have the mutex, it won't unlock it when set to Inactive")
    {
        constrained1.state(State::Active, ++now);
        CHECK(constrained1.state() == State::Active);
        constrained2.state(State::Inactive, ++now);
        constrained2.state(State::Active, ++now);
        CHECK(constrained2.state() == State::Inactive);
    }

    WHEN("A minimum switch time of 1000 is set on the mutex and actuator 1 was active before")
    {
        mut->minSwitchTime(1000);
        constrained1.state(State::Active, ++now);
        CHECK(constrained1.state() == State::Active);

        constrained1.state(State::Inactive, ++now);
        CHECK(constrained1.state() == State::Inactive);

        THEN("Actuator 1 can go active again immediately")
        {
            constrained1.state(State::Active, ++now);
            CHECK(constrained1.state() == State::Active);
        }

        THEN("Actuator 2 has to wait until no actuator has been active for 1000ms")
        {
            constrained2.state(State::Active, ++now);
            CHECK(constrained2.state() == State::Inactive);

            while (constrained2.state() != State::Active && now < 2000) {
                constrained2.state(State::Active, ++now);
            }
            CHECK(now == 1002);
        }
    }

    WHEN("The state is changed without providing the current time, it is applied using the last update time")
    {
        constrained1.state(State::Active, 2000);
        CHECK(constrained1.state() == State::Active);

        constrained1.update(6000);
        constrained1.state(State::Inactive);

        auto activeTimes = constrained1.getLastStartEndTime(State::Active, 8000);
        auto inactiveTimes = constrained1.getLastStartEndTime(State::Inactive, 8000);

        CHECK(activeTimes.start == 2000);
        CHECK(activeTimes.end == 6000);

        CHECK(inactiveTimes.start == 6000);
        CHECK(inactiveTimes.end == 8000);
    }
}
