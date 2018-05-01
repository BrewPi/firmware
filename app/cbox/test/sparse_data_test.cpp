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

#include "SparseData.pb.h"

SCENARIO("an object can be sparsely updated by sending a subset of values"){
    GIVEN("an existing struct holding 2 values"){
        blox_SparseDataComplete original;
        original.value1 = 123;
        original.value2 = 456;

        GIVEN("a compatible update struct with only partial data"){
            blox_SparseDataPartial update;
            update.value2 = 789;


            /* This is the buffer where we will store our message. */
            uint8_t buffer[100];
            size_t message_length;
            bool status;

            WHEN("partial update is encoded"){
                /* Create a message from the partial update
                 */
                pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
                status = pb_encode_delimited(&stream, blox_SparseDataPartial_fields, &update);

                THEN("no errors occur"){
                    if(!status)
                    {
                        WARN("Encoding failed: " << PB_GET_ERROR(&stream));
                    }
                    CHECK(status);
                }

                /* Now we could transmit the message over network, store it in a file or
                 * wrap it to a pigeon's leg.
                 */

                /* But because we are lazy, we will just decode it immediately. */

                AND_WHEN("the update message is decoded"){
                    blox_SparseDataComplete result = original;
                    REQUIRE(result.value1 == 123);
                    REQUIRE(result.value2 == 456);

                    /* Create a stream that reads from the buffer. */
                    pb_istream_t stream = pb_istream_from_buffer(buffer, sizeof(buffer));

                    /* Now we are ready to decode the message. */
                    status = pb_decode_delimited_noinit(&stream, blox_SparseDataComplete_fields, &result);

                    THEN("no errors occur"){
                        if (!status)
                        {
                            WARN("Decoding failed: " << PB_GET_ERROR(&stream));
                            CAPTURE(stream);
                        }
                        CHECK(status);
                    }

                    AND_THEN("It can be merged with the original value"){
                        CHECK(result.value1 == 123);
                        CHECK(result.value2 == 789);
                    }
                }
            }
        }
    }
}

