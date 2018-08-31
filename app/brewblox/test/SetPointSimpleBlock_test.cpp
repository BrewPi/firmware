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

#include "blox/SetPointSimpleBlock.h"

#include "catch.hpp"
#include <cstdio>

#include "temperatureFormats.h"
#include <memory>

#include "../BrewBlox.h"
#include "blox/Block.h"
#include "cbox/Box.h"
#include "cbox/DataStream.h"
#include "cbox/Object.h"

using namespace cbox;

SCENARIO("A Bloc SetPointSimple object can be created from streamed protobuf data")
{
    WHEN("a protobuf message defining a SetPointSimple object is encoded to a buffer")
    {
        blox_SetPointSimple message;
        message.setting = 123;

        uint8_t buf[100];
        BufferDataOut out(buf, sizeof(buf));
        auto result = streamProtoTo(out, &message, blox_SetPointSimple_fields, blox_SetPointSimple_size);
        THEN("no errors occur")
        {
            CHECK(result == CboxError::OK);
        }

        AND_WHEN("we stream that same buffer back into an existing SetPoint object, the setting matches the message")
        {
            BufferDataIn in(buf, sizeof(buf));
            SetPointSimpleBlock sp;
            CboxError res = sp.streamFrom(in);
            CHECK(res == CboxError::OK);

            temp_t setting = sp.get().read();
            temp_t valid;
            valid.setRaw(123);
            CHECK(setting == valid);

            AND_THEN("we can stream that same SetPoint to a DataOut stream")
            {
                // change the value for a round trip test
                sp.get().write(21.0);

                uint8_t buf2[100];
                BufferDataOut out2(buf2, sizeof(buf2));
                CboxError res2 = sp.streamTo(out2);
                CHECK(res2 == CboxError::OK);

                sp.get().write(25.0); // change again, so we can verify the receive
                CHECK(sp.get().read() == temp_t(25.0));

                BufferDataIn in_roundtrip(buf2, sizeof(buf2));
                CboxError res3 = sp.streamFrom(in_roundtrip);
                CHECK(res3 == CboxError::OK);

                CHECK(sp.get().read() == temp_t(21.0));
            }
        }
    }
}

/*
SCENARIO("Create blox SetPointSimple application object from definition"){
    GIVEN("A BrewBlox SetPointSimple definition"){
        blox_SetPointSimple definition;
        definition.setting = 123;

        uint8_t buffer1[100];
        BufferDataOut tempOut(buffer1, sizeof(buffer1));
        CboxError res = streamProtoTo(tempOut, &definition, blox_SetPointSimple_fields, sizeof(buffer1));
        CHECK(res == CboxError::no_error);

        BufferDataIn in(buffer1, sizeof(buffer1));
        obj_type_t typeId = resolveTypeId<SetPointSimpleBlock>();
        CboxError er;

        WHEN("an application object is created form the definition"){
        	std::shared_ptr<Object> obj;
        	obj = createApplicationObject(typeId, in, er);

            THEN("No errors occur"){
                CHECK(obj != nullptr);
                CHECK(asUint8(er) == asUint8(CboxError::no_error));
            }
        }
    }
}

*/
