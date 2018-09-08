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
#include "DataStream.h"
#include "DataStreamConverters.h"
#include "Object.h"
#include "ObjectContainer.h"
#include "ObjectStorage.h"
#include <memory>
#include <tuple>

extern void
handleReset(bool);

namespace cbox {

Box::Box(ObjectFactory& _factory, ObjectContainer& _objects, ObjectStorage& _storage, ConnectionPool& _connections)
    : factory(_factory)
    , objects(_objects)
    , storage(_storage)
    , connections(_connections)
    , activeProfiles(0x01)
    , lastUpdateTime(0)
{
    objects.add(std::make_unique<ProfilesObject>(this), 0xFF); // add profiles object to give access to the active profiles setting
    objects.setObjectsStartId(userStartId());                  // set startId for user objects to 100
    loadObjectsFromStorage();
}

/**
 * The no-op command simply echoes the response until the end of stream.
 */
void
Box::noop(DataIn& in, HexCrcDataOut& out)
{
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(CboxError::OK));
}

/**
 * The no-op command simply echoes the response until the end of stream.
 */
void
Box::invalidCommand(DataIn& in, HexCrcDataOut& out)
{
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(CboxError::INVALID_COMMAND));
}

void
Box::readObject(DataIn& in, HexCrcDataOut& out)
{
    CboxError status = CboxError::OK;
    obj_id_t id = 0;
    ContainedObject* cobj = nullptr;
    if (!in.get(id)) {
        status = CboxError::INPUT_STREAM_READ_ERROR; // LCOV_EXCL_LINE
    } else {
        cobj = objects.fetchContained(id);
        if (cobj == nullptr) {
            status = CboxError::INVALID_OBJECT_ID;
        }
    }

    in.spool();
    if (out.crc()) {
        status = CboxError::CRC_ERROR_IN_COMMAND;
    }
    out.writeResponseSeparator();
    out.write(asUint8(status));
    if (status == CboxError::OK) {
        // stream object as id, profiles, typeId, data
        status = cobj->streamTo(out);
        // todo handle status?
    }
}

void
Box::writeObject(DataIn& in, HexCrcDataOut& out)
{
    CboxError status = CboxError::OK;
    obj_id_t id = 0;
    ContainedObject* cobj = nullptr;
    if (!in.get(id)) {
        status = CboxError::INPUT_STREAM_READ_ERROR; // LCOV_EXCL_LINE
    } else {
        cobj = objects.fetchContained(id);
        if (cobj == nullptr) {
            status = CboxError::INVALID_OBJECT_ID;
        } else {
            if (cobj->object()->typeId() == InactiveObject::staticTypeId()) {
                // replace contained object with actual object to be able to write to it
                InactiveObject* inactiveObj = static_cast<InactiveObject*>(cobj->object().get());

                obj_type_t actualType = inactiveObj->actualTypeId();
                uint8_t oldProfiles = cobj->profiles();
                obj_id_t id = cobj->id();

                CboxError result;
                std::shared_ptr<Object> obj;
                std::tie(result, obj) = factory.make(actualType);

                if (result == CboxError::OK && obj) {
                    // replace inactive object with active object to be able to stream to it
                    *cobj = ContainedObject(id, oldProfiles, std::move(obj));
                }
            }
        }
    }

    // stream new settings to object
    if (cobj) {
        status = cobj->streamFrom(in);
    }

    in.spool();
    if (out.crc()) {
        status = CboxError::CRC_ERROR_IN_COMMAND;
    }

    if (cobj != nullptr && status == CboxError::OK) {
        // save new settings to storage
        auto storeContained = [&cobj](DataOut& out) -> CboxError {
            return cobj->streamPersistedTo(out);
        };
        status = storage.storeObject(id, storeContained);

        // deactive object if it is not a system objects and is not in an active profile
        if (id >= userStartId() && (cobj->profiles() & activeProfiles) == 0) {
            cobj->deactivate();
        }
    }

    out.writeResponseSeparator();
    out.write(asUint8(status));
    if (cobj != nullptr) {
        cobj->forcedUpdate(lastUpdateTime); // force an update of the object
        status = cobj->streamTo(out);       // TODO: handle status ?
    }
}

/**
 * Creates a new object by streaming in everything except the object id
 */
std::tuple<CboxError, std::shared_ptr<Object>, uint8_t>
Box::createObjectFromStream(DataIn& in)
{
    obj_type_t typeId;
    uint8_t profiles;

    if (!in.get(profiles)) {
        return {CboxError::INPUT_STREAM_READ_ERROR, nullptr, 0}; // LCOV_EXCL_LINE
    }
    if (!in.get(typeId)) {
        return {CboxError::INPUT_STREAM_READ_ERROR, nullptr, 0}; // LCOV_EXCL_LINE
    }

    CboxError result;
    std::shared_ptr<Object> obj;
    std::tie(result, obj) = factory.make(typeId);
    if (obj) {
        obj->streamFrom(in);
    }
    return std::make_tuple(std::move(result), std::move(obj), profiles);
}

