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

#include "SetPointSimpleBlock.h"

#include "Block.h"
#include "Object.h"
#include "Commands.h"
#include "temperatureFormats.h"
#include "DataStream.h"
#include "CboxApp.h"
#include <memory>

SCENARIO("A Bloc SetPointSimple object can be created from streamed protobuf data"){
    WHEN("a protobuf message defining a SetPointSimple object is encoded to a buffer"){
        blox_SetPointSimple message;
        message.setting = 123;

        uint8_t buf[100];
        cbox::BufferDataOut out(buf, sizeof(buf));
        auto result = streamProtoTo(out, &message, blox_SetPointSimple_fields, blox_SetPointSimple_size);
        THEN("no errors occur")
        {
            CHECK(result == cbox::StreamResult::success);
        }

        AND_WHEN("we stream that same buffer back into an existing SetPoint object, the setting matches the message")
        {
            cbox::BufferDataIn in(buf, sizeof(buf));
            SetPointSimpleBlock sp;
            cbox::StreamResult res = sp.streamFrom(in);
            CHECK(res == cbox::StreamResult::success);

            temp_t setting = sp.get().read();
            temp_t valid; valid.setRaw(123);
            CHECK(setting == valid);

            AND_THEN("we can stream that same SetPoint to a DataOut stream")
            {
                // change the value for a round trip test
                sp.get().write(21.0);

                uint8_t buf2[100];
                cbox::BufferDataOut out2(buf2, sizeof(buf2));
                cbox::StreamResult res2 = sp.streamTo(out2);
                CHECK(res2 == cbox::StreamResult::success);

                sp.get().write(25.0); // change again, so we can verify the receive
                CHECK(sp.get().read() == temp_t(25.0));

                cbox::BufferDataIn in_roundtrip(buf2, sizeof(buf2));
                cbox::StreamResult res3 = sp.streamFrom(in_roundtrip);
                CHECK(res3 == cbox::StreamResult::success);

                CHECK(sp.get().read() == temp_t(21.0));
            }
        }
    }
}

SCENARIO("Create blox SetPointSimple application object from definition"){
    GIVEN("A BrewBlox SetPointSimple definition"){
        blox_SetPointSimple definition;
        definition.setting = 123;

        uint8_t buffer1[100];
        cbox::BufferDataOut tempOut(buffer1, sizeof(buffer1));
        cbox::StreamResult res = streamProtoTo(tempOut, &definition, blox_SetPointSimple_fields, sizeof(buffer1));
        CHECK(res == cbox::StreamResult::success);

        cbox::BufferDataIn in(buffer1, sizeof(buffer1));
        cbox::obj_type_t typeId = cbox::resolveTypeID<SetPointSimpleBlock>();
        cbox::CommandError er;

        WHEN("an application object is created form the definition"){
        	std::shared_ptr<cbox::Object> obj;
        	obj = createApplicationObject(typeId, in, er);

            THEN("No errors occur"){
                CHECK(obj != nullptr);
                CHECK(errorCode(er) == errorCode(cbox::CommandError::no_error));
            }
        }
    }
}


