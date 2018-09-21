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

#include "cbox/Box.h"
#include "cbox/Object.h"

#include "../BrewBlox.h"
#include "Temperature.h"
#include "blox/ActuatorAnalogMockBlock.h"
#include "blox/PidBlock.h"
#include "blox/SetpointSensorPairBlock.h"
#include "blox/SetpointSimpleBlock.h"
#include "blox/TempSensorMockBlock.h"
#include "cbox/Box.h"
#include "cbox/DataStream.h"
#include "cbox/DataStreamIo.h"
#include "cbox/Object.h"
#include "proto/test/cpp/ActuatorAnalogMock.test.pb.h"
#include "proto/test/cpp/Pid.test.pb.h"
#include "proto/test/cpp/SetpointSensorPair.test.pb.h"
#include "proto/test/cpp/SetpointSimple.test.pb.h"
#include "proto/test/cpp/TempSensorMock.test.pb.h"
#include "testHelpers.h"

SCENARIO("A Blox Pid object can be created from streamed protobuf data")
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

    // create actuator
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(103));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(ActuatorAnalogMockBlock::staticTypeId());

    blox::ActuatorAnalogMock newActuator;
    newActuator.set_setting(cnl::unwrap(ActuatorAnalog::value_t(0)));
    newActuator.set_minsetting(cnl::unwrap(ActuatorAnalog::value_t(0)));
    newActuator.set_maxsetting(cnl::unwrap(ActuatorAnalog::value_t(100)));
    newActuator.set_minvalue(cnl::unwrap(ActuatorAnalog::value_t(0)));
    newActuator.set_maxvalue(cnl::unwrap(ActuatorAnalog::value_t(100)));
    inProto.put(newActuator);

    inEncoder.endMessage();
    box.hexCommunicate();
    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // create Pid
    clearStreams();
    inEncoder.put(commands::CREATE_OBJECT);
    inEncoder.put(cbox::obj_id_t(104));
    inEncoder.put(uint8_t(0xFF));
    inEncoder.put(PidBlock::staticTypeId());

    blox::Pid newPid;
    newPid.set_inputid(102);
    newPid.set_outputid(103);
    newPid.set_filter(blox::Pid_FilterChoice::Pid_FilterChoice_FILT_30s);
    newPid.set_filterthreshold(cnl::unwrap(ActuatorAnalog::value_t(1)));
    newPid.set_enabled(true);
    newPid.set_kp(cnl::unwrap(Pid::in_t(10)));
    newPid.set_ti(2000);
    newPid.set_td(200);

    inProto.put(newPid);

    inEncoder.endMessage();
    box.hexCommunicate();
    CHECK(out.str().find("|00") != std::string::npos); // no errors

    // update 100 times (PID updates every second, t is in ms)
    uint32_t t = 0;
    for (; t < 1000000; ++t) {
        box.update(t);
    }

    // read PID
    clearStreams();
    inEncoder.put(commands::READ_OBJECT);
    inEncoder.put(cbox::obj_id_t(104));

    inEncoder.endMessage();
    box.hexCommunicate();

    CHECK(out.str().find("|00") != std::string::npos); // no errors

    blox::Pid reply;
    decodeProtoFromReply(out, reply);

    CHECK(cnl::wrap<Pid::out_t>(reply.p()) == Approx(10.0).epsilon(0.01));
    CHECK(cnl::wrap<Pid::out_t>(reply.i()) == Approx(10.0 * 1.0 * 1000 / 2000).epsilon(0.05));
    CHECK(cnl::wrap<Pid::out_t>(reply.d()) == 0);
    CHECK(cnl::wrap<Pid::out_t>(reply.outputvalue()) == Approx(15.0).epsilon(0.05));

    // only nonzero values are shown in the debug string
    CHECK(reply.ShortDebugString() == "inputId: 102 outputId: 103 "
                                      "inputValid: true outputValid: true "
                                      "inputValue: 81920 inputSetting: 86016 "
                                      "outputValue: 60518 outputSetting: 60518 "
                                      "filterThreshold: 4096 "
                                      "enabled: true active: true "
                                      "kp: 40960 ti: 2000 td: 200 "
                                      "p: 40950 i: 19568 "
                                      "error: 4095 integral: 19568 derivative: -1");
}
