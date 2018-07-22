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


#include "Commands.h"
#include "DataStream.h"
#include "GenericContainer.h"
#include "SystemProfile.h"
#include "Comms.h"
#include <string.h>

#include "Object.h"

namespace cbox {

/**
 * The no-op command simply echoes the response until the end of stream.
 */
void Commands::noopCommandHandler(DataIn& _in, DataOut& out)
{
	while (_in.hasNext()) { _in.next(); }
	out.writeResponseSeparator();
    out.write(errorCode(no_error));       // success
}

bool checkType(obj_type_t& typeID, ReadableObject* value) {
	return !typeID || value->typeID()==typeID;
}

void readValue(Object* root, DataIn& in, DataOut& out) {
	Object* o = lookupObject(root, in);		// read the object and pipe read data to output
	obj_type_t typeID = readObjTypeFrom(in);
	uint8_t available = in.next();			// number of bytes expected
	int8_t status = 0;
	ReadableObject* v = (ReadableObject*)o;
	uint8_t expectedSize = 0;
	if (!o) {
		status = errorCode(invalid_id);
	}
	else{
	    if (!isValue(o)) {
	        status = errorCode(object_not_readable);
	    }
		else{
		    expectedSize = v->readStreamSize();
		    if (!(available==0 || (expectedSize==available))) {
		        status = errorCode(invalid_size);
		    }
            else if (!checkType(typeID, v)) {
                status = errorCode(invalid_type);
            }
        }
	}
	out.writeResponseSeparator();
	out.write(uint8_t(status));
	if (!status) {
		out.write(v->typeID());
		out.write(expectedSize);
		v->readTo(out);
	}
}

/**
 * Implements the read value command. Accepts multiple ID chains and outputs each chain plus the
 * data for that object, or a 0-byte block if the object is not known or is not readable.
 */
void Commands::readValueCommandHandler(DataIn& in, DataOut& out) {
    readValue(systemProfile.rootContainer(), in, out);
}

void Commands::readSystemValueCommandHandler(DataIn& in, DataOut& out) {
	readValue(systemProfile.systemContainer(), in, out);
}

/**
 * Implements the set value command.
 */
void setValue(Object* root, DataIn& in, DataOut& out) {
	Object* o = lookupObject(root, in);		// fetch the id and the object
	WritableObject* v = (WritableObject*)o;
	obj_type_t typeID = readObjTypeFrom(in);
	uint8_t available = in.next();
	uint8_t expected;
	int8_t status = 0;
	if (!o) {
		status = errorCode(invalid_id);
	}
	else if (!isWritable(o)) {
		status = errorCode(object_not_writable);
	}
	else if (!((expected=v->writeStreamSize())==available || expected==0)) {
		status = errorCode(invalid_size);
	}
	else if (!checkType(typeID, v)) {
		status = errorCode(invalid_type);
	}

	if (status) {
	    while(in.hasNext()){
	        in.next(); // consume rest of the command, so it is echoed correctly before the data is sent
	    }
	    out.writeResponseSeparator();
        out.write(uint8_t(status));
	}
	else {
		v->writeFrom(in);									// assign from stream
        while(in.hasNext()){
            in.next(); // consume rest of the command if not consumed, so it echoes before the data output
        }
        out.writeResponseSeparator();
        out.write(uint8_t(status));
		out.write(v->typeID());
		out.write(v->readStreamSize());							// now write out actual value
		v->readTo(out);
	}
}


void Commands::setValueCommandHandler(DataIn& in, DataOut& out) {
	setValue(systemProfile.rootContainer(), in, out);
}
void Commands::setSystemValueCommandHandler(DataIn& in, DataOut& out) {
	setValue(systemProfile.systemContainer(), in, out);
}

/**
 * Consumes the definition data from the stream and returns a {@code NULL} pointer.
 */
std::shared_ptr<Object> nullFactory(RegionDataIn& def) {
    def.spool();
    return NULL;
}

int8_t checkOpenContainer(Object* obj, OpenContainer*& container, int8_t lastID=0) {
	int8_t error = no_error;
	if (!obj || lastID<0) {
		error = errorCode(invalid_id);
	}
	else if (!isContainer(obj)) {
		error = errorCode(object_not_container);
	}
	else if (!isOpenContainer(obj)) {
		error = errorCode(object_not_open_container);
	}
	else {
		container = reinterpret_cast<OpenContainer*>(obj);
	}
	return error;
}

int8_t lookupUserOpenContainer(Container* root, DataIn& in, OpenContainer*& container) {
	Object* obj = lookupObject(root, in);			// find the container where the object will be added
	return checkOpenContainer(obj, container);
}

int8_t lookupUserOpenContainer(Container* root, DataIn& in, int8_t& lastID, OpenContainer*& container) {
	Object* obj = lookupObject(root, in, lastID);			// find the container where the object will be added
	return checkOpenContainer(obj, container, lastID);
}


/**
 * Rehydrate an object from a definition.
 * @param offset	The location in eeprom where this object definition exists. This points to the first byte
	in the object creation command, namely the command id.
 * @param in		The stream containing the object definition. First the id, then the object type, definition block length and
 * the definition block.
 * @return 0 on success, an error code on failure.
 */
int8_t Commands::rehydrateObject(eptr_t offset, PipeDataIn& in, bool dryRun)
{
	container_id lastID;
	OpenContainer* container;
	int8_t error = lookupUserOpenContainer(systemProfile.rootContainer(), in, lastID, container);			// find the container where the object will be added
	if (!in.pipeOk()) {
		error = errorCode(stream_error);
	}

	Object* newObject = nullptr;
	if (!error) {
		OpenContainer* target = container;
		error = createObject(newObject, in, dryRun);			// read the type and create args

		if (!error && !target->add(lastID,newObject)) {
			error = errorCode(insufficient_heap);
		}
	}

	if (!error) {
        // skip object create command, type and id.
        offset++; // skip creation id
        while (int8_t(eepromAccess.readByte(offset++))<0) {}	// skip contianer
		offset+=2;												// skip object type and length
        newObject->persist(offset);
    }
	else {
	    delete_object(newObject);
	}
	return error;
}

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

/**
 * Creates a new object at a specific location.
 */
void Commands::createObjectCommandHandler(DataIn& _in, DataOut& out)
{
	EepromDataOut& writer = systemProfile.persistence();
	PipeDataIn in(_in, writer);		// pipe object creation command to eeprom

	// todo - streaming the object creation command to eeprom is elegant and simple (little code)
	// but wasteful of space. Each object definition requires a minimum of 4 bytes overhead:
	// 0x03 - object creation command
	// 0xXX+ - container id
	// 0xTT - object type id - will not need 256 of these, so this is a candidate for combining with the command id
	// 0xLL - number of data blocks - this could be implicit?

	eptr_t offset = writer.offset();          // save current eeprom pointer - this is where the object definition is written.
	// write the command id placeholder to eeprom (since that's already been consumed)
	writer.write(CMD_INVALID);			// value for partial write, will go a back when successfully completed and write this again
	int8_t error_code = rehydrateObject(offset, in, false);
	if (!error_code) {
		eepromAccess.writeByte(offset, CMD_CREATE_OBJECT);	// finalize creation in eeprom
	}
	systemProfile.setOpenProfileEnd(writer.offset());	// save end of open profile
	out.writeResponseSeparator();
	out.write(uint8_t(error_code));						// status is index it was created at
}

/**
 * Finds the corresponding create object command in eeprom and marks it as invalid. The command is then ignored,
 * and will be removed from eeprom next time eeprom is compacted.
 */
void Commands::removeEepromCreateCommand(BufferDataOut& id) {
	EepromDataIn eepromData cb_nonstatic_decl((eepromAccess));
	systemProfile.profileReadRegion(systemProfile.currentProfile(), eepromData);
	Commands& cmds =
#if CONTROLBOX_STATIC
			commands
#else
			*this
#endif
			;
	ObjectDefinitionWalker walker(cmds, eepromData);
	uint8_t buf[MAX_CONTAINER_DEPTH+1];
	BufferDataOut capture(buf, MAX_CONTAINER_DEPTH+1);	// save the contents of the eeprom

	eptr_t offset = eepromData.offset();		// save the offset at the start of the creation block
	while (walker.writeNext(capture)) {			// parse creation data. The first part (command + object id) is stored in the capture buffer.
		if (capture.bytesWritten()) {					// valid entry written
			// compare captured id with the id we are looking for
			if (!memcmp(capture.data()+1, id.data(), id.bytesWritten())) {
				eepromAccess.writeByte(offset, Commands::CMD_DISPOSED_OBJECT);
			}
			capture.reset();
		}
		offset = eepromData.offset();
	}
}

int8_t Commands::deleteObject(DataIn& id) {
	int8_t lastID = 0;
	OpenContainer* container = nullptr;
	int8_t error = lookupUserOpenContainer(systemProfile.rootContainer(), id, lastID, container);	// find the container and the ID in the chain to remove
	if (!error && lastID>=container->size()) {
		error = errorCode(invalid_id);
	}

	if (!error) {
		Object* target = container->item(lastID);
		error = target ? int8_t(target->typeID()) : 0;
		container->remove(lastID);
	}
	return error;
}

/**
 * Handles the delete object command.
 *
 */
void Commands::deleteObjectCommandHandler(DataIn& in, DataOut& out)
{
	uint8_t buf[MAX_CONTAINER_DEPTH+1];
	BufferDataOut idCapture(buf, MAX_CONTAINER_DEPTH+1);	// buffer to capture id
	PipeDataIn idPipe(in, idCapture);						// capture read id
	int8_t error = deleteObject(idPipe);
	if (error>=0)
		removeEepromCreateCommand(idCapture);
	out.writeResponseSeparator();
	out.write(uint8_t(error));
}

/**
 * Walks the eeprom and writes out the construction definitions.
 */
void Commands::listObjectsCommandHandler(DataIn& _in, DataOut& out)
{
	// todo - how to flag an invalid profile (currently no results)
	profile_id_t profile = profile_id_t(_in.next());
	out.writeResponseSeparator();
	out.write(0)	;	// status. TODO: check that the profile ID is valid
	systemProfile.listEepromInstructionsTo(profile, out);
	out.write(0);	// list terminator
}


container_id fetchNextSlot(OpenContainer* container)
{
	container_id slot = container->next();
	if (slot==-1) {  // generic error
		slot = errorCode(container_full);
	}
	return slot;
}


int8_t freeSlot(Container* root, DataIn& in, DataOut& out) {
	int8_t status;
	if (!root) {
		status = errorCode(profile_not_active);
	}
	else {
		OpenContainer* container = nullptr;
		status = lookupUserOpenContainer(root, in, container);
		if (!status) {
			status = fetchNextSlot(container);
		}
	}
	return status;
}

void Commands::freeSlotCommandHandler(DataIn& in, DataOut& out)
{
	int8_t status = freeSlot(systemProfile.rootContainer(), in, out);
	out.writeResponseSeparator();
	out.write(status);
}

void Commands::freeSlotRootCommandHandler(DataIn& in, DataOut& out)
{
	int8_t status = freeSlot(systemProfile.rootContainer(), in, out);
    out.writeResponseSeparator();
	out.write(status);
}

void Commands::deleteProfileCommandHandler(DataIn& in, DataOut& out) {
	profile_id_t profile_id = profile_id_t(in.next());
	int8_t result = systemProfile.deleteProfile(profile_id);
	out.writeResponseSeparator();
	out.write(uint8_t(result));
}

void Commands::createProfileCommandHandler(DataIn& in, DataOut& out) {
	int8_t result = systemProfile.createProfile();
	out.writeResponseSeparator();
	if(result >= 0){
		out.write(0); // error code
	    out.write(uint8_t(result)); // profile id
	}
	else {
		out.write(result); // error code
	}
}

/**
 * Writes an ID chain to the stream.
 */
void writeID(const container_id* id, DataOut& out) {
	do {
		out.write(uint8_t(*id));
	} while (*id++<0);
}

/**
 * Hierarchy traversal callback
 */
bool logValuesCallback(Object* o, void* data, const container_id* id, const container_id* end, bool enter) {
	DataOut& out = *(DataOut*)data;
	if (enter && isLoggedValue(o)) {
		ReadableObject* r = (ReadableObject*)o;
		out.write(uint8_t(1));					// ID of "read command"
		writeID(id, out);
		out.write(r->typeID());
		out.write(r->readStreamSize());
		r->readTo(out);
	}
	return false;
}

void Commands::logValuesImpl(container_id* ids, DataOut& out) {
	walkRoot(systemProfile.rootContainer(), logValuesCallback, NULL, ids);
}

const uint8_t LOG_FLAGS_IDCHAIN = 1<<0;
const uint8_t LOG_FLAGS_SYSTEM_CONTAINER = 1<<1;

/**
 * Logs the values in the object identified.
 * The flags allow either system or user root to be used, and an optional ID chain to walk the container hierarchy.
 */
void Commands::logValuesCommandHandler(DataIn& in, DataOut& out) {
	uint8_t flags = in.next();

    Container* root = (flags & LOG_FLAGS_SYSTEM_CONTAINER) ? systemProfile.systemContainer() : systemProfile.rootContainer();

    int8_t error = errorCode(profile_not_active);
    if (root) {
	    // read the ID into a buffer (also used for iterating the container hierarchy.)
	    container_id ids[MAX_CONTAINER_DEPTH];
		if (flags & LOG_FLAGS_IDCHAIN)  {
			uint8_t idx = 0;
			container_id id;
			do
			{
				id = container_id(in.next());
				ids[idx++] = id;
			}
			while (id & 0x80);
			BufferDataIn buffer(ids);

			Object* source = lookupUserObject(root, buffer);
			if (source) {
				out.writeResponseSeparator();
				out.write(errorCode(no_error));
				walkObject(source, logValuesCallback, &out, ids, ids+idx);
				out.write(0);		// list terminator
				return;
			}
		}
		else {
			error = errorCode(no_error);
            out.writeResponseSeparator();
			out.write(error);
			walkContainer(root, logValuesCallback, &out, ids, ids);
			out.write(0);		// list terminator
			return;
		}
	}
    if (error<0) {
        out.writeResponseSeparator();
        out.write(uint8_t(error));
    }
}

void Commands::resetCommandHandler(DataIn& in, DataOut& out) {
	uint8_t flags = in.next();
	if (flags&1)
		systemProfile.initializeEeprom();
	handleReset(false);
	out.writeResponseSeparator();
    out.write(errorCode(no_error));       // success
	if (flags&2)
		comms.resetOnCommandComplete();
}

void Commands::activateProfileCommandHandler(DataIn& in, DataOut& out) {
	profile_id_t id = profile_id_t(in.next());
	bool activated = systemProfile.activateProfile(id);
	out.writeResponseSeparator();
	out.write(activated ? errorCode(no_error) : errorCode(invalid_profile));
}

void Commands::listDefinedProfilesCommandHandler(DataIn& in, DataOut& out)
{
    out.writeResponseSeparator();
    out.write(errorCode(no_error));       // success
    systemProfile.listDefinedProfiles(in, out);
}

CommandHandler Commands::handlers[] = {
	&Commands::noopCommandHandler,				// 0x00
	&Commands::readValueCommandHandler,		    // 0x01
	&Commands::setValueCommandHandler,			// 0x02
	&Commands::createObjectCommandHandler,		// 0x03
	&Commands::deleteObjectCommandHandler,		// 0x04
	&Commands::listObjectsCommandHandler,		// 0x05
	&Commands::freeSlotCommandHandler,			// 0x06
	&Commands::createProfileCommandHandler,	    // 0x07
	&Commands::deleteProfileCommandHandler,	    // 0x08
	&Commands::activateProfileCommandHandler,	// 0x09
	&Commands::logValuesCommandHandler,		    // 0x0A
	&Commands::resetCommandHandler,			    // 0x0B
	&Commands::freeSlotRootCommandHandler,		// 0x0C
	&Commands::noopCommandHandler,				// 0x0D
	&Commands::listDefinedProfilesCommandHandler,	// 0x0E
	&Commands::readSystemValueCommandHandler,	// 0x0F
	&Commands::setSystemValueCommandHandler,	// 0x10
};

// todo - there are pairs of commands that affect system or user objects
// would be good if these differed only by 1-bit.
// todo - a way to list the objects in the system profile


/*
 * Processes the command request from a data stream.
 * @param dataIn The request data. The first byte is the command id. The stream is assumed to contain at least
 *   this data.
 */
void Commands::handleCommand(DataIn& dataIn, DataOut& dataOut)
{
	PipeDataIn pipeIn = PipeDataIn(dataIn, dataOut);	// ensure command input is also piped to output
	uint8_t cmd_id = pipeIn.next();						// command type code
	if (cmd_id>sizeof(handlers)/sizeof(handlers[0]))	// check range
		cmd_id = 0;
	(
#if !CONTROLBOX_STATIC
	// invoke as a non-static member function
	this->*
#endif
	handlers[cmd_id])(pipeIn, dataOut);					// do it!
}


#if CONTROLBOX_STATIC
Commands commands;
#endif

} // end namespace cbox
