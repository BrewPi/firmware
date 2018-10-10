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

#include "../BrewBlox.h"
#include "ActuatorPin.test.pb.h"
#include "blox/ActuatorPinBlock.h"
#include "cbox/DataStreamIo.h"
#include <catch.hpp>
#include <sstream>

using namespace cbox;

SCENARIO("An ActuatorPinBlock")
{
    WHEN("a ActuatorPinBlock receives protobuf settings, the new settings match what was sent")
    {
        blox::ActuatorPin message;
        message.set_state(blox::AD_State::AD_State_Active);
        message.set_pin(1);
        message.set_invert(true);
        std::stringstream ssIn;

        message.SerializeToOstream(&ssIn);
        ssIn << '\0'; // zero terminate
        cbox::IStreamDataIn in(ssIn);

        ActuatorPinBlock act;
        auto res = act.streamFrom(in);
        CHECK(res == CboxError::OK);

        uint8_t pin = act.get().pin();
        bool invert = act.get().invert();
        ActuatorDigital::State state = act.get().state();
        CHECK(state == ActuatorDigital::State::Active);
        CHECK(pin == 1);
        CHECK(invert == true);

        AND_WHEN("an ActuatorPinBlock streams out protobuf settings, the output matches what was sent before")
        {
            std::stringstream ssOut;
            cbox::OStreamDataOut out(ssOut);

            auto res = act.streamTo(out);
            CHECK(res == CboxError::OK);

            blox::ActuatorPin round_trip;
            round_trip.ParseFromIstream(&ssOut);

            // state is active because invert is true
            CHECK(round_trip.ShortDebugString() == "state: Active "
                                                   "pin: 1 "
                                                   "invert: true");
        }
    }
}
