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
#include "BrewBloxTestBox.h"
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
    BrewBloxTestBox testBox;
    using commands = cbox::Box::CommandID;

    testBox.reset();

    // create mock sensor
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(100));
    testBox.put(uint8_t(0xFF));
    testBox.put(TempSensorMockBlock::staticTypeId());

    auto newSensor = blox::TempSensorMock();
    newSensor.set_value(cnl::unwrap(temp_t(20.0)));
    newSensor.set_connected(true);
    testBox.put(newSensor);

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // create setpoint
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(101));
    testBox.put(uint8_t(0xFF));
    testBox.put(SetpointSimpleBlock::staticTypeId());

    blox::SetpointSimple newSetpoint;
    newSetpoint.set_setting(cnl::unwrap(temp_t(21.0)));
    testBox.put(newSetpoint);

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // create pair
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(102));
    testBox.put(uint8_t(0xFF));
    testBox.put(SetpointSensorPairBlock::staticTypeId());

    blox::SetpointSensorPair newPair;
    newPair.set_sensorid(100);
    newPair.set_setpointid(101);
    testBox.put(newPair);

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // read pair
    testBox.put(commands::READ_OBJECT);
    testBox.put(cbox::obj_id_t(102));

    auto decoded = blox::SetpointSensorPair();
    testBox.processInputToProto(decoded);
    CHECK(testBox.lastReplyHasStatusOk());
    CHECK(decoded.ShortDebugString() == "setpointId: 101 sensorId: 100 "
                                        "setpointValid: true sensorValid: true "
                                        "setpointValue: 86016 sensorValue: 81920");
}
