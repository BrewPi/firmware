/*
 * Copyright 2018 BrewPi B.V.
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

#include "BrewBloxTestBox.h"
#include "blox/ActuatorPinBlock.h"
#include "blox/ActuatorPwmBlock.h"
#include "blox/BalancerBlock.h"
#include "blox/MutexBlock.h"
#include "proto/test/cpp/ActuatorPin.test.pb.h"
#include "proto/test/cpp/ActuatorPwm.test.pb.h"
#include "proto/test/cpp/Balancer.test.pb.h"
#include "proto/test/cpp/Mutex.test.pb.h"

SCENARIO("Two PWM actuators can be constrained by a balancer")
{
    BrewBloxTestBox testBox;
    using commands = cbox::Box::CommandID;

    testBox.reset();

    // create balancer
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(100));
    testBox.put(uint8_t(0xFF));
    testBox.put(BalancerBlock::staticTypeId());
    {
        auto newBalancer = blox::Balancer();
        testBox.put(newBalancer);
    }
    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // create mutex
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(101));
    testBox.put(uint8_t(0xFF));
    testBox.put(MutexBlock::staticTypeId());
    {
        auto newMutex = blox::Mutex();
        newMutex.set_differentactuatorwait(100);
        testBox.put(newMutex);
    }
    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // configure pin actuator 1
    testBox.put(commands::WRITE_OBJECT);
    testBox.put(cbox::obj_id_t(10)); // system object 11 is a predefined pin actuator
    testBox.put(uint8_t(0xFF));
    testBox.put(ActuatorPinBlock::staticTypeId());
    {
        auto newPin = blox::ActuatorPin();
        newPin.set_state(blox::AD_State_Active);
        newPin.set_invert(false);
        auto constraintPtr = newPin.mutable_constrainedby()->add_constraints();
        constraintPtr->set_mutex(101);
        testBox.put(newPin);
    }

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // create pwm actuator 1
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(201));
    testBox.put(uint8_t(0xFF));
    testBox.put(ActuatorPwmBlock::staticTypeId());

    {
        auto newPwm = blox::ActuatorPwm();
        newPwm.set_actuatorid(10);
        newPwm.set_setting(cnl::unwrap(ActuatorAnalog::value_t(80)));
        newPwm.set_period(4000);

        auto c = newPwm.mutable_constrainedby()->add_constraints();
        c->set_balancer(100);

        testBox.put(newPwm);
    }
    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // configure pin actuator 2
    testBox.put(commands::WRITE_OBJECT);
    testBox.put(cbox::obj_id_t(11)); // system object 11 is a predefined pin actuator
    testBox.put(uint8_t(0xFF));
    testBox.put(ActuatorPinBlock::staticTypeId());

    {
        auto newPin = blox::ActuatorPin();
        newPin.set_state(blox::AD_State_Active);
        newPin.set_invert(false);
        auto constraintPtr = newPin.mutable_constrainedby()->add_constraints();
        constraintPtr->set_mutex(101);
        testBox.put(newPin);
    }

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // create pwm actuator 2
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(301));
    testBox.put(uint8_t(0xFF));
    testBox.put(ActuatorPwmBlock::staticTypeId());

    {
        auto newPwm = blox::ActuatorPwm();
        newPwm.set_actuatorid(11);
        newPwm.set_setting(cnl::unwrap(ActuatorAnalog::value_t(80)));
        newPwm.set_period(4000);

        auto c = newPwm.mutable_constrainedby()->add_constraints();
        c->set_balancer(100);

        testBox.put(newPwm);
    }

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    testBox.update(0);
    testBox.update(1000);

    // read balancer
    testBox.put(commands::READ_OBJECT);
    testBox.put(cbox::obj_id_t(100));

    {
        auto decoded = blox::Balancer();
        testBox.processInputToProto(decoded);
        CHECK(testBox.lastReplyHasStatusOk());
        CHECK(decoded.ShortDebugString() == "clients { requested: 327680 granted: 204800 } "  // 80*4096, 50*4096
                                            "clients { requested: 327680 granted: 204800 }"); // 80*4096, 50*4096
    }

    // read mutex
    testBox.put(commands::READ_OBJECT);
    testBox.put(cbox::obj_id_t(101));

    {
        auto decoded = blox::Mutex();
        testBox.processInputToProto(decoded);
        CHECK(testBox.lastReplyHasStatusOk());
        CHECK(decoded.ShortDebugString() == "differentActuatorWait: 100");
    }

    // read a pin actuator
    testBox.put(commands::READ_OBJECT);
    testBox.put(cbox::obj_id_t(10));

    {
        auto decoded = blox::ActuatorPin();
        testBox.processInputToProto(decoded);
        CHECK(testBox.lastReplyHasStatusOk());
        CHECK(decoded.ShortDebugString() == "state: Active constrainedBy { constraints { mutex: 101 } }");
    }

    // read a pwm actuator
    testBox.put(commands::READ_OBJECT);
    testBox.put(cbox::obj_id_t(201));

    {
        auto decoded = blox::ActuatorPwm();
        testBox.processInputToProto(decoded);
        CHECK(testBox.lastReplyHasStatusOk());
        CHECK(decoded.ShortDebugString() == "actuatorId: 10 actuatorValid: true "
                                            "period: 4000 setting: 327680 "
                                            "constrainedBy { constraints { balancer: 100 } }");
    }
}
