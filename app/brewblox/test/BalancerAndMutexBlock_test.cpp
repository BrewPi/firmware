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
#include <cstdio>
#include <iomanip>
#include <iostream>

#include "../BrewBlox.h"
#include "Temperature.h"
#include "blox/ActuatorPinBlock.h"
#include "blox/ActuatorPwmBlock.h"
#include "blox/BalancerBlock.h"
#include "blox/MutexBlock.h"
#include "cbox/Box.h"
#include "cbox/DataStream.h"
#include "cbox/DataStreamIo.h"
#include "cbox/Object.h"
#include "proto/test/cpp/ActuatorPin.test.pb.h"
#include "proto/test/cpp/ActuatorPwm.test.pb.h"
#include "proto/test/cpp/Balancer.test.pb.h"
#include "proto/test/cpp/Mutex.test.pb.h"
#include "testHelpers.h"

SCENARIO("Two PWM actuators can be constrained by a balancer")
{
    static auto& box = brewbloxBox();
    static auto& connSource = testConnectionSource();
    using commands = cbox::Box::CommandID;

    std::stringstream in, out, expected;

    // use some helpers to create the data commands
    cbox::OStreamDataOut inOs(in);
    cbox::BinaryToHexTextOut toHex(inOs);
    cbox::HexCrcDataOut inEncoder(toHex);
    ProtoDataOut inProto(inEncoder);

    connSource.add(in, out);

    auto clearStreams = [&in, &out, &expected]() {
        in.str("");
        in.clear();
        out.str("");
        out.clear();
        expected.str("");
        expected.clear();
    };

    // clear objects
    clearStreams();
    inEncoder.put(commands::CLEAR_OBJECTS);
    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    box.update(0); // ensure last update is at 0 and not influenced by other tests

    // create balancer
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(100));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(BalancerBlock::staticTypeId());
    {
        auto newBalancer = blox::Balancer();
        inProto.put(newBalancer);
    }
    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create mutex
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(101));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(MutexBlock::staticTypeId());
    {
        auto newMutex = blox::Mutex();
        newMutex.set_differentactuatorwait(100);
        inProto.put(newMutex);
    }
    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create pin actuator 1
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(200));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(ActuatorPinBlock::staticTypeId());
    {
        auto newPin = blox::ActuatorPin();
        newPin.set_pin(0);
        newPin.set_state(blox::AD_State_Active);
        newPin.set_invert(false);
        auto constraintPtr = newPin.mutable_constrainedby()->add_constraints();
        constraintPtr->set_mutex(101);
        inProto.put(newPin);
    }

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create pwm actuator 1
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(201));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(ActuatorPwmBlock::staticTypeId());

    {
        auto newPwm = blox::ActuatorPwm();
        newPwm.set_actuatorid(200);
        newPwm.set_setting(cnl::unwrap(ActuatorAnalog::value_t(80)));
        newPwm.set_period(4000);

        auto c = newPwm.mutable_constrainedby()->add_constraints();
        c->set_balancer(100);

        inProto.put(newPwm);
    }
    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create pin actuator 2
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(300));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(ActuatorPinBlock::staticTypeId());

    {
        auto newPin = blox::ActuatorPin();
        newPin.set_pin(0);
        newPin.set_state(blox::AD_State_Active);
        newPin.set_invert(false);
        inProto.put(newPin);
        auto constraintPtr = newPin.mutable_constrainedby()->add_constraints();
        constraintPtr->set_mutex(101);
        inProto.put(newPin);
    }

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create pwm actuator 2
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(301));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(ActuatorPwmBlock::staticTypeId());

    {
        auto newPwm = blox::ActuatorPwm();
        newPwm.set_actuatorid(300);
        newPwm.set_setting(cnl::unwrap(ActuatorAnalog::value_t(80)));
        newPwm.set_period(4000);

        auto c = newPwm.mutable_constrainedby()->add_constraints();
        c->set_balancer(100);

        inProto.put(newPwm);
    }

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    box.update(0);
    box.update(1000);

    // read balancer
    clearStreams();
    inEncoder.put(commands::READ_OBJECT);
    inEncoder.put(cbox::obj_id_t(100));

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors
    {
        auto reply = blox::Balancer();
        decodeProtoFromReply(out, reply);
        CHECK(reply.ShortDebugString() == "clients { requested: 327680 granted: 204800 } "  // 80*4096, 50*4096
                                          "clients { requested: 327680 granted: 204800 }"); // 80*4096, 50*4096
    }

    // read mutex
    clearStreams();
    inEncoder.put(commands::READ_OBJECT);
    inEncoder.put(cbox::obj_id_t(101));

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors
    {
        auto reply = blox::Mutex();
        decodeProtoFromReply(out, reply);
        CHECK(reply.ShortDebugString() == "differentActuatorWait: 100");
    }

    // read a pin actuator
    clearStreams();
    inEncoder.put(commands::READ_OBJECT);
    inEncoder.put(cbox::obj_id_t(200));

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors
    {
        auto reply = blox::ActuatorPin();
        decodeProtoFromReply(out, reply);
        CHECK(reply.ShortDebugString() == "state: Active constrainedBy { constraints { mutex: 101 } }");
    }

    // read a pwm actuator
    clearStreams();
    inEncoder.put(commands::READ_OBJECT);
    inEncoder.put(cbox::obj_id_t(201));

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors
    {
        auto reply = blox::ActuatorPwm();
        decodeProtoFromReply(out, reply);
        CHECK(reply.ShortDebugString() == "actuatorId: 200 actuatorValid: true "
                                          "period: 4000 setting: 327680 "
                                          "constrainedBy { constraints { balancer: 100 } }");
    }
}
