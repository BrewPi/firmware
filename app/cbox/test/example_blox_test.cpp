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

#include <pb_decode.h>
#include <pb_encode.h>

#include "Example.pb.h"

SCENARIO("an object can receive just Settings"){
    GIVEN("an existing message holding Settings, Value and State"){
        blox_Example original;
        original.settings.setting1 = 123;
        original.settings.setting2  = 456;

        original.state.state1 = 123;
        original.state.state2 = 123;

        original.links.link1 = 0x01010101;
        original.links.link2 = 0x02020202;


        GIVEN("a compatible update struct with only new Settings"){
            blox_Example_Settings_Only update;
            update.settings.setting1 = 678;
            update.settings.setting2 = 789;


            /* This is the buffer where we will store our message. */
            uint8_t buffer[100];
            size_t message_length;
            bool status;

            WHEN("partial update is encoded"){
                /* Create a message from the partial update
                 */
                pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

                status = pb_encode_delimited(&stream, blox_Example_Settings_Only_fields, &update);

                THEN("no errors occur"){
                    if(!status)
                    {
                        WARN("Encoding failed: " << PB_GET_ERROR(&stream));
                    }
                    REQUIRE(status);
                }

                /* Now we could transmit the message over network, store it in a file or
                 * wrap it to a pigeon's leg.
                 */

                /* But here we will just decode it immediately. */

                AND_WHEN("the update message is decoded, using the original proto with all fields"){
                    blox_Example result = original; // copy original
                    REQUIRE(result.settings.setting1 == 123);
                    REQUIRE(result.settings.setting2 == 456);
                    REQUIRE(result.state.state1 == 123);
                    REQUIRE(result.state.state2 == 123);
                    REQUIRE(result.links.link1 == 0x01010101);
                    REQUIRE(result.links.link2 == 0x02020202);

                    /* Create a stream that reads from the buffer. */
                    pb_istream_t stream = pb_istream_from_buffer(buffer, sizeof(buffer));

                    /* Now we are ready to decode the message. */
                    status = pb_decode_delimited_noinit(&stream, blox_Example_fields, &result);

                    THEN("no errors occur"){
                        if (!status)
                        {
                            WARN("Decoding failed: " << PB_GET_ERROR(&stream));
                            CAPTURE(stream);
                        }
                        REQUIRE(status);
                    }

                    AND_THEN("The settings are updated in the Example message"){
                        CHECK(result.settings.setting1 == 678);
                        CHECK(result.settings.setting2 == 789);
                    }

                    AND_THEN("The fields not in the update are untouched"){
                        CHECK(result.state.state1 == 123);
                        CHECK(result.state.state2 == 123);
                        CHECK(result.links.link1 == 0x01010101);
                        CHECK(result.links.link2 == 0x02020202);
                    }
                }
            }
        }
    }
}

