#include "catch.hpp"
#include <cstdio>

#include "SetPointSimpleCbox.h"

SCENARIO("A Cbox SetPointSimple object can be created from streamed protobuf data"){
    GIVEN("a protobuf message defining a SetPointSimple object"){
        blox_SetPointSimple message;
        message.value = 123;

        WHEN("it is encoded to a buffer"){
            uint8_t buf[100];
            pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
            bool status = pb_encode_delimited(&stream, blox_SetPointSimple_fields, &message);
            CHECK(status);

            AND_WHEN("we create a DataIn object form that buffer"){
                BufferDataIn in(buf);

                THEN("a newly created cbox SetPointSimple object can receive settings from the DataIn stream")
                {
                    SetPointSimpleCBox sp;
                    sp.writeMaskedFrom(in, in); // use in as mask too, it is not used.
                    temp_t setting = sp.get().read();
                    temp_t valid;
                    valid.setRaw(123);
                    CHECK(setting == valid);

                    AND_THEN("we can stream that cbox object to a DataOut stream")
                    {
                        // change the value for a round trip test
                        sp.get().write(21.0);

                        uint8_t buf2[100];
                        BufferDataOut out(buf2, sizeof(buf2));
                        sp.readTo(out);

                        sp.get().write(25.0); // change again, so we can verify the receive
                        CHECK(sp.get().read() == temp_t(25.0));

                        BufferDataIn in_roundtrip(buf2);
                        sp.writeMaskedFrom(in_roundtrip, in_roundtrip);
                        CHECK(sp.get().read() == temp_t(21.0));
                    }
                }
            }
        }
    }
}


