#include "ArrayEepromAccess.h"
#include "Box.h"
#include "Connections.h"
#include "ConnectionsStringStream.h"
#include "DataStreamConverters.h"
#include "EepromAccess.h"
#include "EepromObjectStorage.h"
#include "Object.h"
#include "ObjectContainer.h"
#include "ObjectFactory.h"
#include "ResolveType.h"
#include "TestObjects.h"
#include "catch.hpp"
#include "testinfo.h"
#include <iomanip>
#include <iostream>

using namespace cbox;

SCENARIO("A controlbox Box")
{
    ObjectContainer container = {
        ContainedObject(1, 0xFF, std::make_shared<LongIntObject>(0x11111111)),
        ContainedObject(2, 0xFF, std::make_shared<LongIntObject>(0x22222222))};

    EepromAccess eeprom;
    EepromObjectStorage storage(eeprom);
    ObjectFactory factory = {
        OBJECT_FACTORY_ENTRY(LongIntObject),
        OBJECT_FACTORY_ENTRY(LongIntVectorObject),
        OBJECT_FACTORY_ENTRY(UpdateCounter),
        {cbox::resolveTypeId<PtrLongIntObject>(), [&container]() {
             return std::make_unique<PtrLongIntObject>(container);
         }}};

    StringStreamConnectionSource connSource;
    ConnectionPool connPool = {connSource};

    Box box(factory, container, storage, connPool);

    std::stringstream in, out, expected;
    connSource.add(in, out);

    auto clearStreams = [&in, &out, &expected]() {
        in.str("");
        in.clear();
        out.str("");
        out.clear();
        expected.str("");
        expected.clear();
    };

    WHEN("A connection sends a read object command, it is processed by the Box")
    {
        in << "010100"; // read object 1
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("010100")
                 << "|" << addCrc("00"        // no error
                                  "0100"      // object id 1
                                  "FF"        // profiles 0xFF
                                  "E803"      // object type 1000
                                  "11111111") // object data
                 << "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("A connection sends a read object command for a non-existing object, INVALID_OBJECT_ID is returned")
    {
        in << "010800"; // read object 8
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("010800")
                 << "|" << addCrc("40")
                 << "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("A connection sends a read stored object command for a non-existing object, INVALID_OBJECT_ID is returned")
    {
        in << "060800"; // read stored object 8
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("060800")
                 << "|" << addCrc("40")
                 << "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("A connection sends a write object command, it is processed by the Box")
    {
        in << "02010001E80333333333"; // write object 1, set profiles to 01 and value to 33333333
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("02010001E80333333333")
                 << "|" << addCrc("00"        // no error
                                  "0100"      // object id 1
                                  "01"        // profiles 0x01
                                  "E803"      // object type 1000
                                  "33333333") // object data
                 << "\n";
        CHECK(out.str() == expected.str());

        AND_WHEN("A connection sends a write object command with a wrong object type, INVALID_OBJECT_TYPE is returned and the object unchanged")
        {
            clearStreams();
            in << "02010001E90333333333"; // E903 instead of E803 is used
            in << crc(in.str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("02010001E90333333333")
                     << "|" << addCrc("41"        // INVALID_OBJECT_TYPE
                                      "0100"      // object id 1
                                      "01"        // profiles 0x01
                                      "E803"      // object type 1000
                                      "33333333") // object data
                     << "\n";
            CHECK(out.str() == expected.str());
        }
    }

    WHEN("A connection sends a create object command, it is processed by the Box")
    {
        in << "03"        // create object
           << "0000"      // ID assigned by box
           << "FF"        // profiles FF
           << "E803"      // type 1000
           << "44444444"; // value 44444444
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("030000FFE80344444444") << "|"
                 << addCrc("00"        // status
                           "6400"      // id
                           "FF"        // profiles
                           "E803"      // type
                           "44444444") // data
                 << "\n";
        CHECK(out.str() == expected.str());
        CHECK(box.getObject(100).lock());

        AND_WHEN("A connection sends a delete object command for a user object, it is processed by the Box")
        {
            clearStreams();

            in << "04"
               << "6400"; // ID 100
            in << crc(in.str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("046400")
                     << "|" << addCrc("00")
                     << "\n";
            CHECK(out.str() == expected.str());
            CHECK(!box.getObject(100).lock());
        }
    }

    WHEN("A connection sends a create object command with an ID in the system range, it is refused with INVALID_OBJECT_ID")
    {
        in << "03"        // create object
           << "0500"      // ID 5
           << "FF"        // profiles FF
           << "E803"      // type 1000
           << "44444444"; // value 44444444
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("030500FFE80344444444") << "|" << addCrc("40") << "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("A connection sends a delete object command for system object, it is refused with status object_not_deletable (35)")
    {
        in << "04"
           << "0100"; // delete object, ID 1
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("040100")
                 << "|" << addCrc("23") // error code for object_not_deletable
                 << "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("A connection sends a delete object command for a non-existing object, it is refused with status invalid_object_id (65)")
    {
        in << "04"
           << "0001"; // delete object, ID 256
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("040001")
                 << "|" << addCrc("40") << "\n"; // error code for invalid object id
        CHECK(out.str() == expected.str());
    }

    WHEN("A connection sends a list objects command, all objects are sent out")
    {
        in << "05"; // list all objects
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("05")
                 << "|" << addCrc("00")
                 << "," << addCrc("0100FFE80311111111")
                 << "," << addCrc("0200FFE80322222222")
                 << "," << addCrc("0300FF020001")
                 << "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("A connection sends a noop command, it receives a reply.")
    {
        in << "00"; // list all objects
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("00")
                 << "|" << addCrc("00")
                 << "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("A connection sends an invalid command, it receives a reply with error code invalid command.")
    {
        in << "99";
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("99")
                 << "|" << addCrc("3F")
                 << "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("An object is created with a profile that is not active, it's value is written to storage, but the runtime object is still inactive")
    {
        clearStreams();

        in << "03"        // create object
           << "0000"      // id assigned by box
           << "00"        // active in no profiles
           << "E803"      // typeid 1000
           << "44444444"; // value
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("03000000E80344444444") << "|" // command repetition
                 << addCrc("00"                           // status OK
                           "6400"                         // id 100
                           "0001"                         // type InactiveObject
                           "00"                           // profiles 0x00
                           "E803")                        // actual type 1000
                 << "\n";

        CHECK(out.str() == expected.str());
        CHECK(box.getObject(100).lock());

        THEN("The stored settings can still be read with the READ_STORED_OBJECT command")
        {
            clearStreams();

            in << "066400"; // read stored object 100
            in << crc(in.str()) << "\n";
            box.hexCommunicate();

            // Objects are stored with a CRC in EEPROM.
            // The message will have 2 CRCs appended.
            // This allows to distinguish between an error in EEPROM or a communication error.

            expected << addCrc("066400") << "|"
                     << addCrc("00"        // status
                               "6400"      // id
                               "00"        // stored profiles
                               "E803"      // stored type
                               "44444444") // stored data
                     << "\n";
            CHECK(out.str() == expected.str());
        }

        THEN("The object can still be written as the original object type, but the reply still contains an inactive object")
        {
            clearStreams();

            in << "02"        // write object
               << "6400"      // id 100
               << "00"        // active in no profiles
               << "E803"      // typeid 1000
               << "14141414"; // value
            in << crc(in.str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("02640000E80314141414") << "|" // command repetition
                     << addCrc("00"                           // status OK
                               "6400"                         // id 100
                               "0001"                         // type InactiveObject
                               "00"                           // profiles 0x00
                               "E803")                        // actual type 1000
                     << "\n";

            CHECK(out.str() == expected.str());

            AND_THEN("But the stored data will be updated")
            {
                clearStreams();

                in << "066400"; // read stored object 100
                in << crc(in.str()) << "\n";
                box.hexCommunicate();

                expected << addCrc("066400") << "|"
                         << addCrc("00"        // status
                                   "6400"      // id
                                   "00"        // stored profiles
                                   "E803"      // stored type
                                   "14141414") // stored data
                         << "\n";
                CHECK(out.str() == expected.str());
            }
        }
    }

    WHEN("Objects are created in different profiles")
    {
        CHECK(box.getActiveProfiles() == 0x01); // default is only profile 0 active

        in << "03"
           << "6400"
           << "01"
           << "E803"
           << "44444444"; // create object, ID 100, profiles 01, type 1000, value 44444444
        in << crc(in.str()) << "\n";

        box.hexCommunicate();
        clearStreams();

        in << "03"
           << "6500"
           << "02"
           << "E803"
           << "44444444"; // create object, ID 101, profiles 02, type 1000, value 55555555
        in << crc(in.str()) << "\n";

        box.hexCommunicate();
        clearStreams();

        in << "03"
           << "6600"
           << "03"
           << "E803"
           << "44444444"; // create object, ID 102, profiles 03, type 1000, value 66666666
        in << crc(in.str()) << "\n";

        box.hexCommunicate();
        clearStreams();

        in << "02"
           << "0200"
           << "00"
           << "E803"
           << "12341234"; // write system object 2 (including 00 for profiles)
        in << crc(in.str()) << "\n";

        box.hexCommunicate();
        clearStreams();

        REQUIRE(box.getObject(100).lock());
        REQUIRE(box.getObject(101).lock());
        REQUIRE(box.getObject(102).lock());

        THEN("System object 2 was written, but it is not deactivated, because it is a system object)")
        {
            in << "010200"; // read object 02
            in << crc(in.str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("010200") << "|" << addCrc("00020000E80312341234") << "\n";
            CHECK(out.str() == expected.str());
        }

        THEN("Only objects in an active profile are active, the others have the InactiveObject placeholder")
        {
            CHECK(box.getObject(100).lock()->typeId() == resolveTypeId<LongIntObject>());
            CHECK(box.getObject(101).lock()->typeId() == resolveTypeId<InactiveObject>());
            CHECK(box.getObject(102).lock()->typeId() == resolveTypeId<LongIntObject>());
        }

        WHEN("An object is given an active profiles value that would disable it, it is replaced by InactiveObject")
        {
            in << "02"        // command
               << "6400"      // id
               << "00"        // profiles of object
               << "E803"      // type
               << "12341234"; // value
            in << crc(in.str()) << "\n";
            box.hexCommunicate();
            CHECK(box.getObject(100).lock()->typeId() == resolveTypeId<InactiveObject>());
        }

        WHEN("The active profiles setting is changed (through the persisted block representing it)")
        {
            in << "02"   // command
               << "0300" // id
               << "FF"   // profiles of object
               << "0200" // type
               << "02";  // value
            in << crc(in.str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("020300FF020002") << "|" << addCrc("000300FF020002") << "\n";
            CHECK(out.str() == expected.str());
            CHECK(box.getActiveProfiles() == 0x2);

            THEN("Objects that are not active anymore are replaced with Inactive Object")
            {
                CHECK(box.getObject(100).lock()->typeId() == resolveTypeId<InactiveObject>());
            }
            THEN("Objects that where not active but should be are loaded from storage")
            {
                CHECK(box.getObject(101).lock()->typeId() == resolveTypeId<LongIntObject>());
            }
            THEN("Objects that should remain active are still active")
            {
                CHECK(box.getObject(102).lock()->typeId() == resolveTypeId<LongIntObject>());
            }
            THEN("The objects are listed correctly")
            {
                clearStreams();

                in << "05"; // list all objects
                in << crc(in.str()) << "\n";
                box.hexCommunicate();

                expected << addCrc("05") << "|"
                         << addCrc("00") << ","
                         << addCrc("0100FFE80311111111")
                         << "," << addCrc("020000E80312341234")
                         << "," << addCrc("0300FF020002")
                         << "," << addCrc("6400010100E803") // object 100 (0x64) is listed as inactive object of actual type 0xE803: 6400 - 01 - 0100 - E803
                         << "," << addCrc("650002E80344444444")
                         << "," << addCrc("660003E80344444444")
                         << "\n";
                CHECK(out.str() == expected.str());

                auto listObjectsOriginal = expected.str();

                AND_WHEN("A new box is created from existing storage (for example after a reboot)")
                {
                    // note that only eeprom is not newly created here
                    ObjectContainer container2 = {
                        ContainedObject(1, 0xFF, std::make_shared<LongIntObject>(0x11111111)),
                        ContainedObject(2, 0xFF, std::make_shared<LongIntObject>(0x22222222))};

                    EepromObjectStorage storage2(eeprom);
                    ObjectFactory factory2 = {
                        OBJECT_FACTORY_ENTRY(LongIntObject),
                        OBJECT_FACTORY_ENTRY(LongIntVectorObject)};

                    StringStreamConnectionSource connSource2;
                    ConnectionPool connPool2 = {connSource2};

                    THEN("All objects are restored from storage")
                    {
                        Box box2(factory2, container2, storage2, connPool2);

                        std::stringstream in2, out2;
                        connSource2.add(in2, out2);

                        in2 << "05"; // list all objects
                        in2 << crc(in2.str()) << "\n";
                        box2.hexCommunicate();

                        CHECK(out2.str() == expected.str());
                    }

                    THEN("Invalid EEPROM data is handled correctly due to CRC checking")
                    {
                        // Lambda that finds replaces something in EEPROM, given as hex string
                        auto eepromReplace = [&eeprom](std::string from, std::string to) -> bool {
                            // copy the eeprom
                            const uint16_t start = 32;
                            const uint16_t length = 200;
                            uint8_t eepromCopy[length];
                            eeprom.readBlock(eepromCopy, start, length);

                            // convert to a hex string
                            std::stringstream ssEepromAsHex;

                            for (uint8_t* b = eepromCopy; b < &eepromCopy[length]; ++b) {
                                ssEepromAsHex << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << +*b;
                            }

                            std::string eepromAsHex = ssEepromAsHex.str();
                            REQUIRE(eepromAsHex.size() == 2 * length);

                            // Replace the substring
                            size_t pos = eepromAsHex.find(from);
                            if (pos == std::string::npos) {
                                return false;
                            }

                            eepromAsHex.replace(pos, from.length(), to);
                            REQUIRE(eepromAsHex.size() == 2 * length); // replacement should not alter EEPROM length

                            // Put the data back
                            uint16_t idx = start;
                            for (auto it = eepromAsHex.begin(); it < eepromAsHex.end() - 1; ++it, ++it) {
                                eeprom.writeByte(idx++, (h2d(*it) << 4) + h2d(*(it + 1)));
                            }

                            return true;
                        };

                        THEN("Check that the lambda works")
                        {
                            CHECK(eepromReplace("650002E80344444444", "650002E80344444444"));  // old string is found, not replaced
                            CHECK(eepromReplace("650002E80344444444", "650002E80344554444"));  // old string is found, and replaced
                            CHECK(eepromReplace("650002E80344554444", "650002E80344554444"));  // new string is found, not replaced
                            CHECK(!eepromReplace("650002E80344444444", "650002E80344444444")); // original is not found
                        }

                        WHEN("Object data has changed, it results in a CRC error and only the damaged object is not loaded")
                        {
                            std::string originalObject = "650002E80344444444";
                            std::string damagedObject = "650002E80344554444";

                            CHECK(eepromReplace(originalObject, damagedObject) == true);

                            Box box2(factory2, container2, storage2, connPool2);

                            std::stringstream in2, out2;
                            connSource2.add(in2, out2);

                            in2 << "05"; // list all objects
                            in2 << crc(in2.str()) << "\n";
                            box2.hexCommunicate();

                            // remove object with CRC error from expected string
                            std::string toRemove = "," + addCrc("650002E80344444444");
                            size_t pos = listObjectsOriginal.find(toRemove);
                            auto listObjectsWithObjectMissing = listObjectsOriginal.replace(pos, toRemove.length(), ""); // remove single element

                            CHECK(out2.str() == listObjectsWithObjectMissing);

                            AND_THEN("The EEPROM data can still be retreived with READ_STORED_VALUE")
                            {
                                in2.str("");
                                in2.clear();
                                out2.str("");
                                out2.clear();
                                expected.str("");
                                expected.clear();

                                in2 << "066500"; // read stored object 101
                                in2 << crc(in2.str()) << "\n";
                                box2.hexCommunicate();

                                expected << addCrc("066500") << "|"
                                         << addCrc("00"             // status
                                                   + damagedObject) // obj data
                                         << "\n";
                                CHECK(out2.str() == expected.str());
                            }
                        }
                    }
                }
            }
        }

        THEN("When list stored objects command is received, eeprom data for all objects is streamed")
        {
            clearStreams();
            in << "07"; // list stored objects
            in << crc(in.str()) << "\n";
            box.hexCommunicate();

            // each object has 2 CRCs: one from EEPROM and from the message part
            expected << addCrc("07") << "|" << addCrc("00")
                     << "," << addCrc("640001E80344444444")
                     << "," << addCrc("650002E80344444444")
                     << "," << addCrc("660003E80344444444")
                     << "," << addCrc("020000E80312341234") // modified system object is also persisted
                     << "\n";

            CHECK(out.str() == expected.str());
        }

        THEN("When the clear objects command is received, all user objects are removed, system objects remain")
        {
            clearStreams();
            in << "08"; // clear all objects
            in << crc(in.str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("08") << "|"
                     << addCrc("00") << "\n";
            CHECK(out.str() == expected.str());

            clearStreams();
            in << "05"; // list all objects
            in << crc(in.str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("05") << "|" << addCrc("00") << "," << addCrc("0100FFE80311111111") << "," << addCrc("020000E80312341234") << "," << addCrc("0300FF020001") << "\n";
            CHECK(out.str() == expected.str());
        }
    }

    THEN("Objects update at their requested interval")
    {
        box.update(0);
        // create 2 counter objects with different update intervals
        // object creation and write also triggers an object update

        clearStreams();
        in << "03"    // create counter object
           << "6400"  // ID 100
           << "FF"    // profiles FF
           << "EA03"  // type 1002
           << "E803"; // interval 1000
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("036400FFEA03E803") << "|" // command repetition
                 << addCrc("00"                       // status OK
                           "6400"                     // id 100
                           "FF"                       // profiles 0xFF
                           "EA03"                     // type 1002
                           "E803"                     // interval 1000
                           "0100")                    // count 1
                 << "\n";

        CHECK(out.str() == expected.str());

        clearStreams();
        in << "03"    // create counter object
           << "6500"  // ID 101
           << "FF"    // profiles FF
           << "EA03"  // type 1002
           << "D007"; // interval 2000
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("036400FFEA03D007") << "|" // command repetition
                 << addCrc("00"                       // status OK
                           "6500"                     // id 101
                           "FF"                       // profiles 0xFF
                           "EA03"                     // type 1002
                           "D007"                     // interval 2000
                           "0100")                    // count 1
                 << "\n";

        auto counterObjPtr1 = box.getObject(100).lock();
        auto counterObjPtr2 = box.getObject(101).lock();

        REQUIRE(counterObjPtr1);
        REQUIRE(counterObjPtr2);

        // cast the Object pointers to the actual type for easier testing
        auto counter1 = static_cast<UpdateCounter*>(counterObjPtr1.get());
        auto counter2 = static_cast<UpdateCounter*>(counterObjPtr2.get());

        THEN("Update was called once on each object when they were created")
        {
            CHECK(counter1->count() == 1);
            CHECK(counter2->count() == 1);
            CHECK(counter1->interval() == 1000);
            CHECK(counter2->interval() == 2000);
        }

        for (uint32_t now = 1; now <= 10001; now += 1) {
            box.update(now);
        }

        THEN("After simulating 10 seconds, counter1 was updated 10 more times and counter2 was updated 5 more times")
        {
            CHECK(counter1->count() == 1 + 10);
            CHECK(counter2->count() == 1 + 5);
        }

        box.update(10500); // update just before write
        clearStreams();
        in << "02"    // write counter object
           << "6400"  // ID 100
           << "FF"    // profiles FF
           << "EA03"  // type 1002
           << "A00F"; // interval 4000
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        CHECK(counter1->interval() == 4000);

        THEN("Writing a new interval (4000) to counter 1 has triggered an update of the object")
        {
            CHECK(counter1->count() == 10 + 1 + 1);
            CHECK(counter2->count() == 5 + 1);
        }

        THEN("It is updated 4000 ms after the write and 4000 ms after that")
        {
            uint32_t now;

            for (now = 10500; now <= 20000; now += 1) {
                box.update(now);
                if (counter1->count() > 12) {
                    break;
                }
            }
            CHECK(now == 14500);

            for (now = 14500; now <= 20000; now += 1) {
                box.update(now);
                if (counter1->count() > 13) {
                    break;
                }
            }
            CHECK(now == 18500);
        }

        THEN("An overflowing time is handled correctly")
        {
            update_t tenSecondsBeforeOverflow = std::numeric_limits<update_t>::max() - 10000;
            update_t now = tenSecondsBeforeOverflow;

            box.forcedUpdate(now);
            auto count = counter1->count();

            // run for 20 seconds
            for (; now != 10000; ++now) {
                box.update(now);
            }

            CHECK(counter1->count() == count + 20000 / counter1->interval());
        }
    }

    WHEN("An object with links to other objects is created, it can use data from those other objects")
    {
        in << "03"    // create object
           << "6400"  // ID 100
           << "FF"    // profiles FF
           << "ED03"  // type 1005
           << "0100"  // ptr 1 points to object 1
           << "0200"; // ptr 2 points to object 2

        std::string command = in.str();

        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected
            << addCrc(command) << "|" // command repetition
            << addCrc("00"            // status OK
                      "6400"          // id 100
                      "FF"            // profiles 0xFF
                      "ED03"          // type 1002
                      "0100"          // ptr 1 points to ID 1
                      "0200"          // ptr 2 points to ID 2
                      "01"            // ptr1 OK
                      "01"            // ptr2 OK
                      "11111111"      // value 1
                      "22222222"      // value 2
                      )
            << "\n";

        CHECK(out.str() == expected.str());

        AND_THEN("If the links change and point to an invalid object, the output is correct")
        {
            clearStreams();

            in << "02"    // write object
               << "6400"  // ID 100
               << "FF"    // profiles FF
               << "ED03"  // type 1005
               << "0200"  // ptr 1 points to object 2
               << "0300"; // ptr 2 points to object 3

            std::string command = in.str();

            in << crc(in.str()) << "\n";
            box.hexCommunicate();

            expected
                << addCrc(command) << "|" // command repetition
                << addCrc("00"            // status OK
                          "6400"          // id 100
                          "FF"            // profiles 0xFF
                          "ED03"          // type 1002
                          "0200"          // ptr 1 points to ID 1
                          "0300"          // ptr 2 points to ID 2
                          "01"            // ptr1 OK
                          "00"            // ptr2 not OK
                          "22222222"      // value 1
                          "00000000"      // value 2 is default value 0
                          )
                << "\n";

            CHECK(out.str() == expected.str());
        }
    }

    WHEN("A connection sends only a partial message, a CRC error is returned")
    {
        in << "03"   // create object
           << "0000" // ID assigned by box
           << "FF";  // profiles FF
                     //<< "E803"      // type 1000
                     //<< "44444444"; // value 44444444
                     // in << crc(in.str()) << "\n";
        box.hexCommunicate();

        expected << "030000FF"
                 << "|"
                 << addCrc("43") << "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("A connection sends only a partial message with half a hex encoded byte (1 nibble), a CRC error is returned")
    {
        in << "03" // create object
           << "0"; // ID assigned by box

        box.hexCommunicate();

        expected << "0300"
                 << "|"
                 << addCrc("43") << "\n";
        CHECK(out.str() == expected.str());
    }

    WHEN("All commands are sent commands with invalid CRC, CRC errors are returned")
    {
        for (uint8_t c = 1; c <= 10; ++c) {
            clearStreams();
            in << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << +c;
            in << "0000000000";
            in << crc(in.str() + "10");

            box.hexCommunicate();
            INFO(out.str());
            CHECK(out.str().find("|43") != std::string::npos);
        }
    }

    WHEN("A reboot command is sent, the reset handler is called")
    {
        CHECK(testInfo.rebootCount == 0);

        in << "09" // reboot
           << crc(in.str()) << "\n";

        box.hexCommunicate();

        CHECK(testInfo.rebootCount == 1);
    }

    WHEN("a factory reset command is sent, EEPROM is reinitialized, followed by a system reboot")
    {
        auto rebootCountBeforeCommand = testInfo.rebootCount;

        in << "03"        // create object
           << "0000"      // ID assigned by box
           << "FF"        // profiles FF
           << "E803"      // type 1000
           << "44444444"; // value 44444444
        in << crc(in.str()) << "\n";
        box.hexCommunicate();

        clearStreams();

        auto countNonZeroBytes = [&eeprom]() {
            uint16_t nonZeroBytesInEeprom = 0;
            for (uint16_t i = 32; i < eeprom.length(); ++i) {
                if (eeprom.readByte(i) != 0) {
                    ++nonZeroBytesInEeprom;
                }
            }
            return nonZeroBytesInEeprom;
        };

        CHECK(storage.freeSpace() < 2000);
        CHECK(countNonZeroBytes() > 10);

        in << "0A" // factory reset
           << crc(in.str()) << "\n";

        box.hexCommunicate();

        CHECK(countNonZeroBytes() == 3); // just the disposed block header
        CHECK(storage.freeSpace() == 2013);

        CHECK(testInfo.rebootCount == rebootCountBeforeCommand + 1);
    }
}
