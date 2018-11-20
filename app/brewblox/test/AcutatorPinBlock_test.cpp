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
        newPin.set_state(blox::AD_State_Inactive);
        newPin.set_invert(true);

        auto cPtr1 = newPin.mutable_constrainedby()->add_constraints();
        cPtr1->set_minoff(180000);

        auto cPtr2 = newPin.mutable_constrainedby()->add_constraints();
        cPtr2->set_minon(120000);

        testBox.put(newPin);

        testBox.processInput();
        CHECK(testBox.lastReplyHasStatusOk());

        testBox.put(commands::READ_OBJECT);
        testBox.put(cbox::obj_id_t(10));

        auto decoded = blox::ActuatorPin();
        testBox.processInputToProto(decoded);

        CHECK(testBox.lastReplyHasStatusOk());
        CHECK(decoded.ShortDebugString() == "invert: true "
                                            "constrainedBy { "
                                            "constraints { minOff: 180000 } "
                                            "constraints { minOn: 120000 } }");

        // write state inactive
        auto writeState = [&testBox, &decoded](blox::AD_State state) {
            testBox.put(commands::WRITE_OBJECT);
            testBox.put(cbox::obj_id_t(10));
            testBox.put(uint8_t(0xFF));
            testBox.put(ActuatorPinBlock::staticTypeId());

            decoded.set_state(state);
            testBox.put(decoded);

            decoded = blox::ActuatorPin();

            testBox.processInputToProto(decoded);

            CHECK(testBox.lastReplyHasStatusOk());
        };

        // will refuse to turn ON
        writeState(blox::AD_State_Active);
        CHECK(decoded.ShortDebugString() == "invert: true "
                                            "constrainedBy { "
                                            "constraints { minOff: 180000 } "
                                            "constraints { minOn: 120000 } "
                                            "blocking: 1 }");

        testBox.update(200000);

        // will turn ON
        writeState(blox::AD_State_Active);
        CHECK(decoded.ShortDebugString() == "state: Active "
                                            "invert: true "
                                            "constrainedBy { "
                                            "constraints { minOff: 180000 } "
                                            "constraints { minOn: 120000 } }");
        testBox.update(201000);

        // will refuse to turn OFF
        writeState(blox::AD_State_Inactive);
        CHECK(decoded.ShortDebugString() == "state: Active "
                                            "invert: true "
                                            "constrainedBy { "
                                            "constraints { minOff: 180000 } "
                                            "constraints { minOn: 120000 } "
                                            "blocking: 2 }");

        testBox.update(500000);

        // will turn OFF
        writeState(blox::AD_State_Inactive);
        CHECK(decoded.ShortDebugString() == "invert: true "
                                            "constrainedBy { "
                                            "constraints { minOff: 180000 } "
                                            "constraints { minOn: 120000 } }");
    }
}
