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

#include "../blox/PidBlock.h"
#include "Values.h"
#include "Commands.h"
#include "VisitorCast.h"

SCENARIO("A Blox Pid object can be created from streamed protobuf data"){
    GIVEN("a protobuf message struct defining a Pid object"){
        blox_Pid_Persisted message;
        message.settings.enabled = false;
        message.settings.kp = 10;
        message.settings.ti = 1600;
        message.settings.td = 60;
        message.links = {{0x01, 0x00, 0x00, 0x00}, {0x02, 0x00, 0x00, 0x00}};
        message.filtering.input = 3;
        message.filtering.derivative = 4;

        WHEN("it is encoded to a buffer"){
            uint8_t buf[100] = {0};
            pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
            bool status = pb_encode_delimited(&stream, blox_Pid_Persisted_fields, &message);
            CHECK(status);

            std::stringstream ss;
            ss << "0x" << std::setfill('0') << std::hex;
            for(int i =0 ; i <= blox_Pid_Persisted_size; i ++){
                ss << std::setw(2) << static_cast<unsigned>(buf[i]);
            }
            INFO("Encoding of Pid Object is " << ss.str());
            INFO("Length of encoding is " << blox_Pid_Persisted_size);

            AND_WHEN("we create a DataIn object from that buffer and create an object definition from it"){
            	cbox::BufferDataIn in(buf);

                uint8_t len = PidBlock::persistedMaxSize();
                cbox::obj_type_t typeId = cbox::resolveTypeID<PidBlock>();

                cbox::ObjectDefinition dfn = {&in, len, typeId};

                WHEN("an application object is created form the definition"){
                	cbox::Object * obj = nullptr;
                    uint8_t error = createApplicationObject(obj, dfn, false);

                    THEN("No errors occur"){
                        CHECK(error == cbox::errorCode(cbox::no_error));
                    }
                    REQUIRE(obj != nullptr);

                    cbox::WritableValue * pidObj = (cbox::WritableValue*) obj;

                    AND_WHEN("we stream that bloc object to a DataOut stream")
                    {
                        uint8_t buf2[100] = {0};
                        cbox::BufferDataOut out(buf2, sizeof(buf2));
                        pidObj->readTo(out);

                        // verify data that is streamed out by streaming it back in
                        pb_istream_t stream_in = pb_istream_from_buffer(buf2, sizeof(buf2));

                        blox_Pid received;
                        pb_decode_delimited(&stream_in, blox_Pid_fields, &received);
                        THEN("The output matches the definition")
                        {
                            // check settings
                            CHECK(received.settings.enabled == message.settings.enabled);
                            CHECK(received.settings.kp == message.settings.kp);
                            CHECK(received.settings.ti == message.settings.ti);
                            CHECK(received.settings.td == message.settings.td);
                            // check links
                            for(uint8_t i = 0; i < MAX_ID_CHAIN_LENGHT; i++){
                                CHECK(received.links.input[i] == message.links.input[i]);
                                CHECK(received.links.output[i] == message.links.output[i]);
                            }
                            // check filtering
                            CHECK(received.filtering.input == message.filtering.input);
                            CHECK(received.filtering.derivative == message.filtering.derivative);
                        }
                    }

                    AND_WHEN("The newly created PidBloc object receives settings from a DataIn stream")
                    {
                        blox_Pid_Persisted message2;
                        message2.settings.enabled = false;
                        message2.settings.kp = temp_t(5.0).getRaw();
                        message2.settings.ti = 1200;
                        message2.settings.td = 30;
                        message2.links = {{0x02, 0x03, 0x04, 0x05}, {0x03, 0x04, 0x05, 0x06}};
                        message2.filtering.input = 1;
                        message2.filtering.derivative = 2;

                        uint8_t buf2[100] = {0};
                        pb_ostream_t stream2 = pb_ostream_from_buffer(buf2, sizeof(buf2));
                        bool status = pb_encode_delimited(&stream2, blox_Pid_Persisted_fields, &message2);
                        CHECK(status);

                        cbox::BufferDataIn in2(buf2);
                        pidObj->writeFrom(in2);

                        THEN("The output stream changes to reflect the new settings")
                        {

                          uint8_t buf3[100] = {0};
                          cbox::BufferDataOut out(buf3, sizeof(buf3));
                          pidObj->readTo(out);

                          // verify data that is streamed out by streaming it back in
                          pb_istream_t stream_in = pb_istream_from_buffer(buf3, sizeof(buf3));

                          blox_Pid received;
                          pb_decode_delimited(&stream_in, blox_Pid_fields, &received);
                              // check settings
                          CHECK(received.settings.enabled == false);
                          CHECK(received.settings.kp == temp_t(5.0).getRaw());
                          CHECK(received.settings.ti == 1200);
                          CHECK(received.settings.td == 30);
                          // check links
                          for(uint8_t i = 0; i < MAX_ID_CHAIN_LENGHT; i++){
                              CHECK(received.links.input[i] == message2.links.input[i]);
                              CHECK(received.links.output[i] == message2.links.output[i]);
                          }
                          // check filtering
                          CHECK(received.filtering.input == 1);
                          CHECK(received.filtering.derivative == 2);
                        }
                    }
                    AND_WHEN("The Pid contained in the bloc is used as application object")
                    {
                        Pid * p = asInterface<Pid>(pidObj->getApplicationInterface());
                        REQUIRE(p != nullptr);

                        p->setConstants(6.0, 900, 15);
                        p->setInputFiltering(2);
                        p->setDerivativeFiltering(3);
                        p->disable(false);

                        THEN("the output stream changes accordingly"){
                            uint8_t buf2[100] = {0};
                            cbox::BufferDataOut out(buf2, sizeof(buf2));
                            pidObj->readTo(out);

                            // verify data that is streamed out by streaming it back in
                            pb_istream_t stream_in = pb_istream_from_buffer(buf2, sizeof(buf2));

                            blox_Pid received;
                            pb_decode_delimited(&stream_in, blox_Pid_fields, &received);
                            // check settings
                            CHECK(received.settings.enabled == false);
                            CHECK(received.settings.kp == temp_t(6.0).getRaw());
                            CHECK(received.settings.ti == 900);
                            CHECK(received.settings.td == 15);
                            CHECK(received.filtering.input == 2);
                            CHECK(received.filtering.derivative == 3);
                        }
                    }
                }
            }
        }
    }
}
