/*
 * Copyright 2014-2016 Matthew McGowan.
 *
 * This file is part of Nice Firmware.
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

#include "Box.h"
#include "CboxError.h"
#include "Connections.h"
#include "ContainedObject.h"
#include "Container.h"
#include "DataStream.h"
#include "DataStreamConverters.h"
#include "Object.h"
#include "ObjectStorage.h"
#include "ResolveType.h"
#include <memory>

namespace cbox {

Box::Box(ObjectFactory& _factory, ObjectContainer& _objects, ObjectStorage& _storage, ConnectionPool& _connections)
    : factory(_factory)
    , objects(_objects)
    , storage(_storage)
    , connections(_connections)
    , activeProfiles(0x01)
{
    objects.add(std::make_unique<ProfilesObject>(this), 0xFF); // add profiles object to give access to the active profiles setting
    objects.setObjectsStartId(userStartId());                  // set startId for user objects to 100
    loadObjectsFromStorage();
}

/**
 * The no-op command simply echoes the response until the end of stream.
 */
void
Box::noop(DataIn& in, CrcDataOut& out)
{
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(CboxError::no_error));
}

/**
 * The no-op command simply echoes the response until the end of stream.
 */
void
Box::invalidCommand(DataIn& in, CrcDataOut& out)
{
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(CboxError::invalid_command));
}

void
Box::readObject(DataIn& in, CrcDataOut& out)
{
    CboxError status = CboxError::no_error;
    obj_id_t id = 0;
    ContainedObject* cobj = nullptr;
    if (!in.get(id)) {
        status = CboxError::input_stream_read_error;
    } else {
        cobj = objects.fetchContained(id);
        if (cobj == nullptr) {
            status = CboxError::invalid_object_id;
        }
    }

    in.spool();
    if (out.crc()) {
        status = CboxError::crc_error_in_command;
    }
    out.writeResponseSeparator();
    out.write(asUint8(status));
    if (status == CboxError::no_error) {
        // stream object as id, profiles, typeId, data
        status = cobj->streamTo(out);
        // todo handle status?
        if (cobj->object()->typeId() == resolveTypeId<InactiveObject>()) {
            out.writeListSeparator();
            auto objectStreamer = [&out](DataIn& objInStorage) -> CboxError {
                if (objInStorage.push(out)) {
                    return CboxError::no_error;
                }
                return CboxError::output_stream_write_error;
            };
            storage.retrieveObject(storage_id_t(id), objectStreamer);
        }
    }
}

void
Box::writeObject(DataIn& in, CrcDataOut& out)
{
    CboxError status = CboxError::no_error;
    obj_id_t id = 0;
    ContainedObject* cobj = nullptr;
    if (!in.get(id)) {
        status = CboxError::input_stream_read_error;
    } else {
        cobj = objects.fetchContained(id);
        if (cobj == nullptr) {
            status = CboxError::invalid_object_id;
        } else {
            if (cobj->object()->typeId() == resolveTypeId<InactiveObject>()) {
                // create object (at least temporarily to write it) and replace inactive object
                uint8_t profiles = 0;

                CboxError status = CboxError::no_error;
                std::unique_ptr<Object> newObj;
                if (status == CboxError::no_error) {
                    status = createObjectFromStream(in, profiles, newObj);

                    if (newObj) {
                        objects.add(std::move(newObj), profiles, id);
                    }
                    cobj = objects.fetchContained(id);
                }
            } else {
                // update existing object
                status = cobj->streamFrom(in);
            }
        }
    }

    in.spool();
    if (out.crc()) {
        status = CboxError::crc_error_in_command;
    }
    out.writeResponseSeparator();
    out.write(asUint8(status));
    if (cobj != nullptr && status == CboxError::no_error) {
        status = cobj->streamTo(out);
        if (status == CboxError::no_error) {
            auto storeContained = [&cobj](DataOut& out) -> CboxError {
                return cobj->streamPersistedTo(out);
            };
            status = storage.storeObject(id, storeContained);
            // TODO: handle status
            if (id >= userStartId() && !(cobj->profiles() & activeProfiles)) {
                cobj->deactivate();
            }
        }
    }
}

/**
 * Creates a new object by streaming in everything except the object id
 */
