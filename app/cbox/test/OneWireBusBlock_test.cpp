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

#include "catch.hpp"
#include <cstdio>
#include <iostream>
#include <iomanip>

#include "Values.h"
#include "Commands.h"
#include "OneWireBusBlock.h"

void streamHex(std::stringstream & ss, uint8_t * buf, size_t len){
    ss << "0x" << std::setfill('0') << std::hex;
    for(size_t i = 0 ; i < len; i ++){
        ss << std::setw(2) << static_cast<unsigned>(buf[i]);
    }
}

SCENARIO("A Blox OneWireBus can stream a variable number of found addresses"){
    GIVEN("A Blox OneWireBus"){
        OneWireBusBlock ow;

        WHEN("it is encoded to a buffer"){
            uint8_t outbuf[100] = {0};
            cbox::BufferDataOut out(outbuf, sizeof(outbuf));
            ow.readTo(out);
            std::stringstream ss;
            streamHex(ss, outbuf, out.bytesWritten());
            INFO("OneWireBus result is " << ss.str());
        }

        WHEN("a family search command is streamed in"){

            blox_OneWireCommand message;
            message.command = 2; // OneWire search
            message.data = 0x28; // family code for onewire temp sensor

            uint8_t inbuf[100] = {0};
            pb_ostream_t stream = pb_ostream_from_buffer(inbuf, sizeof(inbuf));
            bool status = pb_encode_delimited(&stream, blox_OneWireCommand_fields, &message);

            cbox::BufferDataIn in(inbuf);

            ow.writeFrom(in);

            uint8_t outbuf[100] = {0};
            cbox::BufferDataOut out(outbuf, sizeof(outbuf));
            ow.readTo(out);
            std::stringstream ss;
            streamHex(ss, outbuf, out.bytesWritten());
            INFO("OneWireBus result 2 is " << ss.str());

            out.reset();
            ow.readTo(out);
            std::stringstream ss2;
            streamHex(ss2, outbuf, out.bytesWritten());
            INFO("OneWireBus result 3 is " << ss2.str());
        }
    }
}