/**
 * Creates a new object and adds it to the container
 */
void
Box::createObject(DataIn& in, HexCrcDataOut& out)
{
    obj_id_t id;
    obj_type_t typeId;
    uint8_t profiles = 0;

    CboxError status = CboxError::OK;

    if (!in.get(id)) {
        status = CboxError::INPUT_STREAM_READ_ERROR; // LCOV_EXCL_LINE
    }

    if (id > 0 && id < userStartId()) {
        status = CboxError::INVALID_OBJECT_ID;
    }

    std::shared_ptr<Object> newObj;
    if (status == CboxError::OK) {
        std::tie(status, newObj, profiles) = createObjectFromStream(in);
    }

    in.spool();
    if (out.crc()) {
        status = CboxError::CRC_ERROR_IN_COMMAND;
    }
    out.writeResponseSeparator();
    out.write(asUint8(status));
    if (status == CboxError::OK) {
        // add object to container. Id is returned because id from stream can be 0 to let the box assign it
        id = objects.add(std::move(newObj), profiles, id, false);
        if (auto ptrCobj = objects.fetchContained(id)) {
            auto storeContained = [&ptrCobj](DataOut& out) -> CboxError {
                return ptrCobj->streamPersistedTo(out);
            };
            status = storage.storeObject(id, storeContained);
            if (id >= userStartId() && !(ptrCobj->profiles() & activeProfiles)) {
                // object should not be active, replace object with inactive object
                ptrCobj->deactivate();
            }
            ptrCobj->forcedUpdate(lastUpdateTime); // force an update of the object
            status = ptrCobj->streamTo(out);       // TODO: handle status ?
        }
    }
}

/**
 * Handles the delete object command.
 *
 */
