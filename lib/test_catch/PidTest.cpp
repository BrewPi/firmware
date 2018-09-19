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

#include "Pid.h"

#include "ActuatorAnalogMock.h"
#include "Setpoint.h"
#include "SetpointSensorPair.h"
#include "TempSensorMock.h"
#include <iostream>
#include <math.h>

SCENARIO("PID Test")
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
        CHECK(pid.d() == Approx(10 * 9.0 / 900 * 200).epsilon(0.01));

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
        CHECK(pid.d() == Approx(10 * 9.0 / 900 * 200).epsilon(0.01));

        CHECK(actuator->setting() == pid.p() + pid.i() + pid.d());
    }

    WHEN("The actuator setting is clipped, the integrator is limited by anti-windup (positive kp)")
    {
        pid.kp(10);
        pid.ti(2000);
        pid.td(200);

        setpoint->setting(21);
        sensor->value(20);
        actuator->min(0);
        actuator->max(20);

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
        actuator->min(0);
        actuator->max(20);

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

        CHECK(actuator->setting() == Approx(5).margin(0.01));
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

        CHECK(actuator->setting() == Approx(5).margin(0.01));
    }
}
#if 0

BOOST_AUTO_TEST_CASE(error_is_invalid_and_actuator_zero_when_input_is_invalid_longer_than_10_s)
{
    auto sp = SetpointSimple(25.0); // setpoint is higher than temperature, actuator will heat
    auto sensor = TempSensorMock(20.0);
    auto pin = ActuatorBool();
    auto act = ActuatorPwm(pin, 4);
    auto input = SetpointSensorPair(sensor, sp);
    auto p = Pid(input, act);

    p.setConstants(10.0, 0.0, 0.0);
    p.update();
    BOOST_CHECK_EQUAL(act.setting(), temp_t(50.0)); // 10.0*(25.0-20.0)

    sensor.setConnected(false);
    p.update();

    // last values will be remembered during invalid input shorter than updates
    for (int i = 0; i < 20; i++) {
        p.update(); // is normally called every second
        if (i < 9) {
            // before being unavailable for 10 seconds
            BOOST_CHECK_EQUAL(p.state.error, temp_t(-5.0));
            BOOST_CHECK_EQUAL(act.setting(), temp_t(50.0)); // 10.0*(25.0-20.0)
        } else {
            // after being unavailable for 10 seconds
            BOOST_CHECK_EQUAL(p.state.error, temp_t::invalid()); // input error is marked as invalid
            BOOST_CHECK_EQUAL(act.setting(), temp_t(0.0));       // actuator is zero
        }
    }

    BOOST_CHECK_EQUAL(p.state.error, temp_t::invalid());
    BOOST_CHECK_EQUAL(act.setting(), temp_t(0.0));
}

BOOST_AUTO_TEST_CASE(pid_driving_setpoint_actuator)
{
    auto sp = SetpointSimple(25.0); // setpoint is higher than temperature, actuator will heat
    auto sensor = TempSensorMock(20.0);
    auto targetSensor = TempSensorMock(20.0);
    auto targetSetpoint = SetpointSimple(20.0);

    auto input = SetpointSensorPair(sensor, sp);
    auto target = SetpointSensorPair(targetSensor, targetSetpoint);

    auto act = ActuatorOffset(target, input);

    auto p = Pid(input, act);

    p.setConstants(2.0, 40, 0);
    p.update();

    // first check correct behavior under normal conditions
    // actuator value will be (sp-sensor)*kp = (25-20)*2 = 10;
    BOOST_CHECK_EQUAL(act.setting(), temp_t(10.0));

    // setpoint will be reference sp + actuator value = 35
    BOOST_CHECK_EQUAL(targetSetpoint.read(), temp_t(35.0));

    // achieved actuator value will be targetSensor - reference setpoint (sp) = 20.0 - 25.0
    BOOST_CHECK_EQUAL(act.value(), temp_t(-5.0));

    for (int i = 0; i < 10; i++) {
        p.update();
    }
    // integrator will stay at zero due to anti-windup (actuator is not reaching target)
    BOOST_CHECK_EQUAL(act.setting(), temp_t(10.0)); // still just proportional

    // but if target sensor is reaching value, the integrator will increase
    targetSensor.setTemp(35.0);
    p.update(); // integral will increase with p (10)
    p.update(); // integral is updated after setting output (lags 1 update), so do 2 updates

    BOOST_CHECK_EQUAL(act.setting(), temp_t(10.25)); // proportional (10) + integral (integral/Ti) (10/40=0.25)

    // now check how the pid responds to a disconnected target sensor
    targetSensor.setConnected(false);
    targetSensor.update();
    p.update();

    // setpoint will still be set, because this is what scales the actuators from (for example)
    // beer temp -> fridge temp setting -> actuators
    // the feedback of the actual fridge temp is lost, but the setpoint should still be set

    BOOST_CHECK_CLOSE(double(act.setting()), 10.5, 1); // +0.25 because of another actuator increase

    // setpoint will be reference sp + actuator value = 35.5
    BOOST_CHECK_CLOSE(double(targetSetpoint.read()), 35.5, 1);

    // achieved actuator value will be invalid
    BOOST_CHECK_EQUAL(act.value(), temp_t::invalid());
}

BOOST_AUTO_TEST_SUITE_END()
#endif
