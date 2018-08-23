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

    WHEN("A connection sends a read object command, it is processed by the Box"){
        std::stringstream in, out;
        connSource.add(in, out);
        in << "010100\n"; // read object 1
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 00 no_error - 0100 obj_id 0001 - FF profile - E803 obj type 1000 (0x03e8) - 11111111 obj data
        CHECK(out.str() == "010100|000100FFE80311111111\n");
    }

    WHEN("A connection sends a write object command, it is processed by the Box"){
        std::stringstream in, out;
        connSource.add(in, out);
        in << "02010001E80333333333\n"; // write object 1, set profiles to 01 and value to 33333333
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 00 no_error - 0100 obj_id 0001 - 01 profile - E803 obj type 1000 (0x03e8) - 33333333 obj data
        CHECK(out.str() == "02010001E80333333333|00010001E80333333333\n");
    }

    WHEN("A connection sends a create object command, it is processed by the Box"){
        std::stringstream in, out;
        connSource.add(in, out);
        in << "03" << "0000" << "FF" << "E803" << "44444444\n"; // create object, ID assigned by box, profiles FF, type 1000, value 44444444
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 00 no_error - 6400 obj_id 100 - FF profile - E803 obj type 1000 (0x03e8) - 44444444 obj data
        CHECK(out.str() == "030000FFE80344444444|006400FFE80344444444\n");
        CHECK(box.getObject(100).lock());

        AND_WHEN("A connection sends a delete object command for a user object, it is processed by the Box"){
            std::stringstream in, out;
            connSource.add(in, out);
            in << "04" << "6400\n"; // delete object, ID 100
            box.hexCommunicate();

            // commands are sent out LSB first
            // command repetition | 00 no_error
            CHECK(out.str() == "046400|00\n");
            CHECK(!box.getObject(100).lock());
        }
    }

    WHEN("A connection sends a delete object command for system object, it is refused with status object_not_deletable (35)"){
        std::stringstream in, out;
        connSource.add(in, out);
        in << "04" << "0100\n"; // delete object, ID 1
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 23 object_not_deletable
        CHECK(out.str() == "040100|23\n");
    }

    WHEN("A connection sends a delete object command for a non-existing object, it is refused with status invalid_object_id (65)"){
        std::stringstream in, out;
        connSource.add(in, out);
        in << "04" << "0001\n"; // delete object, ID 256
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 41 invalid_object_id
        CHECK(out.str() == "040001|41\n");
    }

    WHEN("A connection sends a list objects command, all objects are sent out"){
        std::stringstream in, out;
        connSource.add(in, out);
        in << "05\n"; // list all objects
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | status, obj1, obj2, profiles
        CHECK(out.str() == "05|00,0100FFE80311111111,0200FFE80322222222,0300FF020001\n");
    }

    WHEN("An object is created with a profile that is not active, it's value is written to storage, but the runtime object is still inactive"){

    }


    WHEN("Objects are created in different profiles"){
    	CHECK(box.getActiveProfiles() == 0x01); // default is only profile 0 active

    	std::stringstream in, out;
		connSource.add(in, out);

		in << "03" << "6400" << "01" << "E803" << "44444444\n"; // create object, ID 100, profiles 01, type 1000, value 44444444
		in << "03" << "6500" << "02" << "E803" << "44444444\n"; // create object, ID 101, profiles 02, type 1000, value 55555555
		in << "03" << "6600" << "03" << "E803" << "44444444\n"; // create object, ID 102, profiles 03, type 1000, value 66666666
		in << "02" << "0200" << "00" << "E803" << "12341234\n"; // write system object 2 (including 00 for profiles)


		box.hexCommunicate();

		WHEN("Multiple commands are sent over a connection, separated by \n, they are all processed"){
			REQUIRE(box.getObject(100).lock());
			REQUIRE(box.getObject(101).lock());
			REQUIRE(box.getObject(102).lock());

            THEN("System object 2 was written, but it is not deactivated, because it is a system object)"){
                std::stringstream in2, out2;
                connSource.add(in2, out2);
                in2 << "010200\n"; // read object 02
                box.hexCommunicate();
                CHECK(out2.str() == "010200|00020000E80312341234\n");
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
		    std::stringstream in2, out2;
            connSource.add(in2, out2);
		    in2 << "02" << "0300" << "FF" << "0200" << "02\n"; // write Profiles object 02 with value 02
            box.hexCommunicate();
            CHECK(out2.str() == "020300FF020002|000300FF020002\n");
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
                std::stringstream in2, out2;
                connSource.add(in2, out2);
                in2 << "05\n"; // list all objects
                box.hexCommunicate();
                // object 100 (0x64) is listed as inactive object of actual type 0xE803: 6400 - 01 - 0100 - E803
                std::string expected = "05|00,0100FFE80311111111,020000E80312341234,0300FF020002,6400010100E803,650002E80344444444,660003E80344444444\n";
                CHECK(out2.str() == expected);

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

                    std::stringstream in3, out3;
                    connSource2.add(in3, out3);
                    in3 << "05\n"; // list all objects
                    box2.hexCommunicate();

                    CHECK(out3.str() == expected);
                }
            }
		}

		THEN("When read directly, inactive objects also stream their EEPROM data"){
		    std::stringstream in2, out2;
            connSource.add(in2, out2);
            in2 << "016500\n"; // read object 101
            box.hexCommunicate();

            std::string expected = "016500|006500020100E803,02E80344444444";
            std::string got = out2.str();

            // storage will append some extra bytes as reserved space, so we ignore these in the check
            auto res = std::mismatch(expected.begin(), expected.end(), got.begin());
            CHECK(res.first == expected.end());
		}

		THEN("When the clear objects command is received, all user objects are removed, system objects remain"){
		    std::stringstream in2, out2;
            connSource.add(in2, out2);
            in2 << "07\n"; // clear all objects
            box.hexCommunicate();

            CHECK(out2.str() == "07|00\n");

            std::stringstream in3, out3;
            connSource.add(in3, out3);
            in3 << "05\n"; // list all objects
            box.hexCommunicate();

            // only the system objects remain
            CHECK(out3.str() == "05|00,0100FFE80311111111,020000E80312341234,0300FF020001\n");
		}
    }
}
