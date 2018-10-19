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
#include "Temperature.h"
#include "blox/ActuatorOffsetBlock.h"
#include "blox/SetpointSensorPairBlock.h"
#include "blox/SetpointSimpleBlock.h"
#include "blox/TempSensorMockBlock.h"
#include "proto/test/cpp/ActuatorOffset.test.pb.h"
#include "proto/test/cpp/SetpointSensorPair.test.pb.h"
#include "proto/test/cpp/SetpointSimple.test.pb.h"
#include "proto/test/cpp/TempSensorMock.test.pb.h"

SCENARIO("A Blox ActuatorOffset object can be created from streamed protobuf data")
{
    BrewBloxTestBox testBox;
    using commands = cbox::Box::CommandID;

    testBox.reset();

    // create mock sensor 1
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(100));
    testBox.put(uint8_t(0xFF));
    testBox.put(TempSensorMockBlock::staticTypeId());

    auto newSensor1 = blox::TempSensorMock();
    newSensor1.set_value(cnl::unwrap(temp_t(21.0)));
    newSensor1.set_connected(true);
    testBox.put(newSensor1);

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // create setpoint 1
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(101));
    testBox.put(uint8_t(0xFF));
    testBox.put(SetpointSimpleBlock::staticTypeId());

    auto newSetpoint1 = blox::SetpointSimple();
    newSetpoint1.set_setting(cnl::unwrap(temp_t(20.0)));
    testBox.put(newSetpoint1);

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // create pair 1 (target)
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(102));
    testBox.put(uint8_t(0xFF));
    testBox.put(SetpointSensorPairBlock::staticTypeId());

    auto newPair1 = blox::SetpointSensorPair();
    newPair1.set_sensorid(100);
    newPair1.set_setpointid(101);
    testBox.put(newPair1);

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // create mock sensor 2
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(103));
    testBox.put(uint8_t(0xFF));
    testBox.put(TempSensorMockBlock::staticTypeId());

    auto newSensor2 = blox::TempSensorMock();
    newSensor2.set_value(cnl::unwrap(temp_t(27.0)));
    newSensor2.set_connected(true);
    testBox.put(newSensor2);

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // create setpoint 2
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(104));
    testBox.put(uint8_t(0xFF));
    testBox.put(SetpointSimpleBlock::staticTypeId());

    auto newSetpoint2 = blox::SetpointSimple();
    newSetpoint2.set_setting(cnl::unwrap(temp_t(20.0)));
    testBox.put(newSetpoint2);

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // create pair 2 (reference)
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(105));
    testBox.put(uint8_t(0xFF));
    testBox.put(SetpointSensorPairBlock::staticTypeId());

    auto newPair2 = blox::SetpointSensorPair();
    newPair2.set_sensorid(103);
    newPair2.set_setpointid(104);
    testBox.put(newPair2);

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // create actuator
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(106));
    testBox.put(uint8_t(0xFF));
    testBox.put(ActuatorOffsetBlock::staticTypeId());

    blox::ActuatorOffset newAct;
    newAct.set_targetid(102);
    newAct.set_referenceid(105);
    newAct.set_referencesettingorvalue(blox::ActuatorOffset_SettingOrValue(ActuatorOffset::SettingOrValue::SETTING));
    newAct.set_setting(cnl::unwrap(ActuatorAnalog::value_t(12)));

    testBox.put(newAct);

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // read actuator
    testBox.put(commands::READ_OBJECT);
    testBox.put(cbox::obj_id_t(106));
    {
        auto decoded = blox::ActuatorOffset();
        testBox.processInputToProto(decoded);
        CHECK(testBox.lastReplyHasStatusOk());
        CHECK(decoded.ShortDebugString() == "targetId: 102 targetValid: true referenceId: 105 referenceValid: true "
                                            "setting: 49152 value: 4096"); // setting is 12 (setpoint difference), value is 1 (21 - 20)
    }

    // read reference pair
    testBox.put(commands::READ_OBJECT);
    testBox.put(cbox::obj_id_t(102));

    {
        auto decoded = blox::SetpointSensorPair();
        testBox.processInputToProto(decoded);
        CHECK(testBox.lastReplyHasStatusOk());
        CHECK(decoded.ShortDebugString() == "setpointId: 101 sensorId: 100 setpointValid: true sensorValid: true "
                                            "setpointValue: 131072 sensorValue: 86016"); // 32, 21 (setpoint adjusted to 20 + 12)
    }

    // read target pair
    testBox.put(commands::READ_OBJECT);
    testBox.put(cbox::obj_id_t(105));

    {
        auto decoded = blox::SetpointSensorPair();
        testBox.processInputToProto(decoded);
        CHECK(testBox.lastReplyHasStatusOk());
        CHECK(decoded.ShortDebugString() == "setpointId: 104 sensorId: 103 setpointValid: true sensorValid: true "
                                            "setpointValue: 81920 sensorValue: 110592"); // 20, 27 (unaffected)
    }
}
