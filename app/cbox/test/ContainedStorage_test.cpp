#include "catch.hpp"
#include <cstdio>
#include "EepromObjectStorage.h"
#include "ResolveType.h"
#include "Object.h"
#include "Container.h"
#include "ArrayEepromAccess.h"
#include "TestObjects.h"
#include "ObjectFactory.h"

using namespace cbox;

SCENARIO("When objects are stored as contained objects, they can be loaded from storage with the correct type and profiles"){
    ObjectContainer container;
    EepromAccess eeprom;
    EepromObjectStorage storage(eeprom);
    ObjectFactory factory = {
        OBJECT_FACTORY_ENTRY(LongIntObject),
        OBJECT_FACTORY_ENTRY(LongIntVectorObject)
    };

    obj_id_t id1 = container.add(std::make_unique<LongIntObject>(0x11111111), 0x01);
    obj_id_t id2 = container.add(std::make_unique<LongIntObject>(0x22222222), 0x03);
    obj_id_t id3 = container.add(std::make_unique<LongIntVectorObject, std::initializer_list<LongIntObject> >({0x22222222, 0x44444444}), 0x05);
    INFO("IDs: " << id1 << ", " << id2 << ", " << id3);
    
    WHEN("Objects are stored to Eeprom"){
        for(auto cit = container.cbegin(); cit != container.cend(); cit++){
            storage.storeObject(cit->id(), *cit);
        }

        THEN("All objects can be retrieved and added to a new container"){
            ObjectContainer container2;

            auto objectInserter = [&container2, &factory](cbox::DataIn & in) -> CboxError {
                obj_id_t newId;
                auto status = container2.addFromStream(in, factory, newId);
                CHECK(status == CboxError::no_error);
                CHECK(newId.isValid());
                return status;
            };

            storage.retrieveObjects(objectInserter);
            auto cobj1 = container2.fetchContained(id1);
            auto cobj2 = container2.fetchContained(id2);
            auto cobj3 = container2.fetchContained(id3);
            REQUIRE(cobj1);
            REQUIRE(cobj2);
            REQUIRE(cobj3);

            CHECK(cobj1->id() == id1);
            CHECK(cobj2->id() == id2);
            CHECK(cobj3->id() == id3);

            CHECK(cobj1->object()->typeId() == resolveTypeId<LongIntObject>());
            CHECK(cobj2->object()->typeId() == resolveTypeId<LongIntObject>());
            CHECK(cobj3->object()->typeId() == resolveTypeId<LongIntVectorObject>());

            CHECK(cobj1->profiles() == 0x01);
            CHECK(cobj2->profiles() == 0x03);
            CHECK(cobj3->profiles() == 0x05);

            CHECK(*static_cast<LongIntObject*>(&(*cobj1->object())) == LongIntObject(0x11111111));
            CHECK(*static_cast<LongIntObject*>(&(*cobj2->object())) == LongIntObject(0x22222222));
            CHECK(*static_cast<LongIntVectorObject*>(&(*cobj3->object())) == LongIntVectorObject{0x22222222, 0x44444444});
        }    
    }
}
