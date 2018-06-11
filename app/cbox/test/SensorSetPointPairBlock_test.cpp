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

#include "../blox/SensorSetPointPairBlock.h"
#include "Values.h"
#include "Commands.h"
#include "VisitorCast.h"

SCENARIO("A Blox SensorSetPointPair object can be created from streamed protobuf data"){
    GIVEN("a protobuf message defining a SensorSetPointPair object"){
        blox_SensorSetPointPair_Persisted message;
        message.links = {{0x01, 0x00, 0x00, 0x00}, {0x02, 0x00, 0x00, 0x00}};

        WHEN("it is encoded to a buffer"){
            uint8_t buf[100] = {0};
            pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
            bool status = pb_encode_delimited(&stream, blox_SensorSetPointPair_Persisted_fields, &message);
            CHECK(status);

            std::stringstream ss;
            ss << "0x" << std::setfill('0') << std::hex;
            for(int i =0 ; i <= blox_SensorSetPointPair_Persisted_size; i ++){
                ss << std::setw(2) << static_cast<unsigned>(buf[i]);
            }
            INFO("Encoding of SensorSetPointPair with sensor lookup 0x01000000 and setpoint lookup 0x02000000 is " << ss.str());
            INFO("Length of encoding is " << blox_SensorSetPointPair_Persisted_size);

            AND_WHEN("we create a DataIn object from that buffer"){
            	cbox::BufferDataIn in(buf);

                THEN("a newly created SensorSetPointPairBloc object can receive settings from the DataIn stream")
                {
                    SensorSetPointPairBlock sensor;
                    sensor.writeFrom(in); // use in as mask too, it is not used.

                    AND_THEN("we can stream that bloc object to a DataOut stream")
                    {
                        uint8_t buf2[100] = {0};
                        cbox::BufferDataOut out(buf2, sizeof(buf2));
                        sensor.readTo(out);

                        // verify data that is streamed out by streaming it back in
                        pb_istream_t stream_in = pb_istream_from_buffer(buf2, sizeof(buf2));

                        blox_SensorSetPointPair received;
                        pb_decode_delimited(&stream_in, blox_SensorSetPointPair_fields, &received);
                        for(int i = 0; i < 4; i++){
                            CHECK(received.links.sensor[i] == message.links.sensor[i]);
                            CHECK(received.links.setpoint[i] == message.links.setpoint[i]);
                        }
                    }
                }
            }
        }
    }
}


SCENARIO("Create blox SensorSetPointPair application object from definition"){
    GIVEN("A BrewBlox SensorSetPointPair definition"){
        bool status;
        blox_SensorSetPointPair_Persisted definition;
        definition.links = {{0x01, 0x00, 0x00, 0x00}, {0x02, 0x00, 0x00, 0x00}};

        uint8_t buffer1[100] = {0};
        pb_ostream_t stream1 = pb_ostream_from_buffer(buffer1, sizeof(buffer1));
        status = pb_encode_delimited(&stream1, blox_SensorSetPointPair_Persisted_fields, &definition);
        CHECK(status);

        cbox::BufferDataIn in(buffer1);
        uint8_t len = SensorSetPointPairBlock::persistedMaxSize();
        cbox::obj_type_t typeId = cbox::resolveTypeID<SensorSetPointPairBlock>();

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
                blox_SensorSetPointPair received;
                pb_decode_delimited(&stream_in, blox_SensorSetPointPair_fields, &received);

                for(int i = 0; i < 4; i++){
                    CHECK(received.links.sensor[i] == definition.links.sensor[i]);
                    CHECK(received.links.setpoint[i] == definition.links.setpoint[i]);
                }
            }
            AND_WHEN("The SensorSetPointPair contained in the bloc is used as application object"){
                ProcessValue * pv = asInterface<ProcessValue>(obj->getApplicationInterface());
                pv->set(temp_t(10.0));
                temp_t setting = pv->setting();
                temp_t value = pv->value();

                // both values will return invalid, because lookups point to default objects
                CHECK(setting == temp_t::invalid());
                CHECK(value == temp_t::invalid());
            }
            AND_WHEN("Cbox Object::update() invokes update on contained application object"){
                obj->update();
            }
        }
    }
}

