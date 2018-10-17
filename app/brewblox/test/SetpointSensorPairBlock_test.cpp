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
#include "blox/SetpointSensorPairBlock.h"
#include "blox/SetpointSimpleBlock.h"
#include "blox/TempSensorMockBlock.h"
#include "cbox/Box.h"
#include "cbox/DataStream.h"
#include "cbox/DataStreamIo.h"
#include "cbox/Object.h"
#include "proto/test/cpp/SetpointSensorPair.test.pb.h"
#include "proto/test/cpp/SetpointSimple.test.pb.h"
#include "proto/test/cpp/TempSensorMock.test.pb.h"
#include "testHelpers.h"

SCENARIO("A Blox SetpointSensorPair object can be created from streamed protobuf data")
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

    // create mock sensor
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(100));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(TempSensorMockBlock::staticTypeId());

    auto newSensor = blox::TempSensorMock();
    newSensor.set_value(cnl::unwrap(temp_t(20.0)));
    newSensor.set_connected(true);
    inProto.put(newSensor);

    inEncoder.endMessage();

    box.hexCommunicate();
    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create setpoint
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(101));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(SetpointSimpleBlock::staticTypeId());

    blox::SetpointSimple newSetpoint;
    newSetpoint.set_setting(cnl::unwrap(temp_t(21.0)));
    inProto.put(newSetpoint);

    inEncoder.endMessage();

    box.hexCommunicate();
    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create pair
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(102));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(SetpointSensorPairBlock::staticTypeId());

    blox::SetpointSensorPair newPair;
    newPair.set_sensorid(100);
    newPair.set_setpointid(101);
    inProto.put(newPair);

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // read pair
    clearStreams();
    inEncoder.put(commands::READ_OBJECT);
    inEncoder.put(cbox::obj_id_t(102));

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    blox::SetpointSensorPair reply;
    decodeProtoFromReply(out, reply);
    CHECK(reply.ShortDebugString() == "setpointId: 101 sensorId: 100 "
                                      "setpointValid: true sensorValid: true "
                                      "setpointValue: 86016 sensorValue: 81920");
}
