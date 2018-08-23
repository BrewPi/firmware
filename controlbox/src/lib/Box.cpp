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
#include "Object.h"
#include "Container.h"
#include "DataStream.h"
#include "ObjectStorage.h"
#include "DataStreamConverters.h"
#include "CboxError.h"
#include "ResolveType.h"

namespace cbox {

Box::Box(ObjectFactory& _factory, ObjectContainer& _objects, ObjectStorage& _storage, ConnectionPool & _connections) :
    factory(_factory),
    objects(_objects),
    storage(_storage),
    connections(_connections),
    activeProfiles(0x01)
{
    objects.add(std::make_unique<ProfilesObject>(this), 0xFF); // add profiles object to give access to the active profiles setting
    objects.setObjectsStartId(userStartId()); // set startId for user objects to 100
    loadObjectsFromStorage();
}


/**
 * The no-op command simply echoes the response until the end of stream.
 */
void Box::noop(CrcDataIn& in, DataOut& out)
{
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(CboxError::no_error));
}

/**
 * The no-op command simply echoes the response until the end of stream.
 */
void Box::invalidCommand(CrcDataIn& in, DataOut& out)
{
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(CboxError::invalid_command));
}

void Box::readObject(CrcDataIn& in, DataOut& out) {
    CboxError status = CboxError::no_error;
    obj_id_t id = 0;
    ContainedObject * cobj = nullptr;
    if(!in.get(id)){
        status = CboxError::input_stream_read_error;
    }
    else{
        cobj = objects.fetchContained(id);
        if(cobj == nullptr){
            status = CboxError::invalid_object_id;
        }
    }
    out.writeResponseSeparator();
    out.write(asUint8(status));
    if (status == CboxError::no_error) {
        // stream object as id, profiles, typeId, data
        status = cobj->streamTo(out);
        // todo handle status?
        if(cobj->object()->typeId() == resolveTypeId<InactiveObject>()){
            out.writeListSeparator();
            auto objectStreamer = [&out](DataIn & objInStorage) -> CboxError {
                if(objInStorage.push(out)){
                    return CboxError::no_error;
                }
                return CboxError::output_stream_write_error;
            };
            storage.retrieveObject(storage_id_t(id), objectStreamer);
        }
    }
}

void Box::writeObject(CrcDataIn& in, DataOut& out) {
    CboxError status = CboxError::no_error;
    obj_id_t id = 0;
    ContainedObject * cobj = nullptr;
    if(!in.get(id)){
        status = CboxError::input_stream_read_error;
    }
    else {
    	cobj = objects.fetchContained(id);
        if(cobj == nullptr){
            status = CboxError::invalid_object_id;
        }
        else{
            if(cobj->object()->typeId() == resolveTypeId<InactiveObject>()){
                // create object (at least temporarily to write it) and replace inactive object
                addContainedObjectFromStream(in, id, true);
                cobj = objects.fetchContained(id);
            }
            else {
                // update existing object
                status = cobj->streamFrom(in);
            }
        }
    }
    
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(status));
    if(cobj != nullptr){
        status = cobj->streamTo(out);
        if(status == CboxError::no_error){
            auto storeContained = [&cobj](DataOut & out) -> CboxError {
                return cobj->streamPersistedTo(out);
            };
            status = storage.storeObject(id, storeContained);
            // TODO: handle status
            if(id >= userStartId() && !(cobj->profiles() & activeProfiles)){
                cobj->deactivate();
            }
        }
    }
}

/**
 * Creates a new object by streaming in everything except the object id
 * separate body so we can have a version with and without CRC checking
 */
CboxError Box::addContainedObjectFromStreamImpl(DataIn& in, obj_id_t & id, uint8_t & profiles, std::unique_ptr<Object> &newObj)
{
    obj_type_t typeId;
    CboxError status = CboxError::no_error;

    if(!in.get(profiles)){
        status = CboxError::input_stream_read_error;
    }
    if(!in.get(typeId)){
        status = CboxError::input_stream_read_error;
    }

    if(status == CboxError::no_error){
        status = factory.make(typeId, newObj);
        if(newObj){
           status = newObj->streamFrom(in);
        }
    }
    return status;
}
/**
 * Creates a new object by streaming in everything except the object id
 */
CboxError Box::addContainedObjectFromStream(CrcDataIn& in, obj_id_t & id, bool replace){
    uint8_t profiles;
    std::unique_ptr<Object> newObj;
    CboxError status = addContainedObjectFromStreamImpl(in, id, profiles, newObj);
    // add CRC check here
    if(status == CboxError::no_error){
        id = objects.add(std::move(newObj), profiles, id, replace);
    }
    return status;
}

/**
 * Creates a new object by streaming in everything except the object id
 */
CboxError Box::addContainedObjectFromStream(DataIn& in, obj_id_t & id, bool replace){
    uint8_t profiles;
    std::unique_ptr<Object> newObj;
    CboxError status = addContainedObjectFromStreamImpl(in, id, profiles, newObj);

    if(status == CboxError::no_error){
        id = objects.add(std::move(newObj), profiles, id, replace);
    }
    return status;
}

/**
 * Creates a new object and adds it to the container
 */
