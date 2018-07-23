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

#include "Object.h"
#include "Commands.h"
#include "temperatureFormats.h"
#include <memory>

namespace cbox {
std::shared_ptr<Object> createApplicationObject(obj_type_t typeId, DataIn& in, CommandError& errorCode);
}


std::ostream& operator<<( std::ostream& os, const cbox::Object::StreamFromResult& item )
{
  os << static_cast<std::underlying_type<cbox::Object::StreamFromResult>::type>(item);
  return os;
}

std::ostream& operator<<( std::ostream& os, const cbox::Object::StreamToResult& item )
{
  os << static_cast<std::underlying_type<cbox::Object::StreamToResult>::type>(item);
  return os;
}

SCENARIO("A Bloc SetPointSimple object can be created from streamed protobuf data"){
    GIVEN("a protobuf message defining a SetPointSimple object"){
        blox_SetPointSimple_Persisted message;
        message.settings.value = 123;

        WHEN("it is encoded to a buffer"){
            uint8_t buf[100];
            pb_ostream_t stream = pb_ostream_from_buffer(buf, sizeof(buf));
            bool status = pb_encode(&stream, blox_SetPointSimple_Persisted_fields, &message);
            THEN("no errors occur"){
                if (!status)
                {
                    INFO("Decoding failed: " << PB_GET_ERROR(&stream));
                    CAPTURE(stream);
                }
                CHECK(status);
            }

            AND_WHEN("we create a DataIn object form that buffer"){
            	cbox::BufferDataIn in(buf);
            	cbox::RegionDataIn regionIn(in, stream.bytes_written); // limit stream to valid data for this object

                THEN("a newly created SetPointSimpleBloc object can receive settings from the DataIn stream")
                {
                    SetPointSimpleBlock sp;
                    cbox::Object::StreamFromResult res = sp.streamFrom(regionIn); // use in as mask too, it is not used.
                    CHECK(res == cbox::Object::StreamFromResult::success_persist);
                    temp_t setting = sp.get().read();
                    temp_t valid;
                    valid.setRaw(123);
                    CHECK(setting == valid);

                    AND_THEN("we can stream that bloc object to a DataOut stream")
                    {
                        // change the value for a round trip test
                        sp.get().write(21.0);

                        uint8_t buf2[100];
                        cbox::BufferDataOut out(buf2, sizeof(buf2));
                        cbox::Object::StreamToResult res2 = sp.streamTo(out);
                        CHECK(res2 == cbox::Object::StreamToResult::success);

                        sp.get().write(25.0); // change again, so we can verify the receive
                        CHECK(sp.get().read() == temp_t(25.0));

                        cbox::BufferDataIn in_roundtrip(buf2);
                        cbox::Object::StreamFromResult res3 = sp.streamFrom(in_roundtrip);
                        CHECK((uint8_t) res3 == (uint8_t) cbox::Object::StreamFromResult::success_persist);

                        CHECK(sp.get().read() == temp_t(21.0));
                    }
                }
            }
        }
    }
}

SCENARIO("Create blox SetPointSimple application object from definition"){
    GIVEN("A BrewBlox SetPointSimple definition"){
        bool status;
        blox_SetPointSimple_Persisted persistedData;
        persistedData.settings.value = 123;

        uint8_t buffer1[100];
        pb_ostream_t stream1 = pb_ostream_from_buffer(buffer1, sizeof(buffer1));
        status = pb_encode(&stream1, blox_SetPointSimple_Persisted_fields, &persistedData);

        THEN("no errors occur"){
            if (!status)
            {
                INFO("Encoding failed: " << PB_GET_ERROR(&stream1));
                CAPTURE(stream1);
            }
            CHECK(status);
        }

        cbox::BufferDataIn in(buffer1);
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


