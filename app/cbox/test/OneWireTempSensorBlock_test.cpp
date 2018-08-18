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
#include "Box.h"
#include "VisitorCast.h"
#include "CboxApp.h"
#include "TestMatchers.hpp"

using namespace cbox;

SCENARIO("A Blox OneWireTempSensor object can be created from streamed protobuf data"){
    GIVEN("a protobuf message defining a OneWireTempSensor object"){
        blox_OneWireTempSensor message = {0}; // initialize to zero necessary if not all fields are set
        message.address = 0x0102030405060708;
        message.offset = 123;

        WHEN("it is encoded to a buffer"){
            uint8_t buf[100] = {0};

            BufferDataOut tempOut(buf, sizeof(buf));
            CboxError res = streamProtoTo(tempOut, &message, blox_OneWireTempSensor_fields, sizeof(buf));
            CHECK(res == CboxError::no_error);

            AND_WHEN("we create a DataIn object form that buffer"){
                BufferDataIn in(buf, sizeof(buf));

                THEN("a newly created OneWireTempSensorBloc object can receive settings from the DataIn stream")
                {
                    OneWireTempSensorBlock sensor;
                    sensor.streamFrom(in);
                    CHECK(sensor.get().getAddress() == message.address);
                    CHECK(sensor.get().getCalibration().getRaw() == message.offset);

                    AND_THEN("we can stream that bloc object to a DataOut stream")
                    {
                        uint8_t buf2[100] = {0};
                        BufferDataOut out(buf2, sizeof(buf2));
                        sensor.streamTo(out);
                        // verify data that is streamed out by streaming it back in
                        BufferDataIn in(buf2, sizeof(buf2));
                        blox_OneWireTempSensor received;
                        auto res = streamProtoFrom(in, &received, blox_OneWireTempSensor_fields, sizeof(buf2));

                        CHECK(res == CboxError::no_error);
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

/*
SCENARIO("Create blox OneWireTempSensor application object from definition"){
    GIVEN("A BrewBlox OneWireTempSensor definition"){
        blox_OneWireTempSensor definition = {0}; // initialize to zero necessary if not all fields are set
        definition.address = 0x0102030405060708;
        definition.offset = 456;

        uint8_t buffer1[100] = {0};
        BufferDataOut tempOut(buffer1, sizeof(buffer1));
        CboxError res = streamProtoTo(tempOut, &definition, blox_OneWireTempSensor_fields, sizeof(buffer1));
        CHECK(res == CboxError::no_error);

        BufferDataIn in(buffer1, sizeof(buffer1));
        obj_type_t typeId = resolveTypeID<OneWireTempSensorBlock>();

        WHEN("an application object is created form the definition"){

            CboxError err;
            std::shared_ptr<Object> obj = createApplicationObject(typeId, in, err);

            THEN("No errors occur"){
                CHECK(err == CboxError::no_error);
            }

            THEN("The object type is correct"){
                CHECK(typeId == obj->typeID());
            }

            REQUIRE(obj != nullptr);

            AND_THEN("we can stream that bloc object to a DataOut stream and it matches the definition")
            {
                uint8_t buf2[100] = {0};
                BufferDataOut out(buf2, sizeof(buf2));
                obj->streamTo(out);


                // verify data that is streamed out by streaming it back in
                BufferDataIn in(buf2, sizeof(buf2));
                blox_OneWireTempSensor received;
                auto res = streamProtoFrom(in, &received, blox_OneWireTempSensor_fields, sizeof(buf2));

                CHECK(res == CboxError::no_error);
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

        BufferDataIn in(wrong_defition, sizeof(wrong_defition));
        obj_type_t typeId = resolveTypeID<OneWireTempSensorBlock>();

        CboxError err;
        std::shared_ptr<Object> obj = createApplicationObject(typeId, in, err);

        CHECK(err == CboxError::input_stream_decoding_error);

        REQUIRE(obj == nullptr);
    }
}
*/
