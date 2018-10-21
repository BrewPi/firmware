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
#include "proto/test/cpp/ActuatorPin.test.pb.h"
#include "proto/test/cpp/DigitalConstraints.test.pb.h"

using namespace cbox;

SCENARIO("An ActuatorPinBlock")
{
    WHEN("a ActuatorPinBlock receives protobuf settings, the new settings match what was sent")
    {
        BrewBloxTestBox testBox;
        using commands = cbox::Box::CommandID;

        testBox.reset();

        // write to pin actuator (system object)
        testBox.put(commands::WRITE_OBJECT);
        testBox.put(cbox::obj_id_t(10));
        testBox.put(uint8_t(0xFF));
        testBox.put(ActuatorPinBlock::staticTypeId());

        auto newPin = blox::ActuatorPin();
        newPin.set_state(blox::AD_State_Active);
        newPin.set_invert(true);

        auto cPtr1 = newPin.mutable_constrainedby()->add_constraints();
        cPtr1->set_minoff(180);

        auto cPtr2 = newPin.mutable_constrainedby()->add_constraints();
        cPtr2->set_minon(120);

        testBox.put(newPin);

        testBox.processInput();
        CHECK(testBox.lastReplyHasStatusOk());

        testBox.put(commands::READ_OBJECT);
        testBox.put(cbox::obj_id_t(10));

        auto decoded = blox::ActuatorPin();
        testBox.processInputToProto(decoded);

        CHECK(testBox.lastReplyHasStatusOk());
        CHECK(decoded.ShortDebugString() == "state: Active "
                                            "pin: 17 "
                                            "invert: true "
                                            "constrainedBy { "
                                            "constraints { minOff: 180 } "
                                            "constraints { minOn: 120 } }");
    }
}
