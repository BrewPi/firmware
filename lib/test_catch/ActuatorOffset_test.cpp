/*
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

#include "ActuatorOffset.h"
#include "SetpointSensorPair.h"
#include "TempSensorMock.h"
#include <catch.hpp>
#include <memory>

SCENARIO("ActuatorOffset offsets one setpoint from another", "[ActuatorOffset]")
{

    auto targetSetpoint = std::make_shared<SetpointSimple>(20.0);
    auto referenceSetpoint = std::make_shared<SetpointSimple>(20.0);
    auto targetSensor = std::make_shared<TempSensorMock>(20.0);
    auto referenceSensor = std::make_shared<TempSensorMock>(19.0);

    auto target = std::make_shared<SetpointSensorPair>(
        [targetSetpoint]() { return targetSetpoint; },
        [targetSensor]() { return targetSensor; });

    auto reference = std::make_shared<SetpointSensorPair>(
        [referenceSetpoint]() { return referenceSetpoint; },
        [referenceSensor]() { return referenceSensor; });

    auto act = std::make_shared<ActuatorOffset>(
        [target]() { return target; },
        [reference]() { return reference; });

    WHEN("The actuator is written, the target is offset from the reference")
    {
        act->setting(10.0);
        CHECK(referenceSetpoint->setting() == 20.0);
        CHECK(targetSetpoint->setting() == 30.0);
        CHECK(act->setting() == 10.0); // difference between setpoints is now 10
        CHECK(act->value() == 0.0);    // actual value is still zero, because targetSensor has not changed

        targetSensor->value(30.0);
        act->update();
        CHECK(act->value() == 10.0); // actual value is 10 when sensor has reached setpoint

        act->setting(-10.0);
        CHECK(referenceSetpoint->setting() == 20.0);
        CHECK(targetSetpoint->setting() == 10.0);
        CHECK(act->setting() == -10.0); // difference between setpoints is now 10
        act->update();
        CHECK(act->value() == 10.0); // value is still 10, because targetSensor has not changed

        targetSensor->value(10.0);
        act->update();
        CHECK(act->value() == -10.0); // value is -10 when sensor has reached setpoint

        referenceSetpoint->setting(10.0);
        referenceSensor->value(15.0);
        targetSetpoint->setting(20.0);
        targetSensor->value(20.0);
        act->setting(12.0);

        // when using the reference setting as value to offset from (default):
        CHECK(act->value() == 10.0);   // value() returns target sensor - ref setpoint
        CHECK(act->setting() == 12.0); // setting() returns target setpoint - ref setpoint
        CHECK(targetSetpoint->setting() == 22.0);

        // when using the reference value as value to offset from:
        act->selectedReference(ActuatorOffset::ReferenceSelection::VALUE);
        act->setting(12.0);

        CHECK(act->value() == 5.0);    // value() returns target sensor - ref sensor value
        CHECK(act->setting() == 12.0); // setting() returns target setpoint - ref sensor value
        CHECK(targetSetpoint->setting() == 27.0);
    }

    WHEN("the reference setting is used and the reference sensor is invalid, but the reference setpoint is valid"
         "target setpoint be invalid and actuator value is 0, because it cannot get feedback on the actual value")
    {
        act->selectedReference(ActuatorOffset::SETTING);
        targetSetpoint->setting(20);
        referenceSensor->connected(false);
        act->setting(12.0);

        CHECK(targetSetpoint->setting() == 20.0); // unchanged
        CHECK(act->value() == 0);
        CHECK(act->valid() == false);
        CHECK(act->setting() == 12.0); // setting() still returns requested offset
        CHECK(targetSetpoint->valid() == false);
    }

    WHEN("the reference setting is used and reference sensor is valid, but the reference setpoint is invalid"
         "target setpoint will be invalid, and actuator value is 0")
    {
        act->selectedReference(ActuatorOffset::SETTING);
        referenceSensor->connected(true);
        referenceSetpoint->valid(false);
        act->setting(12.0);

        CHECK(targetSetpoint->setting() == 20.0); // unchanged
        CHECK(act->value() == 0);
        CHECK(act->valid() == false);
        CHECK(act->setting() == 12.0); // setting() still returns requested offset
        CHECK(targetSetpoint->valid() == false);
    }

    WHEN("the reference value is used and the reference sensor is invalid, but the reference setpoint is valid"
         "target setpoint be invalid and actuator value is 0, because it cannot get feedback on the actual value")
    {
        act->selectedReference(ActuatorOffset::VALUE);
        targetSetpoint->setting(20);
        referenceSensor->connected(false);
        act->setting(12.0);

        CHECK(targetSetpoint->setting() == 20.0); // unchanged
        CHECK(act->value() == 0);
        CHECK(act->valid() == false);
        CHECK(act->setting() == 12.0); // setting() still returns requested offset
        CHECK(targetSetpoint->valid() == false);
    }

    WHEN("the reference value is used and reference sensor is valid, but the reference setpoint is invalid"
         "target setpoint will be invalid, and actuator value is 0")
    {
        act->selectedReference(ActuatorOffset::VALUE);
        referenceSensor->connected(true);
        referenceSetpoint->valid(false);
        act->setting(12.0);

        CHECK(targetSetpoint->setting() == 20.0); // unchanged
        CHECK(act->value() == 0);
        CHECK(act->valid() == false);
        CHECK(act->setting() == 12.0); // setting() still returns requested offset
        CHECK(targetSetpoint->valid() == false);
    }
}
