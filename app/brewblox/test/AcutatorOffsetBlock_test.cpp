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
#include "blox/ActuatorOffsetBlock.h"
#include "blox/SetpointSensorPairBlock.h"
#include "blox/SetpointSimpleBlock.h"
#include "blox/TempSensorMockBlock.h"
#include "cbox/Box.h"
#include "cbox/DataStream.h"
#include "cbox/DataStreamIo.h"
#include "cbox/Object.h"
#include "proto/test/cpp/ActuatorOffset.test.pb.h"
#include "proto/test/cpp/SetpointSensorPair.test.pb.h"
#include "proto/test/cpp/SetpointSimple.test.pb.h"
#include "proto/test/cpp/TempSensorMock.test.pb.h"
#include "testHelpers.h"

SCENARIO("A Blox ActuatorOffset object can be created from streamed protobuf data")
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

    // create mock sensor 1
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(100));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(TempSensorMockBlock::staticTypeId());

    auto newSensor1 = blox::TempSensorMock();
    newSensor1.set_value(cnl::unwrap(temp_t(21.0)));
    newSensor1.set_connected(true);
    inProto.put(newSensor1);

    inEncoder.endMessage();

    box.hexCommunicate();
    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create setpoint 1
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(101));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(SetpointSimpleBlock::staticTypeId());

    auto newSetpoint1 = blox::SetpointSimple();
    newSetpoint1.set_setting(cnl::unwrap(temp_t(20.0)));
    inProto.put(newSetpoint1);

    inEncoder.endMessage();

    box.hexCommunicate();
    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create pair 1 (target)
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(102));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(SetpointSensorPairBlock::staticTypeId());

    auto newPair1 = blox::SetpointSensorPair();
    newPair1.set_sensorid(100);
    newPair1.set_setpointid(101);
    inProto.put(newPair1);

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create mock sensor 2
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(103));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(TempSensorMockBlock::staticTypeId());

    auto newSensor2 = blox::TempSensorMock();
    newSensor2.set_value(cnl::unwrap(temp_t(27.0)));
    newSensor2.set_connected(true);
    inProto.put(newSensor2);

    inEncoder.endMessage();

    box.hexCommunicate();
    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create setpoint 2
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(104));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(SetpointSimpleBlock::staticTypeId());

    auto newSetpoint2 = blox::SetpointSimple();
    newSetpoint2.set_setting(cnl::unwrap(temp_t(20.0)));
    inProto.put(newSetpoint2);

    inEncoder.endMessage();

    box.hexCommunicate();
    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create pair 2 (reference)
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(105));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(SetpointSensorPairBlock::staticTypeId());

    auto newPair2 = blox::SetpointSensorPair();
    newPair2.set_sensorid(103);
    newPair2.set_setpointid(104);
    inProto.put(newPair2);

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create actuator
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(106));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(ActuatorOffsetBlock::staticTypeId());

    blox::ActuatorOffset newAct;
    newAct.set_targetid(102);
    newAct.set_referenceid(105);
    newAct.set_referencesettingorvalue(blox::ActuatorOffset_SettingOrValue(ActuatorOffset::SettingOrValue::SETTING));
    newAct.set_setting(cnl::unwrap(ActuatorAnalog::value_t(12)));

    inProto.put(newAct);

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // read actuator
    clearStreams();
    inEncoder.put(commands::READ_OBJECT);
    inEncoder.put(cbox::obj_id_t(106));

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors
    {
        auto reply = blox::ActuatorOffset();
        decodeProtoFromReply(out, reply);
        CHECK(reply.ShortDebugString() == "targetId: 102 targetValid: true referenceId: 105 referenceValid: true "
                                          "setting: 49152 value: 4096"); // setting is 12 (setpoint difference), value is 1 (21 - 20)
    }

    // read reference pair
    clearStreams();
    inEncoder.put(commands::READ_OBJECT);
    inEncoder.put(cbox::obj_id_t(102));

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    {
        auto reply = blox::SetpointSensorPair();
        decodeProtoFromReply(out, reply);
        CHECK(reply.ShortDebugString() == "setpointId: 101 sensorId: 100 setpointValid: true sensorValid: true "
                                          "setpointValue: 131072 sensorValue: 86016"); // 32, 21 (setpoint adjusted to 20 + 12)
    }

    // read target pair
    clearStreams();
    inEncoder.put(commands::READ_OBJECT);
    inEncoder.put(cbox::obj_id_t(105));

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    {
        auto reply = blox::SetpointSensorPair();
        decodeProtoFromReply(out, reply);
        CHECK(reply.ShortDebugString() == "setpointId: 104 sensorId: 103 setpointValid: true sensorValid: true "
                                          "setpointValue: 81920 sensorValue: 110592"); // 20, 27 (unaffected)
    }
}
