#include "catch.hpp"
#include <cstdio>

#include <pb_decode.h>
#include <pb_encode.h>
#include "SetPoint.h"

#include "SetPoint.h"
#include "SetPointSimple.pb.h"

SCENARIO("Encoding a message and encoding a SetPointSimple settings struct directly gives the same result"){
    GIVEN("a SetPointSimple settings struct and a SetPointSimple settings message with the same value"){
        SetPointSimple setpoint;
        temp_t setting;
        setting.setRaw(123);
        setpoint.write(setting);

        blox_SetPointSimple message;
        message.value = 123;

        CHECK(sizeof(setpoint.settings) == sizeof(message));


        WHEN("they are encoded."){

            bool status;
            uint8_t buffer1[100];
            pb_ostream_t stream1 = pb_ostream_from_buffer(buffer1, sizeof(buffer1));
            status = pb_encode_delimited(&stream1, blox_SetPointSimple_fields, &setpoint.settings);
            size_t message_length1 = stream1.bytes_written;
            CHECK(status);

            uint8_t buffer2[100];
            pb_ostream_t stream2 = pb_ostream_from_buffer(buffer2, sizeof(buffer2));
            status = pb_encode_delimited(&stream2, blox_SetPointSimple_fields, &message);
            size_t message_length2 = stream2.bytes_written;
            CHECK(status);

            THEN("The resulting message is the same, so we can encode the SetPointSimple directly")
            {
                REQUIRE(message_length1 == message_length2);

                for(size_t i = 0; i < message_length1; i++){
                    INFO("" << i << "/" << message_length1);
                    CHECK((int) buffer1[i] == (int) buffer2[i]);
                }
            }
        }
    }
}


SCENARIO("Encode and decode SetPointSimple settings to protobuf")
{
    GIVEN("A BrewBlox SetPointSimple object")
    {
        SetPointSimple original;

        REQUIRE(original.read() == temp_t::disabled());

        original.write(temp_t(20.0));
        REQUIRE(original.read() == temp_t(20.0));

        /* This is the buffer where we will store our message. */
        uint8_t buffer[100];
        size_t message_length;
        bool status;

        WHEN("SetPointSimple settings are encoded"){
            /* Create a stream that will write to our buffer. Reserve buffer[0] for length*/
            pb_ostream_t stream = pb_ostream_from_buffer(&buffer[1], sizeof(buffer)-1);

            /* The SetPointSimple settings can be directly encoded to the stream.
             * This is because the message definition struct matches the SetPointSimple settings struct 1:1.
             */
            status = pb_encode_delimited(&stream, blox_SetPointSimple_fields, &original.settings);
            message_length = stream.bytes_written;

            THEN("size is smaller than or equal to maximum size"){
                CHECK(message_length <= blox_SetPointSimple_size);
            }

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

            AND_WHEN("the same message is decoded"){
                SetPointSimple round_trip;

                REQUIRE(round_trip.read() == temp_t::disabled());

                /* Get message length from first byte in the buffer */
                size_t receive_length = buffer[0];

                /* Create a stream that reads from the buffer. */
                pb_istream_t stream = pb_istream_from_buffer(&buffer[1], receive_length);

                /* Now we are ready to decode the message. */
                status = pb_decode_delimited(&stream, blox_SetPointSimple_fields, &round_trip.settings);

                THEN("no errors occur"){
                    if (!status)
                    {
                        WARN("Decoding failed: " << PB_GET_ERROR(&stream));
                        CAPTURE(stream);
                    }
                    CHECK(status);
                }

                AND_THEN("The decoded value matches the original value"){
                    /* Print the data contained in the message. */
                    INFO("Data received was: " << round_trip.read());
                    CHECK(original.read() == round_trip.read());
                }
            }
        }
    }
}

