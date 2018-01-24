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
#include "OneWireTempSensorBloc.h"


SCENARIO("A Blox OneWireTempSensor object can be created from streamed protobuf data"){
    GIVEN("a protobuf message defining a OneWireTempSensor object"){
        blox_OneWireTempSensor_Persisted message;
        message.settings.address = 0x0011223344556677;
        message.settings.offset = 123;

        WHEN("it is encoded to a buffer"){
            uint8_t buf[100] = {0};
            pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
            bool status = pb_encode_delimited(&stream, blox_OneWireTempSensor_Persisted_fields, &message);
            CHECK(status);

            std::stringstream ss;
            ss << "0x" << std::setfill('0') << std::hex;
            for(int i =0 ; i <= blox_OneWireTempSensor_Persisted_size; i ++){
                 ss << std::setw(2) << static_cast<unsigned>(buf[i]);
            }
            WARN("Encoding of sensor with address 0x0011223344556677 and offset 123 is " << ss.str());
            WARN("Length of encoding is " << blox_OneWireTempSensor_Persisted_size);

            AND_WHEN("we create a DataIn object form that buffer"){
                BufferDataIn in(buf);

                THEN("a newly created OneWireTempSensorBloc object can receive settings from the DataIn stream")
                {
                    OneWireTempSensorBloc sensor;
                    sensor.writeMaskedFrom(in, in); // use in as mask too, it is not used.

                    AND_THEN("we can stream that bloc object to a DataOut stream")
                    {
                        uint8_t buf2[100] = {0};
                        BufferDataOut out(buf2, sizeof(buf2));
                        sensor.readTo(out);

                        // verify data that is streamed out by streaming it back in
                        pb_istream_t stream_in = pb_istream_from_buffer(buf2, sizeof(buf2));

                        blox_OneWireTempSensor received;
                        pb_decode_delimited(&stream_in, blox_OneWireTempSensor_fields, &received);
                        CHECK(received.settings.address == message.settings.address);
                        CHECK(received.settings.offset == message.settings.offset);
                        CHECK(received.state.value == temp_t::invalid().getRaw());
                        CHECK(received.state.connected == true);
                    }
                }
            }
        }
    }
}


SCENARIO("Create blox OneWireTempSensor application object from definition"){
    GIVEN("A BrewBlox OneWireTempSensor definition")
    {
        bool status;
        blox_OneWireTempSensor_Persisted definition;
        definition.settings = {0x1122334455667788, 456};

        uint8_t buffer1[100] = {0};
        pb_ostream_t stream1 = pb_ostream_from_buffer(buffer1, sizeof(buffer1));
        status = pb_encode_delimited(&stream1, blox_OneWireTempSensor_Persisted_fields, &definition);
        CHECK(status);

        BufferDataIn in(buffer1);
        uint8_t len = OneWireTempSensorBloc::persistedMaxSize();
        uint8_t typeId = 6; //OneWireTempSensorBloc

        ObjectDefinition dfn = {&in, len, typeId};

        WHEN("an application object is created form the definition"){
            Object * obj = nullptr;
            uint8_t error = createApplicationObject(obj, dfn, false);

            THEN("No errors occur"){
                CHECK(error == errorCode(no_error));
            }

            AND_THEN("we can stream that bloc object to a DataOut stream and it matches the definition")
            {
                uint8_t buf2[100] = {0};
                BufferDataOut out(buf2, sizeof(buf2));
                ((Value*)obj)->readTo(out); // TODO: this typecast shouldn't be necessary? What's the base class to stream objects?

                // verify data that is streamed out by streaming it back in
                pb_istream_t stream_in = pb_istream_from_buffer(buf2, sizeof(buf2));

                // settings are streamed first
                blox_OneWireTempSensor received;
                pb_decode_delimited(&stream_in, blox_OneWireTempSensor_fields, &received);
                CHECK(received.settings.address == 0x1122334455667788);
                CHECK(received.settings.offset == 456);
            }
        }
    }
}

