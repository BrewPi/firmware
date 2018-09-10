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
#include "TempSensorOneWire.test.pb.h"
#include "blox/TempSensorOneWireBlock.h"
#include "cbox/DataStreamIo.h"
#include <catch.hpp>
#include <sstream>

using namespace cbox;

SCENARIO("A TempSensorOneWireBlock")
{
    WHEN("a TempSensorOneWireBlock receives protobuf settings")
    {
        blox::TempSensorOneWire message;
        message.set_address(0x12345678);
        message.set_offset(100);
        message.set_connected(true);
        message.set_value(123);
        std::stringstream ssIn;

        message.SerializeToOstream(&ssIn);
        ssIn << '\0'; // zero terminate
        cbox::IStreamDataIn in(ssIn);

        TempSensorOneWireBlock sensor;
        CboxError res = sensor.streamFrom(in);
        CHECK(res == CboxError::OK);

        THEN("The new setting match what was sent")
        {
            CHECK(sensor.get().getAddress() == 0x12345678);
            temp_t calibration = sensor.get().getCalibration();
            CHECK(to_rep(calibration) == 100);
        }
        THEN("The values that are not writable are ignored")
        {
            CHECK(sensor.get().read() == temp_t(0));
            CHECK(sensor.get().valid() == false);
        }

        AND_WHEN("a SetPointSimpleBlock streams out protobuf, the settings match what was sent before and the read only values are correct")
        {
            std::stringstream ssOut;
            cbox::OStreamDataOut out(ssOut);

            CboxError res = sensor.streamTo(out);
            CHECK(res == CboxError::OK);

            blox::TempSensorOneWire round_trip;
            round_trip.ParseFromIstream(&ssOut);

            auto correct = message;
            correct.set_value(0);
            correct.set_connected(false);

            CHECK(correct.DebugString() == round_trip.DebugString());
        }
    }
}
