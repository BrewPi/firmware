/*
 * Copyright 2014-2015 Matthew McGowan.
 * Copyright 2018 BrewBlox / Elco Jacobs
 * This file is part of Controlbox.
 *
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "CboxError.h"
#include "CboxPtr.h"
#include "Connections.h"
#include "DataStream.h"
#include "DataStreamConverters.h"
#include "EepromObjectStorage.h"
#include "Object.h"
#include "ObjectContainer.h"
#include "ObjectFactory.h"
#include <memory>

namespace cbox {

class Box {
private:
    // A single container is used for both system and user objects.
    // The application can add the system objects first, then set the start ID to a higher value.
    // The objects with an ID lower than the start ID cannot be deleted.
    ObjectFactory& factory;
    ObjectContainer& objects;
    ObjectStorage& storage;
    // Box receives commands from connections in the connection pool and streams back the answer to the same connection
    ConnectionPool& connections;
    uint8_t activeProfiles;
    update_t lastUpdateTime;

    // command handlers
    void noop(DataIn& in, HexCrcDataOut& out);
    void invalidCommand(DataIn& in, HexCrcDataOut& out);
    void readObject(DataIn& in, HexCrcDataOut& out);
    void writeObject(DataIn& in, HexCrcDataOut& out);
    void createObject(DataIn& in, HexCrcDataOut& out);
    void deleteObject(DataIn& in, HexCrcDataOut& out);
    void listActiveObjects(DataIn& in, HexCrcDataOut& out);
    void readStoredObject(DataIn& in, HexCrcDataOut& out);
    void listStoredObjects(DataIn& in, HexCrcDataOut& out);
    void clearObjects(DataIn& in, HexCrcDataOut& out);
    void reboot(DataIn& in, HexCrcDataOut& out);
    void factoryReset(DataIn& in, HexCrcDataOut& out);

    std::tuple<CboxError, std::shared_ptr<Object>, uint8_t> createObjectFromStream(DataIn& in);
    void loadObjectsFromStorage();

public:
    Box(ObjectFactory& _factory, ObjectContainer& _objects, ObjectStorage& _storage, ConnectionPool& _connections);
    ~Box() = default;

    void handleCommand(DataIn& data, DataOut& out);

    // process all incoming messages assuming they are hex encoded
    void hexCommunicate();

    auto getObject(const obj_id_t& id)
    {
        return objects.fetch(id);
    }

    void setActiveProfilesAndUpdateObjects(uint8_t newProfiles);

    uint8_t getActiveProfiles() const
    {
        return activeProfiles;
    }

    void update(const update_t& now)
    {
        lastUpdateTime = now;
        objects.update(now);
    }

    void forcedUpdate(const update_t& now)
    {
        lastUpdateTime = now;
        objects.forcedUpdate(now);
    }

    inline const obj_id_t userStartId() const
    {
        return obj_id_t(100);
    }

    template <typename T>
    CboxPtr<T> makeCboxPtr(const obj_id_t& id = 0)
    {
        return CboxPtr<T>(objects, id);
    }

    enum CommandID : uint8_t {
        NONE = 0,                // no-op
        READ_OBJECT = 1,         // stream an object to the data out
        WRITE_OBJECT = 2,        // stream new data into an object from the data in
        CREATE_OBJECT = 3,       // add a new object
        DELETE_OBJECT = 4,       // delete an object by id
        LIST_ACTIVE_OBJECTS = 5, // list objects saved to persistent storage
        READ_STORED_OBJECT = 6,  // list objects saved to persistent storage
        LIST_STORED_OBJECTS = 7, // list objects saved to persistent storage
        CLEAR_OBJECTS = 8,       // remove all user objects
        REBOOT = 9,              // reboot the system
        FACTORY_RESET = 10,      // erase all settings and reboot
    };
};

// the ProfilesObject can added to a box, so the active profile can be written as a system object and is also persisted
class ProfilesObject : public ObjectBase<std::numeric_limits<uint16_t>::max() - 1> {
    Box* myBox;

public:
    ProfilesObject(Box* box)
        : myBox(box)
    {
    }

    virtual cbox::CboxError streamFrom(cbox::DataIn& dataIn) override final
    {
        uint8_t newProfiles;
        if (!dataIn.get(newProfiles)) {
            return CboxError::INPUT_STREAM_READ_ERROR; // LCOV_EXCL_LINE
        }
        myBox->setActiveProfilesAndUpdateObjects(newProfiles);
        return CboxError::OK;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        uint8_t profiles = myBox->getActiveProfiles();
        if (!out.put(profiles)) {
            return CboxError::OUTPUT_STREAM_WRITE_ERROR; // LCOV_EXCL_LINE
        }
        return CboxError::OK;
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        return streamTo(out);
    }

    virtual update_t update(const update_t& now) override final
    {
        return cbox::Object::update_never(now);
    }
};

} // end namespace cbox
