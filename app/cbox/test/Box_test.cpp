#include "catch.hpp"
#include "EepromObjectStorage.h"
#include "ResolveType.h"
#include "Object.h"
#include "Container.h"
#include "ArrayEepromAccess.h"
#include "TestObjects.h"
#include "ObjectFactory.h"
#include "Box.h"
#include "Connections.h"
#include "ConnectionsStringStream.h"
#include "DataStreamConverters.h"

using namespace cbox;

SCENARIO("A controlbox Box"){
    ObjectContainer container = {
            ContainedObject(1, 0xFF, std::make_shared<LongIntObject>(0x11111111)),
            ContainedObject(2, 0xFF, std::make_shared<LongIntObject>(0x22222222))
    };

    EepromAccess eeprom;
    EepromObjectStorage storage(eeprom);
    ObjectFactory factory = {
        OBJECT_FACTORY_ENTRY(LongIntObject),
        OBJECT_FACTORY_ENTRY(LongIntVectorObject)
    };

    StringStreamConnectionSource connSource;
    ConnectionPool connPool = {connSource};

    Box box(factory, container, storage, connPool);

    std::stringstream in, out, expected;
    connSource.add(in, out);

    WHEN("A connection sends a read object command, it is processed by the Box"){
        in << "010100\n"; // read object 1
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 00 no_error - 0100 obj_id 0001 - FF profile - E803 obj type 1000 (0x03e8) - 11111111 obj data
        expected << addCrc("010100") << "|" << addCrc("000100FFE80311111111") << "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("A connection sends a write object command, it is processed by the Box"){
        in << "02010001E80333333333\n"; // write object 1, set profiles to 01 and value to 33333333
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 00 no_error - 0100 obj_id 0001 - 01 profile - E803 obj type 1000 (0x03e8) - 33333333 obj data
        expected << addCrc("02010001E80333333333") << "|" << addCrc("00010001E80333333333") << "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("A connection sends a create object command, it is processed by the Box"){
        in << "03" << "0000" << "FF" << "E803" << "44444444\n"; // create object, ID assigned by box, profiles FF, type 1000, value 44444444
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 00 no_error - 6400 obj_id 100 - FF profile - E803 obj type 1000 (0x03e8) - 44444444 obj data
        expected << addCrc("030000FFE80344444444") << "|" << addCrc("006400FFE80344444444") << "\n";
        CHECK(out.str() == expected.str());
        CHECK(box.getObject(100).lock());

        AND_WHEN("A connection sends a delete object command for a user object, it is processed by the Box"){
            out.str("");
            expected.str("");

            in << "04" << "6400\n"; // delete object, ID 100
            box.hexCommunicate();

            // commands are sent out LSB first
            // command repetition | 00 no_error
            expected << addCrc("046400") << "|" << addCrc("00") << "\n";
            CHECK(out.str() == expected.str());
            CHECK(!box.getObject(100).lock());
        }
    }

    WHEN("A connection sends a delete object command for system object, it is refused with status object_not_deletable (35)"){
        in << "04" << "0100\n"; // delete object, ID 1
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 23 object_not_deletable
        expected << addCrc("040100") << "|" << addCrc("23") << "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("A connection sends a delete object command for a non-existing object, it is refused with status invalid_object_id (65)"){
        in << "04" << "0001\n"; // delete object, ID 256
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 41 invalid_object_id
        expected << addCrc("040001") << "|" << addCrc("41") << "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("A connection sends a list objects command, all objects are sent out"){
        in << "05\n"; // list all objects
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | status, obj1, obj2, profiles
        expected << addCrc("05") << "|" << addCrc("00") <<
                "," << addCrc("0100FFE80311111111") <<
                "," << addCrc("0200FFE80322222222") <<
                "," << addCrc("0300FF020001") <<
                "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("An object is created with a profile that is not active, it's value is written to storage, but the runtime object is still inactive"){

    }


    WHEN("Objects are created in different profiles"){
    	CHECK(box.getActiveProfiles() == 0x01); // default is only profile 0 active

    	in << "03" << "6400" << "01" << "E803" << "44444444\n"; // create object, ID 100, profiles 01, type 1000, value 44444444
		in << "03" << "6500" << "02" << "E803" << "44444444\n"; // create object, ID 101, profiles 02, type 1000, value 55555555
		in << "03" << "6600" << "03" << "E803" << "44444444\n"; // create object, ID 102, profiles 03, type 1000, value 66666666
		in << "02" << "0200" << "00" << "E803" << "12341234\n"; // write system object 2 (including 00 for profiles)


		box.hexCommunicate();

        out.str(""); // discard output

		REQUIRE(box.getObject(100).lock());
        REQUIRE(box.getObject(101).lock());
        REQUIRE(box.getObject(102).lock());


		WHEN("Multiple commands are sent over a connection, separated by \n, they are all processed"){
            THEN("System object 2 was written, but it is not deactivated, because it is a system object)"){
                in << "010200\n"; // read object 02
                box.hexCommunicate();

                expected << addCrc("010200") << "|" << addCrc("00020000E80312341234") << "\n";
                CHECK(out.str() == expected.str());
            }
		}


		THEN("Only objects in an active profile are active, the others have the InactiveObject placeholder"){
		    CHECK(box.getObject(100).lock()->typeId() == resolveTypeId<LongIntObject>());
		    CHECK(box.getObject(101).lock()->typeId() == resolveTypeId<InactiveObject>());
		    CHECK(box.getObject(102).lock()->typeId() == resolveTypeId<LongIntObject>());
		}

		WHEN("An object is given an active profiles value that would disable it, it is replaced by InactiveObject"){
		    in << "02" << "6400" << "00" << "E803" << "12341234\n"; // write user object 100 (including 00 for profiles)
		    box.hexCommunicate();
		    CHECK(box.getObject(100).lock()->typeId() == resolveTypeId<InactiveObject>());
		}

		WHEN("The active profiles setting is changed (through the persisted block representing it)"){
		    in << "02" << "0300" << "FF" << "0200" << "02\n"; // write Profiles object 02 with value 02
            box.hexCommunicate();

            expected << addCrc("020300FF020002") << "|" << addCrc("000300FF020002") << "\n";
            CHECK(out.str() == expected.str());
            CHECK(box.getActiveProfiles() == 0x2);

            THEN("Objects that are not active anymore are replaced with Inactive Object"){
                CHECK(box.getObject(100).lock()->typeId() == resolveTypeId<InactiveObject>());
            }
            THEN("Objects that where not active but should be are loaded from storage"){
                CHECK(box.getObject(101).lock()->typeId() == resolveTypeId<LongIntObject>());
            }
            THEN("Objects that should remain active are still active"){
                CHECK(box.getObject(102).lock()->typeId() == resolveTypeId<LongIntObject>());
            }
            THEN("The objects are listed correctly"){
                out.str("");
                expected.str("");

                in << "05\n"; // list all objects
                box.hexCommunicate();

                expected << addCrc("05") << "|" << addCrc("00") <<
                        "," << addCrc("0100FFE80311111111") <<
                        "," << addCrc("020000E80312341234") <<
                        "," << addCrc("0300FF020002") <<
                        "," << addCrc("6400010100E803") << // object 100 (0x64) is listed as inactive object of actual type 0xE803: 6400 - 01 - 0100 - E803
                        "," << addCrc("650002E80344444444") <<
                        "," << addCrc("660003E80344444444") <<
                        "\n";
                CHECK(out.str() == expected.str());

                AND_WHEN("A new box is created from existing storage (for example after a reboot), all objects are restored"){
                    // note that only eeprom is not newly created here
                    ObjectContainer container2 = {
                            ContainedObject(1, 0xFF, std::make_shared<LongIntObject>(0x11111111)),
                            ContainedObject(2, 0xFF, std::make_shared<LongIntObject>(0x22222222))
                    };

                    EepromObjectStorage storage2(eeprom);
                    ObjectFactory factory2 = {
                        OBJECT_FACTORY_ENTRY(LongIntObject),
                        OBJECT_FACTORY_ENTRY(LongIntVectorObject)
                    };

                    StringStreamConnectionSource connSource2;
                    ConnectionPool connPool2 = {connSource2};

                    Box box2(factory2, container2, storage2, connPool2);

                    std::stringstream in2, out2;
                    connSource2.add(in2, out2);

                    in2 << "05\n"; // list all objects
                    box2.hexCommunicate();

                    CHECK(out2.str() == expected.str());
                }
            }
		}

		THEN("When read directly, inactive objects also stream their EEPROM data"){
		    in << "016500\n"; // read object 101
            box.hexCommunicate();

            expected << addCrc("016500") << "|" <<
                    addCrc("006500020100E803") << "," << // response with inactive object
                    addCrc("02E80344444444") << "\n";  // eeprom data
            CHECK(out.str() == expected.str());
		}

		THEN("When the clear objects command is received, all user objects are removed, system objects remain"){
		    in << "07\n"; // clear all objects
            box.hexCommunicate();

            expected << addCrc("07") << "|" << addCrc("00") << "\n";
            CHECK(out.str() == expected.str());

            in << "05\n"; // list all objects
            box.hexCommunicate();

            expected << addCrc("05") << "|" << addCrc("00") <<
                    "," << addCrc("0100FFE80311111111") <<
                    "," << addCrc("020000E80312341234") <<
                    "," << addCrc("0300FF020001") << "\n";
            CHECK(out.str() == expected.str());
		}
    }
}
