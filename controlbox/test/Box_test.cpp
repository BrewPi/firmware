#include "Box.h"

#include "testinfo.h"
#include <catch.hpp>
#include <iomanip>
#include <iostream>

#include "ArrayEepromAccess.h"
#include "Connections.h"
#include "ConnectionsStringStream.h"
#include "DataStreamConverters.h"
#include "EepromObjectStorage.h"
#include "LongIntScanningFactory.h"
#include "Object.h"
#include "ObjectContainer.h"
#include "ObjectFactory.h"
#include "TestObjects.h"

using namespace cbox;

SCENARIO("A controlbox Box")
{
    ObjectContainer container = {
        // profiles object will have id 1
        ContainedObject(2, 0xFF, std::make_shared<LongIntObject>(0x11111111)),
        ContainedObject(3, 0xFF, std::make_shared<LongIntObject>(0x22222222))};

    ArrayEepromAccess<2048> eeprom;
    EepromObjectStorage storage(eeprom);

    ObjectFactory factory = {
        {LongIntObject::staticTypeId(), std::make_shared<LongIntObject>},
        {LongIntVectorObject::staticTypeId(), std::make_shared<LongIntVectorObject>},
        {UpdateCounter::staticTypeId(), std::make_shared<UpdateCounter>},
        {PtrLongIntObject::staticTypeId(), [&container]() {
             return std::make_shared<PtrLongIntObject>(container);
         }},
        {NameableLongIntObject::staticTypeId(), std::make_shared<NameableLongIntObject>},
    };

    StringStreamConnectionSource connSource;
    ConnectionPool connPool = {connSource};

    auto longIntScanner = std::unique_ptr<ScanningFactory>(new LongIntScanningFactory(container));
    std::vector<std::unique_ptr<ScanningFactory>> scanningFactories;
    scanningFactories.push_back(std::move(longIntScanner));

    Box box(factory, container, storage, connPool, std::move(scanningFactories));

    auto in = std::make_shared<std::stringstream>();
    auto out = std::make_shared<std::stringstream>();
    std::stringstream expected;
    connSource.add(in, out);

    auto clearStreams = [&in, &out, &expected]() {
        in->str("");
        in->clear();
        out->str("");
        out->clear();
        expected.str("");
        expected.clear();
    };

    WHEN("A connection sends a read object command, it is processed by the Box")
    {
        *in << "010200"; // read object 2
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("010200")
                 << "|" << addCrc("00"        // no error
                                  "0200"      // object id 2
                                  "FF"        // profiles 0xFF
                                  "E803"      // object type 1000
                                  "11111111") // object data
                 << "\n";
        CHECK(out->str() == expected.str());
    }

    WHEN("A connection sends a read object command for a non-existing object, INVALID_OBJECT_ID is returned")
    {
        *in << "010800"; // read object 8
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("010800")
                 << "|" << addCrc("40")
                 << "\n";
        CHECK(out->str() == expected.str());
    }

    WHEN("A connection sends a read stored object command for a non-existing object, INVALID_OBJECT_ID is returned")
    {
        *in << "060800"; // read stored object 8
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("060800")
                 << "|" << addCrc("40")
                 << "\n";
        CHECK(out->str() == expected.str());
    }

    WHEN("A connection sends a write object command, it is processed by the Box")
    {
        *in << "02020001E80333333333"; // write object 2, set profiles to 01 and value to 33333333
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("02020001E80333333333")
                 << "|" << addCrc("00"        // no error
                                  "0200"      // object id 2
                                  "01"        // profiles 0x01
                                  "E803"      // object type 1000
                                  "33333333") // object data
                 << "\n";
        CHECK(out->str() == expected.str());

        AND_WHEN("A connection sends a write object command with a wrong object type, INVALID_OBJECT_TYPE is returned and the object unchanged")
        {
            clearStreams();
            *in << "02020001E90333333333"; // E903 instead of E803 is used
            *in << crc(in->str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("02020001E90333333333")
                     << "|" << addCrc("41"        // INVALID_OBJECT_TYPE
                                      "0200"      // object id 2
                                      "01"        // profiles 0x01
                                      "E803"      // object type 1000
                                      "33333333") // object data
                     << "\n";
            CHECK(out->str() == expected.str());
        }
    }

    WHEN("A connection sends a create object command, it is processed by the Box")
    {
        *in << "03"        // create object
            << "0000"      // ID assigned by box
            << "FF"        // profiles FF
            << "E803"      // type 1000
            << "44444444"; // value 44444444
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("030000FFE80344444444") << "|"
                 << addCrc("00"        // status
                           "6400"      // id
                           "FF"        // profiles
                           "E803"      // type
                           "44444444") // data
                 << "\n";
        CHECK(out->str() == expected.str());
        CHECK(box.getObject(100).lock());

        AND_WHEN("A connection sends a delete object command for a user object, it is processed by the Box")
        {
            clearStreams();

            *in << "04"
                << "6400"; // ID 100
            *in << crc(in->str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("046400")
                     << "|" << addCrc("00")
                     << "\n";
            CHECK(out->str() == expected.str());
            CHECK(!box.getObject(100).lock());
        }
    }

    WHEN("A connection sends a create object command with an ID in the system range, it is refused with INVALID_OBJECT_ID")
    {
        *in << "03"        // create object
            << "0500"      // ID 5
            << "FF"        // profiles FF
            << "E803"      // type 1000
            << "44444444"; // value 44444444
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("030500FFE80344444444") << "|" << addCrc("40") << "\n";
        CHECK(out->str() == expected.str());
    }

    WHEN("A connection sends a delete object command for system object, it is refused with status object_not_deletable (35)")
    {
        *in << "04"
            << "0200"; // delete object, ID 2
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("040200")
                 << "|" << addCrc("23") // error code for object_not_deletable
                 << "\n";
        CHECK(out->str() == expected.str());
    }

    WHEN("A connection sends a delete object command for a non-existing object, it is refused with status invalid_object_id (65)")
    {
        *in << "04"
            << "0001"; // delete object, ID 256
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("040001")
                 << "|" << addCrc("40") << "\n"; // error code for invalid object id
        CHECK(out->str() == expected.str());
    }

    WHEN("A connection sends a list objects command, all objects are sent out")
    {
        *in << "05"; // list all objects
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("05")
                 << "|" << addCrc("00")
                 << "," << addCrc("0100FFFEFF01")
                 << "," << addCrc("0200FFE80311111111")
                 << "," << addCrc("0300FFE80322222222")
                 << "\n";
        CHECK(out->str() == expected.str());
    }

    WHEN("A connection sends a list compatible objects command, ids of only compatible objects are returned")
    {
        *in << "03"        // create object
            << "6400"      // ID 100
            << "FF"        // profiles FF
            << "E903"      // type 1001, LongIntVector
            << "0200"      // size 2
            << "33333333"  // value 33333333
            << "44444444"; // value 44444444
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("036400FFE90302003333333344444444") << "|"
                 << addCrc("00"        // status
                           "6400"      // id 100
                           "FF"        // profiles
                           "E903"      // type 1001, LongIntVector
                           "0200"      // size 2
                           "33333333"  // value 33333333
                           "44444444") // value 44444444
                 << "\n";
        CHECK(out->str() == expected.str());
        CHECK(box.getObject(100).lock());

        clearStreams();

        *in << "03"        // create object
            << "6500"      // ID 101
            << "FF"        // profiles FF
            << "EB03"      // type 1003, NameableLongIntObject
            << "44444444"; // value 44444444
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("036500FFEB0344444444") << "|"
                 << addCrc("00"        // status
                           "6500"      // id
                           "FF"        // profiles
                           "EB03"      // type
                           "44444444") // data
                 << "\n";
        CHECK(out->str() == expected.str());
        CHECK(box.getObject(101).lock());

        clearStreams();

        *in << "0B";   // list compatible objects
        *in << "E803"; // typeid for LongIntObject
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("0BE803")
                 << "|" << addCrc("00")
                 << "," << addCrc("0200") // id 2
                 << "," << addCrc("0300") // id 3
                 << "," << addCrc("6500") // id 101
                 << "\n";
        CHECK(out->str() == expected.str());
    }

    WHEN("A connection sends a noop command, it receives a reply.")
    {
        *in << "00"; // list all objects
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("00")
                 << "|" << addCrc("00")
                 << "\n";
        CHECK(out->str() == expected.str());
    }

    WHEN("A connection sends an invalid command, it receives a reply with error code invalid command.")
    {
        *in << "99";
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("99")
                 << "|" << addCrc("3F")
                 << "\n";
        CHECK(out->str() == expected.str());
    }

    WHEN("An object is created with a profile that is not active, it's value is written to storage, but the runtime object is still inactive")
    {
        clearStreams();

        *in << "03"        // create object
            << "0000"      // id assigned by box
            << "00"        // active in no profiles
            << "E803"      // typeid 1000
            << "44444444"; // value
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("03000000E80344444444") << "|" // command repetition
                 << addCrc("00"                           // status OK
                           "6400"                         // id 100
                           "00"                           // profiles 0x00
                           "FFFF"                         // type InactiveObject
                           "E803")                        // actual type 1000
                 << "\n";

        CHECK(out->str() == expected.str());
        CHECK(box.getObject(100).lock());

        THEN("The stored settings can still be read with the READ_STORED_OBJECT command")
        {
            clearStreams();

            *in << "066400"; // read stored object 100
            *in << crc(in->str()) << "\n";
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
            CHECK(out->str() == expected.str());
        }

        THEN("Writing an inactive object as the actual object it replaces, gives invalid object type error")
        {
            clearStreams();

            *in << "02"        // write object
                << "6400"      // id 100
                << "00"        // active in no profiles
                << "E803"      // typeid 1000
                << "14141414"; // value
            *in << crc(in->str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("02640000E80314141414") << "|" // command repetition
                     << addCrc("41"                           // status invalid object type
                               "6400"                         // id 100
                               "00"                           // profiles 0x00
                               "FFFF"                         // type InactiveObject
                               "E803")                        // actual type 1000
                     << "\n";

            CHECK(out->str() == expected.str());

            AND_THEN("The stored data will be unchanged")
            {
                clearStreams();

                *in << "066400"; // read stored object 100
                *in << crc(in->str()) << "\n";
                box.hexCommunicate();

                expected << addCrc("066400") << "|"
                         << addCrc("00"        // status
                                   "6400"      // id
                                   "00"        // stored profiles
                                   "E803"      // stored type
                                   "44444444") // stored data
                         << "\n";
                CHECK(out->str() == expected.str());
            }
        }

        THEN("Writing profiles of an inactive object can re-activate it")
        {
            clearStreams();

            *in << "02"    // write object
                << "6400"  // id 100
                << "FF"    // active in all profiles
                << "FFFF"  // typeid 1000
                << "0000"; // actual type doesn't matter
            *in << crc(in->str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("026400FFFFFF0000") << "|" // command repetition
                     << addCrc("00"                       // status OK
                               "6400"                     // id
                               "FF"                       // profiles
                               "E803"                     // stored type
                               "44444444")                // stored data
                     << "\n";

            CHECK(out->str() == expected.str());

            AND_THEN("The stored data will be contain the new profiles")
            {
                clearStreams();

                *in << "066400"; // read stored object 100
                *in << crc(in->str()) << "\n";
                box.hexCommunicate();

                expected << addCrc("066400") << "|"
                         << addCrc("00"        // status
                                   "6400"      // id
                                   "FF"        // stored profiles
                                   "E803"      // stored type
                                   "44444444") // stored data
                         << "\n";
                CHECK(out->str() == expected.str());
            }
        }
    }

    WHEN("Objects are created in different profiles")
    {
        CHECK(box.getActiveProfiles() == 0x01); // default is only profile 0 active

        *in << "03"
            << "6400"
            << "01"
            << "E803"
            << "44444444"; // create object, ID 100, profiles 01, type 1000, value 44444444
        *in << crc(in->str()) << "\n";

        box.hexCommunicate();
        clearStreams();

        *in << "03"
            << "6500"
            << "02"
            << "E803"
            << "44444444"; // create object, ID 101, profiles 02, type 1000, value 55555555
        *in << crc(in->str()) << "\n";

        box.hexCommunicate();
        clearStreams();

        *in << "03"
            << "6600"
            << "03"
            << "E803"
            << "44444444"; // create object, ID 102, profiles 03, type 1000, value 66666666
        *in << crc(in->str()) << "\n";

        box.hexCommunicate();
        clearStreams();

        *in << "02"
            << "0300"
            << "00"
            << "E803"
            << "12341234"; // write system object 3 (including 00 for profiles)
        *in << crc(in->str()) << "\n";

        box.hexCommunicate();
        clearStreams();

        REQUIRE(box.getObject(100).lock());
        REQUIRE(box.getObject(101).lock());
        REQUIRE(box.getObject(102).lock());

        THEN("System object 3 was written, but it is not deactivated, because it is a system object)")
        {
            *in << "010300"; // read object 03
            *in << crc(in->str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("010300") << "|" << addCrc("00030000E80312341234") << "\n";
            CHECK(out->str() == expected.str());
        }

        THEN("Only objects in an active profile are active, the others have the InactiveObject placeholder")
        {
            CHECK(box.getObject(100).lock()->typeId() == LongIntObject::staticTypeId());
            CHECK(box.getObject(101).lock()->typeId() == InactiveObject::staticTypeId());
            CHECK(box.getObject(102).lock()->typeId() == LongIntObject::staticTypeId());
        }

        WHEN("An object is given an active profiles value that would disable it, it is replaced by InactiveObject")
        {
            *in << "02"        // command
                << "6400"      // id
                << "00"        // profiles of object
                << "E803"      // type
                << "12341234"; // value
            *in << crc(in->str()) << "\n";
            box.hexCommunicate();
            CHECK(box.getObject(100).lock()->typeId() == InactiveObject::staticTypeId());
        }

        WHEN("The active profiles setting is changed (through the persisted block representing it)")
        {
            *in << "02"   // command
                << "0100" // id of profiles object
                << "FF"   // profiles of object
                << "FEFF" // type of profiles object
                << "02";  // value
            *in << crc(in->str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("020100FFFEFF02") << "|" << addCrc("000100FFFEFF02") << "\n";
            CHECK(out->str() == expected.str());
            CHECK(box.getActiveProfiles() == 0x2);

            THEN("Objects that are not active anymore are replaced with Inactive Object")
            {
                CHECK(box.getObject(100).lock()->typeId() == InactiveObject::staticTypeId());
            }
            THEN("Objects that where not active but should be are loaded from storage")
            {
                CHECK(box.getObject(101).lock()->typeId() == LongIntObject::staticTypeId());
            }
            THEN("Objects that should remain active are still active")
            {
                CHECK(box.getObject(102).lock()->typeId() == LongIntObject::staticTypeId());
            }
            THEN("The objects are listed correctly")
            {
                clearStreams();

                *in << "05"; // list all objects
                *in << crc(in->str()) << "\n";
                box.hexCommunicate();

                expected << addCrc("05") << "|"
                         << addCrc("00")
                         << "," << addCrc("0100FFFEFF02")
                         << "," << addCrc("0200FFE80311111111")
                         << "," << addCrc("030000E80312341234")
                         << "," << addCrc("640001FFFFE803") // object 100 (0x64) is listed as inactive object of actual type 0xE803: 6400 - 01 - FFFF - E803
                         << "," << addCrc("650002E80344444444")
                         << "," << addCrc("660003E80344444444")
                         << "\n";
                CHECK(out->str() == expected.str());

                auto listObjectsOriginal = expected.str();

                AND_WHEN("A new box is created from existing storage (for example after a reboot)")
                {
                    // note that only eeprom is not newly created here
                    ObjectContainer container2 = {
                        // profiles obj is id 1
                        ContainedObject(2, 0xFF, std::make_shared<LongIntObject>(0x11111111)),
                        ContainedObject(3, 0xFF, std::make_shared<LongIntObject>(0x22222222))};

                    EepromObjectStorage storage2(eeprom);
                    ObjectFactory factory2 = {
                        {LongIntObject::staticTypeId(), std::make_shared<LongIntObject>},
                        {LongIntVectorObject::staticTypeId(), std::make_shared<LongIntVectorObject>},
                        {UpdateCounter::staticTypeId(), std::make_shared<UpdateCounter>},
                        {PtrLongIntObject::staticTypeId(), [&container]() {
                             return std::make_shared<PtrLongIntObject>(container);
                         }}};

                    StringStreamConnectionSource connSource2;
                    ConnectionPool connPool2 = {connSource2};

                    THEN("All objects can be restored from storage")
                    {
                        Box box2(factory2, container2, storage2, connPool2);
                        box2.loadObjectsFromStorage();

                        auto in2 = std::make_shared<std::stringstream>();
                        auto out2 = std::make_shared<std::stringstream>();
                        connSource2.add(in2, out2);

                        *in2 << "05"; // list all objects
                        *in2 << crc(in2->str()) << "\n";
                        box2.hexCommunicate();

                        CHECK(out2->str() == expected.str());
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
                            box2.loadObjectsFromStorage();

                            auto in2 = std::make_shared<std::stringstream>();
                            auto out2 = std::make_shared<std::stringstream>();
                            connSource2.add(in2, out2);

                            *in2 << "05"; // list all objects
                            *in2 << crc(in2->str()) << "\n";
                            box2.hexCommunicate();

                            // remove object with CRC error from expected string
                            std::string toRemove = "," + addCrc("650002E80344444444");
                            size_t pos = listObjectsOriginal.find(toRemove);
                            auto listObjectsWithObjectMissing = listObjectsOriginal.replace(pos, toRemove.length(), ""); // remove single element

                            CHECK(out2->str() == listObjectsWithObjectMissing);

                            AND_THEN("The EEPROM data can still be retreived with READ_STORED_VALUE")
                            {
                                in2->str("");
                                in2->clear();
                                out2->str("");
                                out2->clear();
                                expected.str("");
                                expected.clear();

                                *in2 << "066500"; // read stored object 101
                                *in2 << crc(in2->str()) << "\n";
                                box2.hexCommunicate();

                                expected << addCrc("066500") << "|"
                                         << addCrc("00"             // status
                                                   + damagedObject) // obj data
                                         << "\n";
                                CHECK(out2->str() == expected.str());
                            }
                        }
                    }
                }
            }
        }

        THEN("When list stored objects command is received, eeprom data for all objects is streamed")
        {
            clearStreams();
            *in << "07"; // list stored objects
            *in << crc(in->str()) << "\n";
            box.hexCommunicate();

            // each object has 2 CRCs: one from EEPROM and from the message part
            expected << addCrc("07") << "|" << addCrc("00")
                     << "," << addCrc("640001E80344444444")
                     << "," << addCrc("650002E80344444444")
                     << "," << addCrc("660003E80344444444")
                     << "," << addCrc("030000E80312341234") // modified system object is also persisted
                     << "\n";

            CHECK(out->str() == expected.str());
        }

        THEN("When the clear objects command is received, all user objects are removed, system objects remain")
        {
            clearStreams();
            *in << "08"; // clear all objects
            *in << crc(in->str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("08") << "|"
                     << addCrc("00") << "\n";
            CHECK(out->str() == expected.str());

            clearStreams();
            *in << "05"; // list all objects
            *in << crc(in->str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("05") << "|" << addCrc("00") // status OK
                     << "," << addCrc("0100FFFEFF01")       // profiles object
                     << "," << addCrc("0200FFE80311111111") // obj 2
                     << "," << addCrc("030000E80312341234") // obj 3
                     << "\n";
            CHECK(out->str() == expected.str());
        }
    }

    THEN("Objects update at their requested interval")
    {
        box.update(0);
        // create 2 counter objects with different update intervals
        // object creation and write also triggers an object update

        clearStreams();
        *in << "03"    // create counter object
            << "6400"  // ID 100
            << "FF"    // profiles FF
            << "EA03"  // type 1002
            << "E803"; // interval 1000
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << addCrc("036400FFEA03E803") << "|" // command repetition
                 << addCrc("00"                       // status OK
                           "6400"                     // id 100
                           "FF"                       // profiles 0xFF
                           "EA03"                     // type 1002
                           "E803"                     // interval 1000
                           "0100")                    // count 1
                 << "\n";

        CHECK(out->str() == expected.str());

        clearStreams();
        *in << "03"    // create counter object
            << "6500"  // ID 101
            << "FF"    // profiles FF
            << "EA03"  // type 1002
            << "D007"; // interval 2000
        *in << crc(in->str()) << "\n";
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
        *in << "02"    // write counter object
            << "6400"  // ID 100
            << "FF"    // profiles FF
            << "EA03"  // type 1002
            << "A00F"; // interval 4000
        *in << crc(in->str()) << "\n";
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
        *in << "03"    // create object
            << "6400"  // ID 100
            << "FF"    // profiles FF
            << "ED03"  // type 1005
            << "0200"  // ptr 1 points to object 2
            << "0300"; // ptr 2 points to object 3

        std::string command = in->str();

        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected
            << addCrc(command) << "|" // command repetition
            << addCrc("00"            // status OK
                      "6400"          // id 100
                      "FF"            // profiles 0xFF
                      "ED03"          // type 1002
                      "0200"          // ptr 1 points to ID 2
                      "0300"          // ptr 2 points to ID 3
                      "01"            // ptr1 OK
                      "01"            // ptr2 OK
                      "11111111"      // value 1
                      "22222222"      // value 2
                      )
            << "\n";

        CHECK(out->str() == expected.str());

        AND_THEN("If the links change and point to an invalid object, the output is correct")
        {
            clearStreams();

            *in << "02"    // write object
                << "6400"  // ID 100
                << "FF"    // profiles FF
                << "ED03"  // type 1005
                << "0300"  // ptr 1 points to object 3
                << "0400"; // ptr 2 points to object 4

            std::string command = in->str();

            *in << crc(in->str()) << "\n";
            box.hexCommunicate();

            expected
                << addCrc(command) << "|" // command repetition
                << addCrc("00"            // status OK
                          "6400"          // id 100
                          "FF"            // profiles 0xFF
                          "ED03"          // type 1002
                          "0300"          // ptr 1 points to ID 3
                          "0400"          // ptr 2 points to ID 4
                          "01"            // ptr1 OK
                          "00"            // ptr2 not OK
                          "22222222"      // value 1
                          "00000000"      // value 2 is default value 0
                          )
                << "\n";

            CHECK(out->str() == expected.str());
        }
    }

    WHEN("A connection sends only a partial message, a CRC error is returned")
    {
        *in << "03"   // create object
            << "0000" // ID assigned by box
            << "FF";  // profiles FF
                      //<< "E803"      // type 1000
                      //<< "44444444"; // value 44444444
                      // *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        expected << "030000FF"
                 << "|"
                 << addCrc("43") << "\n";
        CHECK(out->str() == expected.str());
    }

    WHEN("A connection sends only a partial message with half a hex encoded byte (1 nibble), a CRC error is returned")
    {
        *in << "03" // create object
            << "0"; // ID assigned by box

        box.hexCommunicate();

        expected << "0300"
                 << "|"
                 << addCrc("43") << "\n";
        CHECK(out->str() == expected.str());
    }

    WHEN("All commands are sent commands with invalid CRC, CRC errors are returned")
    {
        for (uint8_t c = 1; c <= 10; ++c) {
            clearStreams();
            *in << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << +c;
            *in << "0000000000";
            *in << crc(in->str() + "10");

            box.hexCommunicate();
            INFO(out->str());
            CHECK(out->str().find("|43") != std::string::npos);
        }
    }

    WHEN("A reboot command is sent, the reset handler is called")
    {
        CHECK(testInfo.rebootCount == 0);

        *in << "09" // reboot
            << crc(in->str()) << "\n";

        box.hexCommunicate();

        CHECK(testInfo.rebootCount == 1);
    }

    WHEN("a factory reset command is sent, EEPROM is reinitialized, followed by a system reboot")
    {
        auto rebootCountBeforeCommand = testInfo.rebootCount;

        *in << "03"        // create object
            << "0000"      // ID assigned by box
            << "FF"        // profiles FF
            << "E803"      // type 1000
            << "44444444"; // value 44444444
        *in << crc(in->str()) << "\n";
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

        *in << "0A" // factory reset
            << crc(in->str()) << "\n";

        box.hexCommunicate();

        CHECK(countNonZeroBytes() == 3); // just the disposed block header
        CHECK(storage.freeSpace() == 2013);

        CHECK(testInfo.rebootCount == rebootCountBeforeCommand + 1);
    }

    WHEN("A device discovery command is received")
    {
        *in << "0C"; // discover new objects
        *in << crc(in->str()) << "\n";
        box.hexCommunicate();

        // we expect this command to create 3 new objects, with values 0x33333333, 0x44444444, 0x55555555
        // 0x11111111 and 0x22222222 already exist

        THEN("A list of IDs of newly created objects is returned")
        {
            expected << addCrc("0C") << "|"
                     << addCrc("00")          // status
                     << "," << addCrc("6400") // new object id 100
                     << "," << addCrc("6500") // new object id 101
                     << "," << addCrc("6600") // new object id 102
                     << "\n";
            CHECK(out->str() == expected.str());
        }

        THEN("The objects that didn't exist yet but where provided by the scanner have been created")
        {
            clearStreams();
            *in << "05"; // list all objects
            *in << crc(in->str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("05")
                     << "|" << addCrc("00")
                     << "," << addCrc("0100FFFEFF01")
                     << "," << addCrc("0200FFE80311111111")
                     << "," << addCrc("0300FFE80322222222")
                     << "," << addCrc("6400FFE80333333333")
                     << "," << addCrc("6500FFE80344444444")
                     << "," << addCrc("6600FFE80355555555")
                     << "\n";
            CHECK(out->str() == expected.str());
        }

        THEN("The newly discoverd objects are persisted")
        {
            clearStreams();
            *in << "07"; // list stored objects
            *in << crc(in->str()) << "\n";
            box.hexCommunicate();

            expected << addCrc("07")
                     << "|" << addCrc("00")
                     << "," << addCrc("6400FFE80333333333")
                     << "," << addCrc("6500FFE80344444444")
                     << "," << addCrc("6600FFE80355555555")
                     << "\n";
            CHECK(out->str() == expected.str());
        }

        clearStreams();
    }
}
