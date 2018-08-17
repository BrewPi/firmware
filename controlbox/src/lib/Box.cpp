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

namespace cbox {

/**
 * The no-op command simply echoes the response until the end of stream.
 */
void Box::noop(DataIn& in, DataOut& out)
{
    in.spool();
    out.writeResponseSeparator();
    out.write(errorCode(CommandError::no_error));       // success
}

/**
 * The no-op command simply echoes the response until the end of stream.
 */
void Box::invalidCommand(DataIn& in, DataOut& out)
{
    in.spool();
    out.writeResponseSeparator();
    out.write(errorCode(CommandError::invalid_command));
}

void Box::readObject(DataIn& in, DataOut& out) {
    CommandError status = CommandError::no_error;
    obj_id_t id = 0;
    ContainedObject * cobj = nullptr;
    if(!in.get(id)){
        status = CommandError::command_parse_error;
    }
    else{
        cobj = objects.fetchContained(id);
        if(cobj == nullptr){
            status = CommandError::invalid_object_id;
        }
    }
    out.writeResponseSeparator();
    out.write(errorCode(status));
    if (status == CommandError::no_error) {
        // stream object as id, profiles, typeId, data
        StreamResult result = cobj->streamTo(out);
        // todo handle result
    }
}

void Box::createObjectFromStorage(obj_id_t id){
    
}

void Box::writeObject(DataIn& in, DataOut& out) {
    CommandError status = CommandError::no_error;
    obj_id_t id = 0;
    ContainedObject * cobj = nullptr;
    if(!in.get(id)){
        status = CommandError::command_parse_error;
    }
    else {
    	cobj = objects.fetchContained(id);
        if(cobj == nullptr){
            status = CommandError::invalid_object_id;
        }
        else{          
            StreamResult sRes = cobj->streamFrom(in);
            switch(sRes){
            case StreamResult::type_mismatch:
            	status = CommandError::invalid_type;
            	break;
            case StreamResult::stream_error:
            	status = CommandError::stream_error;
            	break;
            case StreamResult::success:
            	status = CommandError::no_error;
				break;
            default:
            	status = CommandError::unknown_error;
            }
        }
    }
    
    in.spool();
    out.writeResponseSeparator();
    out.write(errorCode(status));
    if(cobj != nullptr){
        cobj->streamTo(out);
    }
}


/*
std::shared_ptr<Object> Commands::createObject(CommandError & err, obj_type_t type, RegionDataIn& in, bool dryRun)
{
    auto obj = createApplicationObject(err, in, dryRun);			// read the type and create args
    if (!error) {
        if (!newObject) {
            error = errorCode(insufficient_heap);
        }
    }
    def.spool();			// ensure stream is read fully
    return error;
}
*/

/**
 * Creates a new object at a specific location.
 */
void Box::createObject(DataIn& in, DataOut& out)
{
	CommandError status = CommandError::no_error;
	obj_id_t id = 0;
	uint8_t profiles = 0;
	obj_type_t objType;

	if(!in.get(id)){
		status = CommandError::command_parse_error;
	}
	if(!in.get(profiles)){
		status = CommandError::command_parse_error;
	}
	if(!in.get(objType)){
		status = CommandError::command_parse_error;
	}
}

/**
 * Handles the delete object command.
 *
 */
void Box::deleteObject(DataIn& in, DataOut& out)
{
    /*
    uint8_t buf[MAX_CONTAINER_DEPTH+1];
    BufferDataOut idCapture(buf, MAX_CONTAINER_DEPTH+1);	// buffer to capture id
    PipeDataIn idPipe(in, idCapture);						// capture read id
    int8_t error = deleteObject(idPipe);
    if (error>=0)
        removeEepromCreateCommand(idCapture);
    out.writeResponseSeparator();
    out.write(uint8_t(error));
    */
}

/**
 * Walks the eeprom and writes out the construction definitions.
 */
void Box::listActiveObjects(DataIn& _in, DataOut& out)
{
    /*
    // todo - how to flag an invalid profile (currently no results)
    profile_id_t profile = profile_id_t(_in.next());
    out.writeResponseSeparator();
    out.write(0)	;	// status. TODO: check that the profile ID is valid
    systemProfile.listEepromInstructionsTo(profile, out);
    out.write(0);	// list terminator
    */
}

void Box::listStoredObjects(DataIn& _in, DataOut& out){

}

void Box::reboot(DataIn& _in, DataOut& out){

}

void Box::factoryReset(DataIn& in, DataOut& out) {
    /*uint8_t flags = in.next();
    if (flags&1)
        systemProfile.initializeEeprom();
    handleReset(false);
    out.writeResponseSeparator();
    out.write(errorCode(no_error));       // success
    if (flags&2)
        comms.resetOnCommandComplete();
        */
}

/*
 * Processes the command request from a data stream.
 * @param dataIn The request data. The first byte is the command id. The stream is assumed to contain at least
 *   this data.
 */
void Box::handleCommand(DataIn& dataIn, DataOut& dataOut)
{
    TeeDataIn teeIn(dataIn, dataOut);	// ensure command input is also echoed to output
    uint8_t cmd_id = teeIn.next();				// command type code
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
            listStoredObjects(in, out);
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
}

} // end namespace cbox