CboxError
Box::createObjectFromStream(DataIn& in, uint8_t& profiles, std::unique_ptr<Object>& newObj)
{
    obj_type_t typeId;
    CboxError status = CboxError::no_error;

    if (!in.get(profiles)) {
        status = CboxError::input_stream_read_error;
    }
    if (!in.get(typeId)) {
        status = CboxError::input_stream_read_error;
    }

    if (status == CboxError::no_error) {
        status = factory.make(typeId, newObj);
        if (newObj) {
            status = newObj->streamFrom(in);
        }
    }
    return status;
}

/**
 * Creates a new object and adds it to the container
 */
void
Box::createObject(DataIn& in, CrcDataOut& out)
{
    obj_id_t id;
    obj_type_t typeId;
    uint8_t profiles = 0;

    CboxError status = CboxError::no_error;

    if (!in.get(id)) {
        status = CboxError::input_stream_read_error;
    }

    std::unique_ptr<Object> newObj;
    if (status == CboxError::no_error) {
        status = createObjectFromStream(in, profiles, newObj);
    }

    in.spool();
    if (out.crc()) {
        status = CboxError::crc_error_in_command;
    }
    out.writeResponseSeparator();
    out.write(asUint8(status));
    if (status == CboxError::no_error) {
        id = objects.add(std::move(newObj), profiles, id, false);
        if (auto ptrCobj = objects.fetchContained(id)) { // needed because id can be 0 to indicate any free id
            ptrCobj->streamTo(out);
            auto storeContained = [&ptrCobj](DataOut& out) -> CboxError {
                return ptrCobj->streamPersistedTo(out);
            };
            status = storage.storeObject(id, storeContained);
            if (id >= userStartId() && !(ptrCobj->profiles() & activeProfiles)) {
                // object should not be active, replace object with inactive object
                ptrCobj->deactivate();
            }
        }
    }
}

/**
 * Handles the delete object command.
 *
 */
void
Box::deleteObject(DataIn& in, CrcDataOut& out)
{
    CboxError status = CboxError::no_error;
    obj_id_t id;

    if (!in.get(id)) {
        status = CboxError::input_stream_read_error;
    }

    in.spool();
    if (out.crc()) {
        status = CboxError::crc_error_in_command;
    }

    if (status == CboxError::no_error) {
        status = objects.remove(id);
        storage.disposeObject(id); // todo: event if error?
    }

    out.writeResponseSeparator();
    out.write(asUint8(status));
}

/**
 * Walks the eeprom and writes out the construction definitions.
 */
void
Box::listActiveObjects(DataIn& in, CrcDataOut& out)
{
    in.spool();
    auto crc = out.crc();

    out.writeResponseSeparator();

    if (crc) {
        out.write(asUint8(CboxError::crc_error_in_command));
        return;
    }

    out.write(asUint8(CboxError::no_error));
    for (auto it = objects.cbegin(); it < objects.cend(); it++) {
        out.writeListSeparator();
        it->streamTo(out);
    }
}

void
Box::listSavedObjects(DataIn& in, CrcDataOut& out)
{
    in.spool();
    auto crc = out.crc();

    out.writeResponseSeparator();

    if (crc) {
        out.write(asUint8(CboxError::crc_error_in_command));
        return;
    }

    out.write(asUint8(CboxError::no_error));
    auto listObjectStreamer = [&out](const storage_id_t& id, DataIn& objInStorage) -> CboxError {
        out.writeListSeparator();
        obj_id_t objId(id);
        if (out.put(id) && objInStorage.push(out)) {
            return CboxError::no_error;
        }
        return CboxError::output_stream_write_error;
    };
    storage.retrieveObjects(listObjectStreamer);
}

// load all objects from storage
void
Box::loadObjectsFromStorage()
{
    auto objectLoader = [this](const storage_id_t& id, DataIn& objInStorage) -> CboxError {
        obj_id_t objId = obj_id_t(id);
        auto ptrCobj = objects.fetchContained(objId);
        if (ptrCobj == nullptr) {
            // new object
            uint8_t profiles = 0;
            std::unique_ptr<Object> newObj;
            CboxError status = createObjectFromStream(objInStorage, profiles, newObj);

            if (newObj) {
                objects.add(std::move(newObj), profiles, id);
            }

            return status;
        } else {
            // existing object
            return ptrCobj->streamFrom(objInStorage);
        }
    };
    storage.retrieveObjects(objectLoader);
    // this deactives objects loaded from eeprom that should not be active
    // if activeProfiles is modified by one of the loaded objects, this call applies it again so also the objects loaded after it are correct
    setActiveProfilesAndUpdateObjects(activeProfiles);
}

