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

#include "OneWireTempSensorBlock.h"
#include "Object.h"
#include "Commands.h"
#include "VisitorCast.h"
#include "CboxApp.h"
#include "TestMatchers.hpp"

SCENARIO("A Blox OneWireTempSensor object can be created from streamed protobuf data"){
    GIVEN("a protobuf message defining a OneWireTempSensor object"){
        blox_OneWireTempSensor message;
        uint8_t address[8] = {8, 7, 6, 5, 4, 3, 2, 1};
        memcpy(&message.address, &address, 8);
        message.offset = 123;

        WHEN("it is encoded to a buffer"){
            uint8_t buf[100] = {0};
            pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));

            bool status = pb_encode(&stream, blox_OneWireTempSensor_fields, &message);
            CHECK(status);

            AND_WHEN("we create a DataIn object form that buffer"){
                cbox::BufferDataIn in(buf, sizeof(buf));

                THEN("a newly created OneWireTempSensorBloc object can receive settings from the DataIn stream")
                {
                    OneWireTempSensorBlock sensor;
                    sensor.streamFrom(in);
                    CHECK(sensor.get().getAddress() == message.address);
                    CHECK(sensor.get().getCalibration().getRaw() == message.offset);

                    AND_THEN("we can stream that bloc object to a DataOut stream")
                    {
                        uint8_t buf2[100] = {0};
                        cbox::BufferDataOut out(buf2, sizeof(buf2));
                        sensor.streamTo(out);

                        // verify data that is streamed out by streaming it back in
                        pb_istream_t stream_in = pb_istream_from_buffer(buf2, sizeof(buf2));

                        blox_OneWireTempSensor received;
                        bool success = pb_decode(&stream_in, blox_OneWireTempSensor_fields, &received);
                        CHECK(success);
                        CHECK(received.address == message.address);
                        CHECK(received.offset == message.offset);
                        CHECK(received.value == temp_t::invalid().getRaw());
                        CHECK(received.connected == false);
                    }
                }
            }
        }
    }
}


SCENARIO("Create blox OneWireTempSensor application object from definition"){
    GIVEN("A BrewBlox OneWireTempSensor definition"){
        bool status;
        blox_OneWireTempSensor definition;
        uint8_t address[8] = {8, 7, 6, 5, 4, 3, 2, 1};
        memcpy(&definition.address, &address, 8);
        definition.offset = 456;

        uint8_t buffer1[100] = {0};
        pb_ostream_t stream1 = pb_ostream_from_buffer(buffer1, sizeof(buffer1));
        status = pb_encode(&stream1, blox_OneWireTempSensor_fields, &definition);
        CHECK(status);

        cbox::BufferDataIn in(buffer1, sizeof(buffer1));
        cbox::obj_type_t typeId = cbox::resolveTypeID<OneWireTempSensorBlock>();

        WHEN("an application object is created form the definition"){

            cbox::CommandError err;
            std::shared_ptr<cbox::Object> obj = createApplicationObject(typeId, in, err);

            THEN("No errors occur"){
                CHECK(err == cbox::CommandError::no_error);
            }

            REQUIRE(obj != nullptr);

            AND_THEN("we can stream that bloc object to a DataOut stream and it matches the definition")
            {
                uint8_t buf2[100] = {0};
                cbox::BufferDataOut out(buf2, sizeof(buf2));
                obj->streamTo(out);

                // verify data that is streamed out by streaming it back in
                pb_istream_t stream_in = pb_istream_from_buffer(buf2, sizeof(buf2));

                // settings are streamed first
                blox_OneWireTempSensor received;
                pb_decode(&stream_in, blox_OneWireTempSensor_fields, &received);

                CHECK(received.address == definition.address);
                CHECK(received.offset == 456);
            }

            AND_THEN("Cbox Object::update() invokes update on contained application object"){
                obj->update(0);
            }
        }
    }
}

SCENARIO("Send an invalid protobuf creation command"){
    GIVEN("A payload with a protobuf definition that doesn't match the expected format"){
        uint8_t wrong_defition[] = "\x0c\n\n\n\x08(\x9el\xff\x08\x00\x00B";

        cbox::BufferDataIn in(wrong_defition, sizeof(wrong_defition));
        cbox::obj_type_t typeId = cbox::resolveTypeID<OneWireTempSensorBlock>();

        cbox::CommandError err;
        std::shared_ptr<cbox::Object> obj = createApplicationObject(typeId, in, err);

        CHECK(err == cbox::CommandError::stream_error);

        REQUIRE(obj != nullptr);
    }
}
