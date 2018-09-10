/*
 * Copyright 2017 BrewPi
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
#include "SetPointSimple.test.pb.h"
//#include "blox/Block.h"
#include "blox/SetPointSimpleBlock.h"
#include <catch.hpp>
//#include "cbox/Box.h"
#include "cbox/DataStreamIo.h"
#include <sstream>
//#include "cbox/Object.h"
//#include "temperatureFormats.h"
//#include <cstdio>
//#include <memory>

using namespace cbox;

SCENARIO("A Bloc SetPointSimpleBlock")
{
    WHEN("a SetPointSimpleBlock receives protobuf settings, the new setting matches what was sent")
    {
        blox::SetPointSimple message;
        message.set_setting(123);
        std::stringstream ssIn;

        message.SerializeToOstream(&ssIn);
        ssIn << '\0'; // zero terminate
        cbox::IStreamDataIn in(ssIn);

        SetPointSimpleBlock sp;
        CboxError res = sp.streamFrom(in);
        CHECK(res == CboxError::OK);

        temp_t setting = sp.get().read();
        CHECK(to_base(setting) == 123);

        AND_WHEN("a SetPointSimpleBlock streams out protobuf settings, the output matches what was sent before")
        {
            std::stringstream ssOut;
            cbox::OStreamDataOut out(ssOut);

            CboxError res = sp.streamTo(out);
            CHECK(res == CboxError::OK);

            blox::SetPointSimple round_trip;
            round_trip.ParseFromIstream(&ssOut);

            CHECK(message.DebugString() == round_trip.DebugString());
        }
    }
}
