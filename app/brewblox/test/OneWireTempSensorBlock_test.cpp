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
#include "OneWireTempSensor.test.pb.h"
#include "blox/OneWireTempSensorBlock.h"
#include <catch.hpp>
#include "cbox/DataStreamIo.h"
#include <sstream>

using namespace cbox;

SCENARIO("A OneWireTempSensorBlock")
{
    WHEN("a OneWireTempSensorBlock receives protobuf settings")
    {
        blox::OneWireTempSensor message;
        message.set_address(0x12345678);
        message.set_offset(100);
        message.set_connected(true);
        message.set_value(123);
        std::stringstream ssIn;

        message.SerializeToOstream(&ssIn);
        ssIn << '\0'; // zero terminate
        cbox::IStreamDataIn in(ssIn);

        OneWireTempSensorBlock sensor;
        CboxError res = sensor.streamFrom(in);
        CHECK(res == CboxError::OK);

        THEN("The new setting match what was sent")
        {
            CHECK(sensor.get().getAddress() == 0x12345678);
            CHECK(sensor.get().getCalibration() == temp_t::raw(100));
        }
        THEN("The values that are not writable are ignored")
        {
            CHECK(sensor.get().read() == TEMP_SENSOR_DISCONNECTED);
            CHECK(sensor.get().isConnected() == false);
        }

        AND_WHEN("a SetPointSimpleBlock streams out protobuf, the settings match what was sent before and the read only values are correct")
        {
            std::stringstream ssOut;
            cbox::OStreamDataOut out(ssOut);

            CboxError res = sensor.streamTo(out);
            CHECK(res == CboxError::OK);

            blox::OneWireTempSensor round_trip;
            round_trip.ParseFromIstream(&ssOut);

            auto correct = message;
            correct.set_value(temp_t::invalid().getRaw());
            correct.set_connected(false);

            CHECK(correct.DebugString() == round_trip.DebugString());
        }
    }
}
