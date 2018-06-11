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

#include "../blox/OneWireTempSensorBlock.h"
#include "Values.h"
#include "Commands.h"
#include "VisitorCast.h"


SCENARIO("A Blox OneWireTempSensor object can be created from streamed protobuf data"){
    GIVEN("a protobuf message defining a OneWireTempSensor object"){
        blox_OneWireTempSensor_Persisted message;
        uint8_t address[8] = {8, 7, 6, 5, 4, 3, 2, 1};
        memcpy(&message.settings.address, &address, 8);
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
            INFO("Encoding of sensor with address 0x0807060504030201 and offset 123 is " << ss.str());
            INFO("Length of encoding is " << blox_OneWireTempSensor_Persisted_size);

            AND_WHEN("we create a DataIn object form that buffer"){
                cbox::BufferDataIn in(buf);

                THEN("a newly created OneWireTempSensorBloc object can receive settings from the DataIn stream")
                {
                    OneWireTempSensorBlock sensor;
                    sensor.writeFrom(in);

                    AND_THEN("we can stream that bloc object to a DataOut stream")
                    {
                        uint8_t buf2[100] = {0};
                        cbox::BufferDataOut out(buf2, sizeof(buf2));
                        sensor.readTo(out);

                        // verify data that is streamed out by streaming it back in
                        pb_istream_t stream_in = pb_istream_from_buffer(buf2, sizeof(buf2));

                        blox_OneWireTempSensor received;
                        pb_decode_delimited(&stream_in, blox_OneWireTempSensor_fields, &received);
                        for(uint8_t i = 0; i < 8; i++){
                            CHECK(received.settings.address[i] == message.settings.address[i]);
                        }
                        CHECK(received.settings.offset == message.settings.offset);
                        CHECK(received.state.value == temp_t::invalid().getRaw());
                        CHECK(received.state.connected == false);
                    }
                }
            }
        }
    }
}


SCENARIO("Create blox OneWireTempSensor application object from definition"){
    GIVEN("A BrewBlox OneWireTempSensor definition"){
        bool status;
        blox_OneWireTempSensor_Persisted definition;
        uint8_t address[8] = {8, 7, 6, 5, 4, 3, 2, 1};
        memcpy(&definition.settings.address, &address, 8);
        definition.settings.offset = 456;

        uint8_t buffer1[100] = {0};
        pb_ostream_t stream1 = pb_ostream_from_buffer(buffer1, sizeof(buffer1));
        status = pb_encode_delimited(&stream1, blox_OneWireTempSensor_Persisted_fields, &definition);
        CHECK(status);

        cbox::BufferDataIn in(buffer1);
        uint8_t len = OneWireTempSensorBlock::persistedMaxSize();
        cbox::obj_type_t typeId = cbox::resolveTypeID<OneWireTempSensorBlock>();

        CHECK(typeId);

        cbox::ObjectDefinition dfn = {&in, len, typeId};

        WHEN("an application object is created form the definition"){
            cbox::Object * obj = nullptr;
            uint8_t error = createApplicationObject(obj, dfn, false);

            THEN("No errors occur"){
                CHECK(error == cbox::errorCode(cbox::no_error));
            }

            REQUIRE(obj != nullptr);

            AND_THEN("we can stream that bloc object to a DataOut stream and it matches the definition")
            {
                uint8_t buf2[100] = {0};
                cbox::BufferDataOut out(buf2, sizeof(buf2));
                ((cbox::Value*)obj)->readTo(out); // TODO: this typecast shouldn't be necessary? What's the base class to stream objects?

                // verify data that is streamed out by streaming it back in
                pb_istream_t stream_in = pb_istream_from_buffer(buf2, sizeof(buf2));

                // settings are streamed first
                blox_OneWireTempSensor received;
                pb_decode_delimited(&stream_in, blox_OneWireTempSensor_fields, &received);
                for(uint8_t i = 0; i < 8; i++){
                    CHECK(received.settings.address[i] == definition.settings.address[i]);
                }
                CHECK(received.settings.offset == 456);
            }

            AND_THEN("Cbox Object::update() invokes update on contained application object"){
                obj->update();
            }
        }
    }
}

SCENARIO("Send an invalid protobuf creation command"){
    GIVEN("A payload with a protobuf definition that doesn't match the expected format"){
        uint8_t wrong_defition[] = "\x0c\n\n\n\x08(\x9el\xff\x08\x00\x00B";

        cbox::BufferDataIn in(wrong_defition);
        uint8_t len = OneWireTempSensorBlock::persistedMaxSize();
        cbox::obj_type_t typeId = cbox::resolveTypeID<OneWireTempSensorBlock>();

        cbox::ObjectDefinition dfn = {&in, len, typeId};

        cbox::Object * obj = nullptr;
        uint8_t error = createApplicationObject(obj, dfn, false);

        REQUIRE(obj != nullptr);
    }
}

