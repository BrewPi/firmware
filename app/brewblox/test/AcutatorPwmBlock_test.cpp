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
#include "cbox/Box.h"
#include "cbox/DataStream.h"
#include "cbox/DataStreamIo.h"
#include "cbox/Object.h"
#include "proto/test/cpp/ActuatorPin.test.pb.h"
#include "proto/test/cpp/ActuatorPwm.test.pb.h"
#include "testHelpers.h"

SCENARIO("A Blox ActuatorPwm object can be created from streamed protobuf data")
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

    // create pin actuator
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(100));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(ActuatorPinBlock::staticTypeId());

    auto newPin = blox::ActuatorPin();
    newPin.set_pin(0);
    newPin.set_state(blox::AD_State_Active);
    newPin.set_invert(false);
    inProto.put(newPin);

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create pwm actuator
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(101));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(ActuatorPwmBlock::staticTypeId());

    blox::ActuatorPwm newPwm;
    newPwm.set_actuatorid(100);
    newPwm.set_setting(20);
    newPwm.set_period(4000);
    inProto.put(newPwm);

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // read pwm
    clearStreams();
    inEncoder.put(commands::READ_OBJECT);
    inEncoder.put(cbox::obj_id_t(101));

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    blox::ActuatorPwm reply;
    decodeProtoFromReply(out, reply);
    CHECK(reply.ShortDebugString() == "actuatorId: 100 actuatorValid: true period: 4000 setting: 20 value: 20");
}