void Box::createObject(CrcDataIn& in, DataOut& out)
{
    obj_id_t id;
    obj_type_t typeId;

    CboxError status = CboxError::no_error;

    if(!in.get(id)){
        status = CboxError::input_stream_read_error;
    }

    if(status == CboxError::no_error){
        status = addContainedObjectFromStream(in, id, false);
    }

	in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(status));
    if(auto ptrCobj = objects.fetchContained(id)){
        ptrCobj->streamTo(out);
        auto storeContained = [&ptrCobj](DataOut & out) -> CboxError {
           return ptrCobj->streamPersistedTo(out);
        };
        status = storage.storeObject(id, storeContained);
        if(id >= userStartId() && !(ptrCobj->profiles() & activeProfiles) ){
            // object should not be active, replace object with inactive object
            ptrCobj->deactivate();
        }
    }
}

/**
 * Handles the delete object command.
 *
 */
void Box::deleteObject(CrcDataIn& in, DataOut& out)
{
    CboxError status = CboxError::no_error;
    obj_id_t id;
    if(!in.get(id)){
        status = CboxError::input_stream_read_error;
    }
    status = objects.remove(id);
    storage.disposeObject(id); // todo: event if error?
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(status));
}

/**
 * Walks the eeprom and writes out the construction definitions.
 */
void Box::listActiveObjects(CrcDataIn& in, DataOut& out)
{
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(CboxError::no_error));
    for(auto it = objects.cbegin(); it < objects.cend(); it++){
        out.writeListSeparator();
        it->streamTo(out);
    }
}

void Box::listSavedObjects(CrcDataIn& in, DataOut& out){
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(CboxError::no_error));
    auto listObjectStreamer = [&out](const storage_id_t & id, DataIn & objInStorage) -> CboxError {
        out.writeListSeparator();
        obj_id_t objId(id);
        if(out.put(id) && objInStorage.push(out)){
            return CboxError::no_error;
        }
        return CboxError::output_stream_write_error;
    };
    storage.retrieveObjects(listObjectStreamer);
}

// load all objects from storage
void Box::loadObjectsFromStorage(){
    auto objectLoader = [this](const storage_id_t & id, DataIn & objInStorage) -> CboxError {
        obj_id_t objId = obj_id_t(id);
        auto ptrCobj = objects.fetchContained(objId);
        if(ptrCobj == nullptr){
            // new object
            return this->addContainedObjectFromStream(objInStorage, objId, false);
        }
        else{
            // existing object
            return ptrCobj->streamFrom(objInStorage);
        }
    };
    storage.retrieveObjects(objectLoader);
    // this deactives objects loaded from eeprom that should not be active
    // if activeProfiles is modified by one of the loaded objects, this call applies it again so also the objects loaded after it are correct
    setActiveProfilesAndUpdateObjects(activeProfiles);
}

void Box::reboot(CrcDataIn& _in, DataOut& out){
    ::handleReset(true);
}

void Box::factoryReset(CrcDataIn& in, DataOut& out) {
    storage.clear();
    ::handleReset(true);
}


/**
 * Handles the delete all objects command.
 *
 */

void Box::clearObjects(CrcDataIn& in, DataOut& out)
{
    // remove user objects from storage
    for(auto cit = objects.cbegin(); cit != objects.cend(); cit++){
        storage.disposeObject(cit->id());
    }
    // remove all user objects from vector
    objects.clear();

    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(CboxError::no_error));
}

/*
 * Processes the command request from a data stream.
 * @param dataIn The request data. The first byte is the command id. The stream is assumed to contain at least
 *   this data.
 */
void Box::handleCommand(DataIn& dataIn, DataOut& dataOut)
{
    HexTextToBinaryIn hexIn(dataIn);
    BinaryToHexTextOut hexOut(dataOut);


    CrcDataOut out(hexOut);    // write CRC after response
    TeeDataIn teeIn(hexIn, out);	// ensure command input is also echoed to output
    CrcDataIn in(teeIn);
    uint8_t cmd_id = in.next();	// command type code


    switch(cmd_id){
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

void Box::hexCommunicate() {
    connections.process([this](DataIn & in, DataOut & out){
        while(in.hasNext()){
            this->handleCommand(in,out);
        }
    });
}

void Box::setActiveProfilesAndUpdateObjects(const uint8_t newProfiles){
    activeProfiles = newProfiles;
    for(auto cit = objects.userbegin(); cit != objects.cend(); cit++) {
        obj_id_t objId = cit->id();
        uint8_t objProfiles = cit->profiles();
        obj_type_t objType = cit->object()->typeId();

        bool shouldBeActive = activeProfiles & objProfiles;

        // replace entire 'contained object', not just the object inside.
        // this ensures that any smart pointers to the contained object are also invalidated

        if(shouldBeActive && objType == resolveTypeId<InactiveObject>()){
            // look for object in storage and replace existing object with it
            auto retrieveContained = [this, &objId](DataIn & in) -> CboxError {
                return this->addContainedObjectFromStream(in, objId, true);
            };

            CboxError status = storage.retrieveObject(storage_id_t(objId), retrieveContained);
            if(status != CboxError::no_error){
                // TODO emit log event about reloading object from storage failing;
            }
        }

        if(!shouldBeActive && objType != resolveTypeId<InactiveObject>()){
            // replace object with inactive object
            objects.deactivate(cit);
        }
    }
}
} // end namespace cbox