void
Box::reboot(DataIn& in, CrcDataOut& out)
{
    in.spool();
    auto crc = out.crc();

    out.writeResponseSeparator();

    if (crc) {
        out.write(asUint8(CboxError::crc_error_in_command));
        return;
    }

    out.write(asUint8(CboxError::no_error));
    out.flush();

    ::handleReset(true);
}

void
Box::factoryReset(DataIn& in, CrcDataOut& out)
{
    in.spool();
    auto crc = out.crc();

    out.writeResponseSeparator();

    if (crc) {
        out.write(asUint8(CboxError::crc_error_in_command));
        return;
    }

    storage.clear();

    out.write(asUint8(CboxError::no_error));
    out.flush();

    ::handleReset(true);
}

/**
 * Handles the delete all objects command.
 *
 */

void
Box::clearObjects(DataIn& in, CrcDataOut& out)
{
    in.spool();
    auto crc = out.crc();

    out.writeResponseSeparator();

    if (crc) {
        out.write(asUint8(CboxError::crc_error_in_command));
        return;
    }

    // remove user objects from storage
    for (auto cit = objects.cbegin(); cit != objects.cend(); cit++) {
        storage.disposeObject(cit->id());
    }
    // remove all user objects from vector
    objects.clear();

    out.write(asUint8(CboxError::no_error));
}

/*
 * Processes the command request from a data stream.
 * @param dataIn The request data. The first byte is the command id. The stream is assumed to contain at least
 *   this data.
 */
void
Box::handleCommand(DataIn& dataIn, DataOut& dataOut)
{
    HexTextToBinaryIn hexIn(dataIn);
    BinaryToHexTextOut hexOut(dataOut);

    CrcDataOut out(hexOut);     // write CRC after response
    TeeDataIn in(hexIn, out);   // ensure command input is also echoed to output
    uint8_t cmd_id = in.next(); // command type code

    switch (cmd_id) {
    case NONE:
        noop(in, out);
        break;
    case READ_OBJECT:
        readObject(in, out);
        break;
    case WRITE_OBJECT:
        writeObject(in, out);
        break;
    case CREATE_OBJECT:
        createObject(in, out);
        break;
    case DELETE_OBJECT:
        deleteObject(in, out);
        break;
    case LIST_ACTIVE_OBJECTS:
        listActiveObjects(in, out);
        break;
    case LIST_STORED_OBJECTS:
        listSavedObjects(in, out);
        break;
    case CLEAR_OBJECTS:
        clearObjects(in, out);
        break;
    case REBOOT:
        reboot(in, out);
        break;
    case FACTORY_RESET:
        factoryReset(in, out);
        break;
    default:
        invalidCommand(in, out);
        break;
    }

    hexIn.unBlock(); // consumes any leftover \r or \n

    out.endMessage();
}

void
Box::hexCommunicate()
{
    connections.process([this](DataIn& in, DataOut& out) {
        while (in.hasNext()) {
            this->handleCommand(in, out);
        }
    });
}

void
Box::setActiveProfilesAndUpdateObjects(const uint8_t newProfiles)
{
    activeProfiles = newProfiles;
    for (auto cit = objects.userbegin(); cit != objects.cend(); cit++) {
        obj_id_t objId = cit->id();
        uint8_t objProfiles = cit->profiles();
        obj_type_t objType = cit->object()->typeId();

        bool shouldBeActive = activeProfiles & objProfiles;

        // replace entire 'contained object', not just the object inside.
        // this ensures that any smart pointers to the contained object are also invalidated

        if (shouldBeActive && objType == resolveTypeId<InactiveObject>()) {
            // look for object in storage and replace existing object with it
            auto retrieveContained = [this, &objId](DataIn& objInStorage) -> CboxError {
                uint8_t profiles = 0;
                std::unique_ptr<Object> newObj;
                CboxError status = createObjectFromStream(objInStorage, profiles, newObj);

                if (newObj) {
                    objects.add(std::move(newObj), profiles, objId, true);
                }

                return status;
            };

            CboxError status = storage.retrieveObject(storage_id_t(objId), retrieveContained);
            if (status != CboxError::no_error) {
                // TODO emit log event about reloading object from storage failing;
            }
        }

        if (!shouldBeActive && objType != resolveTypeId<InactiveObject>()) {
            // replace object with inactive object
            objects.deactivate(cit);
        }
    }
}
} // end namespace cbox
