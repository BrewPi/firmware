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
#include "proto/test/cpp/ActuatorPin.test.pb.h"
#include "proto/test/cpp/ActuatorPwm.test.pb.h"

SCENARIO("A Blox ActuatorPwm object can be created from streamed protobuf data")
{
    BrewBloxTestBox testBox;
    using commands = cbox::Box::CommandID;

    testBox.reset();

    // create pwm actuator
    testBox.put(commands::CREATE_OBJECT);
    testBox.put(cbox::obj_id_t(101));
    testBox.put(uint8_t(0xFF));
    testBox.put(ActuatorPwmBlock::staticTypeId());

    blox::ActuatorPwm newPwm;
    newPwm.set_actuatorid(10); // predefined system object for pin actuator
    newPwm.set_setting(cnl::unwrap(ActuatorAnalog::value_t(20)));
    newPwm.set_period(4000);

    auto c = newPwm.mutable_constrainedby()->add_constraints();
    c->set_min(cnl::unwrap(ActuatorAnalog::value_t(10)));

    testBox.put(newPwm);

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // read pwm
    testBox.put(commands::READ_OBJECT);
    testBox.put(cbox::obj_id_t(101));

    auto decoded = blox::ActuatorPwm();
    testBox.processInputToProto(decoded);

    CHECK(testBox.lastReplyHasStatusOk());
    CHECK(decoded.ShortDebugString() == "actuatorId: 10 actuatorValid: true "
                                        "period: 4000 setting: 81920 "
                                        "constrainedBy { constraints { min: 40960 } }");
}