void
Box::deleteObject(DataIn& in, HexCrcDataOut& out)
{
    CboxError status = CboxError::OK;
    obj_id_t id;

    if (!in.get(id)) {
        status = CboxError::INPUT_STREAM_READ_ERROR; // LCOV_EXCL_LINE
    }

    in.spool();
    if (out.crc()) {
        status = CboxError::CRC_ERROR_IN_COMMAND;
    }

    if (status == CboxError::OK) {
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
Box::listActiveObjects(DataIn& in, HexCrcDataOut& out)
{
    in.spool();
    auto crc = out.crc();

    out.writeResponseSeparator();

    if (crc) {
        out.write(asUint8(CboxError::CRC_ERROR_IN_COMMAND));
        return;
    }

    out.write(asUint8(CboxError::OK));
    for (auto it = objects.cbegin(); it < objects.cend(); it++) {
        out.writeListSeparator();
        it->streamTo(out);
    }
}

void
Box::readStoredObject(DataIn& in, HexCrcDataOut& out)
{
    CboxError status = CboxError::OK;
    obj_id_t id = 0;

    if (!in.get(id)) {
        status = CboxError::INPUT_STREAM_READ_ERROR; // LCOV_EXCL_LINE
    }

    in.spool();
    auto crc = out.crc();

    out.writeResponseSeparator();

    if (crc) {
        status = CboxError::CRC_ERROR_IN_COMMAND;
    }

    if (status != CboxError::OK) {
        out.write(asUint8(status));
        return;
    }

    bool handlerCalled = false;
    auto objectStreamer = [&out, &id, &handlerCalled](RegionDataIn& objInStorage) -> CboxError {
        out.write(asUint8(CboxError::OK));
        out.put(id);
        handlerCalled = true;
        RegionDataIn objWithoutCrc(objInStorage, objInStorage.available() - 1);
        if (!objWithoutCrc.push(out)) {
            return CboxError::OUTPUT_STREAM_WRITE_ERROR; // LCOV_EXCL_LINE;
        }
        return CboxError::OK;
    };
    status = storage.retrieveObject(storage_id_t(id), objectStreamer);
    if (!handlerCalled) {
        out.write(asUint8(CboxError::INVALID_OBJECT_ID)); // write status if handler has not written it
    }
}

void
Box::listStoredObjects(DataIn& in, HexCrcDataOut& out)
{
    in.spool();
    auto crc = out.crc();

    out.writeResponseSeparator();

    if (crc) {
        out.write(asUint8(CboxError::CRC_ERROR_IN_COMMAND));
        return;
    }

    out.write(asUint8(CboxError::OK));
    auto listObjectStreamer = [&out](const storage_id_t& id, DataIn& objInStorage) -> CboxError {
        out.writeListSeparator();
        obj_id_t objId(id);
        RegionDataIn objWithoutCrc(objInStorage, objInStorage.available() - 1);
        if (out.put(id) && objWithoutCrc.push(out)) {
            return CboxError::OK;
        }
        return CboxError::OUTPUT_STREAM_WRITE_ERROR; // LCOV_EXCL_LINE
    };
    storage.retrieveObjects(listObjectStreamer);
}

// load all objects from storage
void
Box::loadObjectsFromStorage()
{
    auto objectLoader = [this](const storage_id_t& id, RegionDataIn& objInStorage) -> CboxError {
        obj_id_t objId = obj_id_t(id);
        CboxError status = CboxError::OK;

        // use a CrcDataOut to a black hole to check the CRC
        BlackholeDataOut hole;
        CrcDataOut crcCalculator(hole);
        TeeDataIn tee(objInStorage, crcCalculator);
        crcCalculator.put(id); // id is part of CRC, but not part of the stream we get from storage

        if (auto ptrCobj = objects.fetchContained(objId)) {
            // existing object
            status = ptrCobj->streamFrom(tee);

            tee.spool();
            if (crcCalculator.crc() != 0) {
                return CboxError::CRC_ERROR_IN_STORED_OBJECT;
            }

        } else {
            // new object
            uint8_t profiles;
            std::shared_ptr<Object> newObj;

            std::tie(status, newObj, profiles) = createObjectFromStream(tee);

            tee.spool();
            if (crcCalculator.crc() != 0) {
                return CboxError::CRC_ERROR_IN_STORED_OBJECT;
            }

            if (newObj) {
                objects.add(std::move(newObj), profiles, id);
            }
        }
        return status;
    };
    // now apply the loader above to all objects in storage
    storage.retrieveObjects(objectLoader);

    // finally, deactivate objects that should not be active based on the (possibly just loaded) active profiles setting
    setActiveProfilesAndUpdateObjects(activeProfiles);
}

void
Box::reboot(DataIn& in, HexCrcDataOut& out)
{
    in.spool();
    auto crc = out.crc();

    out.writeResponseSeparator();

    if (crc) {
        out.write(asUint8(CboxError::CRC_ERROR_IN_COMMAND));
        return;
    }

    out.write(asUint8(CboxError::OK));

    ::handleReset(true);
}

void
Box::factoryReset(DataIn& in, HexCrcDataOut& out)
{
    in.spool();
    auto crc = out.crc();

    out.writeResponseSeparator();

    if (crc) {
        out.write(asUint8(CboxError::CRC_ERROR_IN_COMMAND));
        return;
    }

    storage.clear();

    out.write(asUint8(CboxError::OK));

    ::handleReset(true);
}

/**
 * Handles the delete all objects command.
 *
 */

void
Box::clearObjects(DataIn& in, HexCrcDataOut& out)
{
    in.spool();
    auto crc = out.crc();

    out.writeResponseSeparator();

    if (crc) {
        out.write(asUint8(CboxError::CRC_ERROR_IN_COMMAND));
        return;
    }

    // remove user objects from storage
    for (auto cit = objects.cbegin(); cit != objects.cend(); cit++) {
        storage.disposeObject(cit->id());
    }
    // remove all user objects from vector
    objects.clear();

    out.write(asUint8(CboxError::OK));
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

    HexCrcDataOut out(hexOut);  // write CRC after response
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
    case READ_STORED_OBJECT:
        readStoredObject(in, out);
        break;
    case LIST_STORED_OBJECTS:
        listStoredObjects(in, out);
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

        if (shouldBeActive && objType == InactiveObject::staticTypeId()) {
            // look for object in storage and replace existing object with it
            auto retrieveContained = [this, &objId](RegionDataIn& objInStorage) -> CboxError {
                CboxError status;
                std::shared_ptr<Object> newObj;
                uint8_t profiles = 0;

                // use a CrcDataOut to a black hole to check the CRC
                BlackholeDataOut hole;
                CrcDataOut crcCalculator(hole);
                TeeDataIn tee(objInStorage, crcCalculator);

                crcCalculator.put(objId); // id is part of CRC, but not part of the stream we get from storage
                std::tie(status, newObj, profiles) = createObjectFromStream(tee);

                tee.spool();
                if (crcCalculator.crc() != 0) {
                    return CboxError::CRC_ERROR_IN_STORED_OBJECT;
                }

                if (newObj) {
                    objects.add(std::move(newObj), profiles, objId, true);
                }

                return status;
            };

            CboxError status = storage.retrieveObject(storage_id_t(objId), retrieveContained);
            if (status != CboxError::OK) {
                // TODO emit log event about reloading object from storage failing;
            }
        }

        if (!shouldBeActive && objType != InactiveObject::staticTypeId()) {
            // replace object with inactive object
            objects.deactivate(cit);
        }
    }
}
} // end namespace cbox
