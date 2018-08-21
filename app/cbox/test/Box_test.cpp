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

    container.setObjectsStartId(100); // objects with id < 100 are system objects

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
        in << "010100"; // read object 1
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 00 no_error - 0100 obj_id 0001 - FF profile - E803 obj type 1000 (0x03e8) - 11111111 obj data
        CHECK(out.str() == "010100|000100FFE80311111111\r\n");
    }

    WHEN("A connection sends a write object command, it is processed by the Box"){
        std::stringstream in, out;
        connSource.add(in, out);
        in << "02010001E80333333333"; // write object 1, set profiles to 01 and value to 33333333
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 00 no_error - 0100 obj_id 0001 - 01 profile - E803 obj type 1000 (0x03e8) - 33333333 obj data
        CHECK(out.str() == "02010001E80333333333|00010001E80333333333\r\n");
    }

    WHEN("A connection sends a create object command, it is processed by the Box"){
        std::stringstream in, out;
        connSource.add(in, out);
        in << "03" << "0000" << "FF" << "E803" << "44444444"; // create object, ID assigned by box, profiles FF, type 1000, value 44444444
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 00 no_error - 6400 obj_id 100 - FF profile - E803 obj type 1000 (0x03e8) - 44444444 obj data
        CHECK(out.str() == "030000FFE80344444444|006400FFE80344444444\r\n");

        AND_WHEN("A connection sends a delete object command for a user object, it is processed by the Box"){
            std::stringstream in, out;
            connSource.add(in, out);
            in << "04" << "6400"; // delete object, ID 100
            box.hexCommunicate();

            // commands are sent out LSB first
            // command repetition | 00 no_error
            CHECK(out.str() == "046400|00\r\n");
        }
    }

    WHEN("A connection sends a delete object command for system object, it is refused with status object_not_deletable (35)"){
        std::stringstream in, out;
        connSource.add(in, out);
        in << "04" << "0100"; // delete object, ID 1
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 23 object_not_deletable
        CHECK(out.str() == "040100|23\r\n");
    }

    WHEN("A connection sends a delete object command for a non-existing object, it is refused with status invalid_object_id (65)"){
        std::stringstream in, out;
        connSource.add(in, out);
        in << "04" << "0001"; // delete object, ID 256
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | 41 invalid_object_id
        CHECK(out.str() == "040001|41\r\n");
    }

    WHEN("A connection sends a list objects command, all objects are sent out"){
        std::stringstream in, out;
        connSource.add(in, out);
        in << "05"; // list all objects
        box.hexCommunicate();

        // commands are sent out LSB first
        // command repetition | status, obj1, obj2
        CHECK(out.str() == "05|00,0100FFE80311111111,0200FFE80322222222\r\n");
    }
}
