/*
 * Copyright 2018 BrewPi B.V./Elco Jacobs.
 *
 * This file is part of BrewBlox.
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

#include "ActuatorAnalogMock.h"
#include "ActuatorOffset.h"
#include "Pid.h"
#include "Setpoint.h"
#include "SetpointSensorPair.h"
#include "TempSensorMock.h"
#include <iostream>
#include <math.h>

SCENARIO("PID Test with mock actuator", "[pid]")
{
    auto setpoint = std::make_shared<SetpointSimple>(20.0);
    auto sensor = std::make_shared<TempSensorMock>(20.0);

    auto pair = std::make_shared<SetpointSensorPair>(
        [&setpoint]() { return setpoint; },
        [&sensor]() { return sensor; });

    auto actuator = std::make_shared<ActuatorAnalogMock>();

    auto pid = Pid(
        [&pair]() { return pair; },
        [&actuator]() { return actuator; });

    pid.enabled(true);

    WHEN("Only proportional gain is active, the output value is correct")
    {
        pid.kp(10);
        pid.ti(0);
        pid.td(0);

        setpoint->setting(21);
        sensor->value(20);

        for (int32_t i = 0; i < 100; ++i) {
            pid.update(); // update 100 times to settle input filter
        }
        CHECK(actuator->setting() == Approx(10).margin(0.01));
    }

    WHEN("Proportional gain and integrator are enabled, the output value is correct")
    {
        pid.kp(10);
        pid.ti(2000);
        pid.td(0);

        setpoint->setting(21);
        sensor->value(20);

        for (int32_t i = 0; i < 1000; ++i) {
            pid.update();
        }

        CHECK(pid.p() == Approx(10).epsilon(0.001));
        CHECK(pid.i() == Approx(5).epsilon(0.05));
        CHECK(pid.d() == 0);
        CHECK(actuator->setting() == Approx(10.0 * (1.0 + 1000 * 1.0 / 2000)).epsilon(0.02));

        for (int32_t i = 0; i < 1000; ++i) {
            pid.update();
        }

        CHECK(pid.p() == Approx(10).epsilon(0.001));
        CHECK(pid.i() == Approx(10).epsilon(0.05));
        CHECK(pid.d() == 0);
        CHECK(actuator->setting() == Approx(10.0 * (1.0 + 2000 * 1.0 / 2000)).epsilon(0.02));
    }

    WHEN("Proportional, Integral and Derivative are enabled, the output value is correct with positive Kp")
    {
        pid.kp(10);
        pid.ti(2000);
        pid.td(200);

        setpoint->setting(30);
        sensor->value(20);

        temp_t mockVal;
        double accumulatedError = 0;
        for (int32_t i = 0; i <= 900; ++i) {
            mockVal = temp_t(20) + (temp_t(9) * i) / 900;
            sensor->value(mockVal);
            pid.update();
            accumulatedError += pid.error();
        }

        CHECK(mockVal == 29);
        CHECK(pid.error() == Approx(1.2).epsilon(0.05)); // the filter introduces some delay, which is why this is not 1.0
        CHECK(pid.p() == Approx(12).epsilon(0.05));
        CHECK(pid.i() == Approx(accumulatedError * (10.0 / 2000)).epsilon(0.01));
        CHECK(pid.d() == Approx(-10 * 9.0 / 900 * 200).epsilon(0.01));

        CHECK(actuator->setting() == pid.p() + pid.i() + pid.d());
    }

    WHEN("Proportional, Integral and Derivative are enabled, the output value is correct with negative Kp")
    {
        pid.kp(-10);
        pid.ti(2000);
        pid.td(200);

        setpoint->setting(20);
        sensor->value(30);

        temp_t mockVal;
        double accumulatedError = 0;
        for (int32_t i = 0; i <= 900; ++i) {
            mockVal = temp_t(30) - (temp_t(9) * i) / 900;
            sensor->value(mockVal);
            pid.update();
            accumulatedError += pid.error();
        }

        CHECK(mockVal == 21);
        CHECK(pid.error() == Approx(-1.2).epsilon(0.05)); // the filter introduces some delay, which is why this is not 1.0
        CHECK(pid.p() == Approx(12).epsilon(0.05));
        CHECK(pid.i() == Approx(accumulatedError * (-10.0 / 2000)).epsilon(0.01));
        CHECK(pid.d() == Approx(-10 * 9.0 / 900 * 200).epsilon(0.01));

        CHECK(actuator->setting() == pid.p() + pid.i() + pid.d());
    }

    WHEN("The actuator setting is clipped, the integrator is limited by anti-windup (positive kp)")
    {
        pid.kp(10);
        pid.ti(2000);
        pid.td(200);

        setpoint->setting(21);
        sensor->value(20);
        actuator->minSetting(0);
        actuator->maxSetting(20);

        double accumulatedError = 0;
        for (int32_t i = 0; i <= 10000; ++i) {
            pid.update();
            accumulatedError += pid.error();
        }

        double integratorValueWithoutAntiWindup = accumulatedError * (10.0 / 2000);
        CHECK(integratorValueWithoutAntiWindup == Approx(50.0).epsilon(0.05));
        CHECK(pid.p() == Approx(10).epsilon(0.01));
        CHECK(pid.i() == Approx(10).epsilon(0.01)); // anti windup limits this to 10
        CHECK(pid.d() == Approx(0).margin(0.01));

        CHECK(actuator->setting() == Approx(20).epsilon(0.01));

        setpoint->setting(19);
        accumulatedError = 0;
        for (int32_t i = 0; i <= 10000; ++i) {
            pid.update();
            accumulatedError += pid.error();
        }

        integratorValueWithoutAntiWindup = accumulatedError * (10.0 / 2000);
        CHECK(integratorValueWithoutAntiWindup == Approx(-50.0).epsilon(0.05));
        CHECK(pid.p() == Approx(-10).epsilon(0.01));
        CHECK(pid.i() == Approx(0).margin(0.01)); // anti windup limits this to 0
        CHECK(pid.d() == Approx(0).margin(0.01));

        CHECK(actuator->setting() == Approx(0).margin(0.01));
    }

    WHEN("The actuator setting is clipped, the integrator is limited by anti-windup (negative kp)")
    {
        pid.kp(-10);
        pid.ti(2000);
        pid.td(200);

        setpoint->setting(19);
        sensor->value(20);
        actuator->minSetting(0);
        actuator->maxSetting(20);

        double accumulatedError = 0;
        for (int32_t i = 0; i <= 10000; ++i) {
            pid.update();
            accumulatedError += pid.error();
        }

        double integratorValueWithoutAntiWindup = accumulatedError * (-10.0 / 2000);
        CHECK(integratorValueWithoutAntiWindup == Approx(50.0).epsilon(0.05));
        CHECK(pid.p() == Approx(10).epsilon(0.01));
        CHECK(pid.i() == Approx(10).epsilon(0.01)); // anti windup limits this to 10
        CHECK(pid.d() == Approx(0).margin(0.01));

        CHECK(actuator->setting() == Approx(20).epsilon(0.01));

        setpoint->setting(21);
        accumulatedError = 0;
        for (int32_t i = 0; i <= 10000; ++i) {
            pid.update();
            accumulatedError += pid.error();
        }

        integratorValueWithoutAntiWindup = accumulatedError * (-10.0 / 2000);
        CHECK(integratorValueWithoutAntiWindup == Approx(-50.0).epsilon(0.05));
        CHECK(pid.p() == Approx(-10).epsilon(0.01));
        CHECK(pid.i() == Approx(0).margin(0.01)); // anti windup limits this to 0
        CHECK(pid.d() == Approx(0).margin(0.01));

        CHECK(actuator->setting() == Approx(0).margin(0.01));
    }

    WHEN("The actuator value is not reaching setting, the integrator is limited by anti-windup (positive kp)")
    {
        pid.kp(10);
        pid.ti(2000);
        pid.td(200);

        setpoint->setting(21);
        sensor->value(20);
        actuator->minValue(5);
        actuator->maxValue(20);

        double accumulatedError = 0;
        for (int32_t i = 0; i <= 10000; ++i) {
            pid.update();
            accumulatedError += pid.error();
        }

        double integratorValueWithoutAntiWindup = accumulatedError * (10.0 / 2000);
        CHECK(integratorValueWithoutAntiWindup == Approx(50.0).epsilon(0.05));
        CHECK(pid.p() == Approx(10).epsilon(0.01));
        CHECK(pid.i() == Approx(10).epsilon(0.01)); // anti windup limits this to 10
        CHECK(pid.d() == Approx(0).margin(0.01));

        CHECK(actuator->setting() == Approx(20).epsilon(0.01));

        setpoint->setting(19);
        accumulatedError = 0;
        for (int32_t i = 0; i <= 10000; ++i) {
            pid.update();
            accumulatedError += pid.error();
        }

        integratorValueWithoutAntiWindup = accumulatedError * (10.0 / 2000);
        CHECK(integratorValueWithoutAntiWindup == Approx(-50.0).epsilon(0.05));
        CHECK(pid.p() == Approx(-10).epsilon(0.01));
        CHECK(pid.i() == Approx(0).margin(0.01)); // anti windup limits this to 0
        CHECK(pid.d() == Approx(0).margin(0.01));

        CHECK(actuator->setting() == Approx(-10).margin(0.01));
    }

    WHEN("The actuator value is not reaching setting, the integrator is limited by anti-windup (negative kp)")
    {
        pid.kp(-10);
        pid.ti(2000);
        pid.td(200);

        setpoint->setting(19);
        sensor->value(20);
        actuator->minValue(5);
        actuator->maxValue(20);

        double accumulatedError = 0;
        for (int32_t i = 0; i <= 10000; ++i) {
            pid.update();
            accumulatedError += pid.error();
        }

        double integratorValueWithoutAntiWindup = accumulatedError * (-10.0 / 2000);
        CHECK(integratorValueWithoutAntiWindup == Approx(50.0).epsilon(0.05));
        CHECK(pid.p() == Approx(10).epsilon(0.01));
        CHECK(pid.i() == Approx(10).epsilon(0.01)); // anti windup limits this to 10
        CHECK(pid.d() == Approx(0).margin(0.01));

        CHECK(actuator->setting() == Approx(20).epsilon(0.01));

        setpoint->setting(21);
        accumulatedError = 0;
        for (int32_t i = 0; i <= 10000; ++i) {
            pid.update();
            accumulatedError += pid.error();
        }

        integratorValueWithoutAntiWindup = accumulatedError * (-10.0 / 2000);
        CHECK(integratorValueWithoutAntiWindup == Approx(-50.0).epsilon(0.05));
        CHECK(pid.p() == Approx(-10).epsilon(0.01));
        CHECK(pid.i() == Approx(0).margin(0.01)); // anti windup limits this to 0
        CHECK(pid.d() == Approx(0).margin(0.01));

        CHECK(actuator->setting() == Approx(-10).margin(0.01));
    }

    WHEN("The PID input is invalid for over 10 seconds, the actuator is set to invalid and the PID is inactive")
    {
        pid.kp(10);
        pid.ti(2000);
        pid.td(200);

        setpoint->setting(21);
        sensor->value(20);

        int32_t i = 0;
        for (; i <= 10000; ++i) {
            if (i == 2000) {
                sensor->connected(false);
            }
            pid.update();
            if (!pid.active()) {
                break;
            }
        }
        CHECK(i == 2010);
        CHECK(actuator->valid() == false);

        AND_WHEN("The input becomes valid again, the pid and actuatore become active again")
        {
            sensor->connected(true);
            pid.update();

            CHECK(pid.active() == true);
            CHECK(actuator->valid() == true);
        }
    }
}

SCENARIO("PID Test with offset actuator", "[pid]")
{
    auto targetSetpoint = std::make_shared<SetpointSimple>(65.0);
    auto targetSensor = std::make_shared<TempSensorMock>(65.0);

    auto referenceSensor = std::make_shared<TempSensorMock>(65.0);
    auto referenceSetpoint = std::make_shared<SetpointSimple>(67.0);

    auto target = std::make_shared<SetpointSensorPair>(
        [targetSetpoint]() { return targetSetpoint; },
        [targetSensor]() { return targetSensor; });

    auto reference = std::make_shared<SetpointSensorPair>(
        [referenceSetpoint]() { return referenceSetpoint; },
        [referenceSensor]() { return referenceSensor; });

    auto actuator = std::make_shared<ActuatorOffset>(
        [target]() { return target; },
        [reference]() { return reference; });

    auto pid = Pid(
        [&reference]() { return reference; },
        [&actuator]() { return actuator; });

    pid.enabled(true);

    pid.kp(2);
    pid.ti(0);
    pid.td(0);

    for (int32_t i = 0; i < 100; ++i) {
        pid.update(); // update 100 times to settle input filter
    }

    WHEN("The PID has updated, the target setpoint is set correctly")
    {
        CHECK(actuator->setting() == Approx(4.0).margin(0.01));
        CHECK(targetSetpoint->setting() == Approx(71.0).margin(0.01));
        CHECK(actuator->valid() == true);
    }

    WHEN("The PID input sensor becomes invalid")
    {
        referenceSensor->connected(false);
        pid.update();
        CHECK(actuator->valid() == false);
        THEN("The target setpoint is set to invalid after 10 failed updates")
        {
            for (uint8_t i = 0; i < 10; ++i) {
                CHECK(pid.active() == true);
                CHECK(targetSetpoint->valid() == true);
                pid.update();
            }

            CHECK(pid.active() == false);
            CHECK(targetSetpoint->valid() == false);
        }

        AND_WHEN("The sensor comes back alive, the pid and setpoint are active/valid again")
        {
            referenceSensor->connected(true);
            pid.update();

            CHECK(pid.active() == true);
            CHECK(targetSetpoint->valid() == true);
        }
    }
}