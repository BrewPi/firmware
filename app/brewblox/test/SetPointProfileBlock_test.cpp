/*
 * Copyright 2018 BrewPi
 *
 * This file is part of BrewPi.
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
#include "SetpointProfile.test.pb.h"
#include "blox/SetpointProfileBlock.h"
#include "cbox/DataStreamIo.h"
#include <catch.hpp>
#include <sstream>

using namespace cbox;

SCENARIO("A SetpointProfile block")
{
    WHEN("a SetpointProfileBlock receives new points")
    {
        blox::SetpointProfile message;
        {
            auto newPoint = message.add_points();
            newPoint->set_time(10);
            newPoint->set_temperature(cnl::unwrap(temp_t(20)));
        }

        {
            auto newPoint = message.add_points();
            newPoint->set_time(20);
            newPoint->set_temperature(cnl::unwrap(temp_t(21)));
        }

        std::stringstream ssIn;

        message.SerializeToOstream(&ssIn);
        ssIn << '\0'; // zero terminate
        cbox::IStreamDataIn in(ssIn);

        SetpointProfileBlock sp;
        CboxError res = sp.streamFrom(in);
        CHECK(res == CboxError::OK);

        setBootTime(1);
        sp.update(14000);
        temp_t setting = sp.get().setting();
        CHECK(setting == temp_t(20.5));

        AND_WHEN("a SetpointProfile block streams out protobuf settings, the output matches what was sent before")
        {
            std::stringstream ssOut;
            cbox::OStreamDataOut out(ssOut);

            CboxError res = sp.streamTo(out);
            CHECK(res == CboxError::OK);

            blox::SetpointProfile round_trip;
            round_trip.ParseFromIstream(&ssOut);

            CHECK(message.DebugString() == round_trip.DebugString());
            CHECK(round_trip.ShortDebugString() == "points { time: 10 temperature: 81920 } points { time: 20 temperature: 86016 }");
        }
    }
}
