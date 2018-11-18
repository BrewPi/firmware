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
#include "blox/ActuatorAnalogMockBlock.h"
#include "blox/PidBlock.h"
#include "blox/SetpointSensorPairBlock.h"
#include "blox/SetpointSimpleBlock.h"
#include "blox/TempSensorMockBlock.h"
#include "proto/test/cpp/ActuatorAnalogMock.test.pb.h"
#include "proto/test/cpp/Pid.test.pb.h"
#include "proto/test/cpp/SetpointSensorPair.test.pb.h"
#include "proto/test/cpp/SetpointSimple.test.pb.h"
#include "proto/test/cpp/TempSensorMock.test.pb.h"

SCENARIO("A Blox Pid object can be created from streamed protobuf data")
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
    newSetpoint.set_valid(true);
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

    // create actuator
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(103));
    testBox.put(uint8_t(0xFF));
    testBox.put(ActuatorAnalogMockBlock::staticTypeId());

    blox::ActuatorAnalogMock newActuator;
    newActuator.set_setting(cnl::unwrap(ActuatorAnalog::value_t(0)));
    newActuator.set_minsetting(cnl::unwrap(ActuatorAnalog::value_t(0)));
    newActuator.set_maxsetting(cnl::unwrap(ActuatorAnalog::value_t(100)));
    newActuator.set_minvalue(cnl::unwrap(ActuatorAnalog::value_t(0)));
    newActuator.set_maxvalue(cnl::unwrap(ActuatorAnalog::value_t(100)));
    testBox.put(newActuator);

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // create Pid
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(104));
    testBox.put(uint8_t(0xFF));
    testBox.put(PidBlock::staticTypeId());

    blox::Pid newPid;
    newPid.set_inputid(102);
    newPid.set_outputid(103);
    newPid.set_filter(blox::Pid_FilterChoice::Pid_FilterChoice_FILT_30s);
    newPid.set_filterthreshold(cnl::unwrap(ActuatorAnalog::value_t(1)));
    newPid.set_enabled(true);
    newPid.set_kp(cnl::unwrap(Pid::in_t(10)));
    newPid.set_ti(2000);
    newPid.set_td(200);

    testBox.put(newPid);

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // update 100 times (PID updates every second, t is in ms)
    uint32_t t = 0;
    for (; t < 1000000; ++t) {
        testBox.update(t);
    }

    // read PID
    testBox.put(commands::READ_OBJECT);
    testBox.put(cbox::obj_id_t(104));

    auto decoded = blox::Pid();
    testBox.processInputToProto(decoded);
    CHECK(testBox.lastReplyHasStatusOk());

    CHECK(cnl::wrap<Pid::out_t>(decoded.p()) == Approx(10.0).epsilon(0.01));
    CHECK(cnl::wrap<Pid::out_t>(decoded.i()) == Approx(10.0 * 1.0 * 1000 / 2000).epsilon(0.05));
    CHECK(cnl::wrap<Pid::out_t>(decoded.d()) == 0);
    CHECK(cnl::wrap<Pid::out_t>(decoded.outputvalue()) == Approx(15.0).epsilon(0.05));

    // only nonzero values are shown in the debug string
    CHECK(decoded.ShortDebugString() == "inputId: 102 outputId: 103 "
                                        "inputValid: true outputValid: true "
                                        "inputValue: 81920 inputSetting: 86016 "
                                        "outputValue: 60518 outputSetting: 60518 "
                                        "filterThreshold: 4096 "
                                        "enabled: true active: true "
                                        "kp: 40960 ti: 2000 td: 200 "
                                        "p: 40950 i: 19568 "
                                        "error: 4095 integral: 19568 derivative: -1");
}
