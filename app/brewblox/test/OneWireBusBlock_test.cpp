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

#include "blox/OneWireBusBlock.h"

#include "catch.hpp"
#include <cstdio>
#include <iomanip>
#include <iostream>

#include "GlobalObjects.h"
#include "blox/Block.h"
#include "cbox/Box.h"
#include "cbox/DataStream.h"
#include "cbox/Object.h"

using namespace cbox;

void
streamHex(std::stringstream& ss, uint8_t* buf, size_t len)
{
    ss << "0x" << std::setfill('0') << std::hex;
    for (size_t i = 0; i < len; i++) {
        ss << std::setw(2) << static_cast<unsigned>(buf[i]);
    }
}

SCENARIO("A Blox OneWireBus can stream a variable number of found addresses")
{
    GIVEN("A Blox OneWireBus")
    {
        OneWireBusBlock ow(theOneWire());

        WHEN("it is encoded to a buffer")
        {
            uint8_t outbuf[100] = {0};
            BufferDataOut out(outbuf, sizeof(outbuf));
            ow.streamTo(out);
            std::stringstream ss;
            streamHex(ss, outbuf, out.bytesWritten());
            INFO("OneWireBus result is " << ss.str());
        }

        WHEN("a family search command is streamed in")
        {

            blox_OneWireBus message;
            message.command.opcode = 2;  // OneWire search
            message.command.data = 0x28; // family code for onewire temp sensor
            message.address.funcs.encode = nullptr;
            message.address.arg = nullptr;

            uint8_t inbuf[100] = {0};

            BufferDataOut tempOut(inbuf, sizeof(inbuf));

            CboxError res = streamProtoTo(tempOut, &message, blox_OneWireBus_fields, sizeof(inbuf));
            CHECK(res == CboxError::OK);

            BufferDataIn in(inbuf, sizeof(inbuf));

            ow.streamFrom(in);

            uint8_t outbuf[100] = {0};
            BufferDataOut out(outbuf, sizeof(outbuf));
            ow.streamTo(out);
            std::stringstream ss;
            streamHex(ss, outbuf, out.bytesWritten());
            INFO("OneWireBus result 2 is " << ss.str());

            out.reset();
            ow.streamTo(out);
            std::stringstream ss2;
            streamHex(ss2, outbuf, out.bytesWritten());
            INFO("OneWireBus result 3 is " << ss2.str());
        }
    }
}
