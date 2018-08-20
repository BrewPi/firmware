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
#include "DataStreamConverters.h"
#include "CboxError.h"

namespace cbox {

/**
 * The no-op command simply echoes the response until the end of stream.
 */
void Box::noop(DataIn& in, DataOut& out)
{
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(CboxError::no_error));
}

/**
 * The no-op command simply echoes the response until the end of stream.
 */
void Box::invalidCommand(DataIn& in, DataOut& out)
{
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(CboxError::invalid_command));
}

void Box::readObject(DataIn& in, DataOut& out) {
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
        CboxError result = cobj->streamTo(out);
        // todo handle result?
    }
}

void Box::writeObject(DataIn& in, DataOut& out) {
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
            status = cobj->streamFrom(in);
        }
    }
    
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(status));
    if(cobj != nullptr){
        cobj->streamTo(out);
    }
}

/**
 * Creates a new object at a specific location.
 */
void Box::createObject(DataIn& in, DataOut& out)
{
	obj_id_t newId;

	auto status = objects.addFromStream(in, factory, newId);
	auto newObj = objects.fetchContained(newId);

	in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(status));
    if(newObj){
        newObj->streamTo(out);
    }
}

/**
 * Handles the delete object command.
 *
 */
void Box::deleteObject(DataIn& in, DataOut& out)
{
    CboxError status = CboxError::no_error;
    obj_id_t id;
    if(!in.get(id)){
        status = CboxError::input_stream_read_error;
    }
    status = objects.remove(id);
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(status));
}

/**
 * Walks the eeprom and writes out the construction definitions.
 */
void Box::listActiveObjects(DataIn& in, DataOut& out)
{
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(CboxError::no_error));
    for(auto it = objects.cbegin(); it < objects.cend(); it++){
        out.writeListSeparator();
        it->streamTo(out);
    }
}

void Box::listSavedObjects(DataIn& in, DataOut& out){
    in.spool();
    out.writeResponseSeparator();
    out.write(asUint8(CboxError::no_error));
    storage.streamAllObjectsTo(out);
}

void Box::reboot(DataIn& _in, DataOut& out){
    ::handleReset(true);
}

void Box::factoryReset(DataIn& in, DataOut& out) {
    storage.clear();
    ::handleReset(true);
}

/*
 * Processes the command request from a data stream.
 * @param dataIn The request data. The first byte is the command id. The stream is assumed to contain at least
 *   this data.
 */
void Box::handleCommand(DataIn& dataIn, DataOut& dataOut)
{
    TeeDataIn teeIn(dataIn, dataOut);	// ensure command input is also echoed to output
    uint8_t cmd_id = teeIn.next();		// command type code
    DataIn& in = teeIn;
    DataOut& out = dataOut;

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

    out.endMessage();
}

void Box::hexCommunicate() {
    connections.processAsHex([this](DataIn & in, DataOut & out){
        if(in.hasNext()){
            this->handleCommand(in,out);
        }
    });
}

} // end namespace cbox
