/*
 * Copyright 2014-2015 Matthew McGowan.
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

#include "Values.h"
#include "Commands.h"
#include "DataStream.h"
#include "GenericContainer.h"
#include "SystemProfile.h"
#include "Comms.h"
#include <string.h>

/**
 * The no-op command simply echoes the response until the end of stream.
 */
void Commands::noopCommandHandler(DataIn& _in, DataOut& out)
{
	while (_in.hasNext()) { _in.next(); }
}

void readValue(Object* root, DataIn& in, DataOut& out) {
	Object* o = lookupObject(root, in);		// read the object and pipe read data to output
	uint8_t available = in.next();		// number of bytes expected
	Value* v = (Value*)o;
	if (isValue(o) && (available==v->streamSize() || available==0)) {
		out.write(v->streamSize());
		v->readTo(out);
	}
	else {								// not a readable object, flag as 0 length
		out.write(0);
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
void setValue(Object* root, DataIn& in, DataIn& mask, DataOut& out) {
	Object* o = lookupObject(root, in);		// fetch the id and the object
	Value* v = (Value*)o;
	uint8_t available = in.next();
	if (isWritable(o) && (v->streamSize()==available)) {		// if it's writable and the correct number of bytes were parsed.
		v->writeMaskedFrom(in, mask);									// assign from stream
		out.write(v->streamSize());							// now write out actual value
		v->readTo(out);
	}
	else {													// either not writable or invalid size
		while (available-->0) {								// consume rest of stream for this command
			in.next();
		}
		out.write(0);										// write 0 bytes (indicates failure)
	}
}


void Commands::setValueCommandHandler(DataIn& in, DataOut& out) {
	DefaultMask defaultMask;
	setValue(systemProfile.rootContainer(), in, defaultMask, out);
}

void Commands::setMaskValueCommandHandler(DataIn& in, DataOut& out) {
	// the mask is given as the input stream - the data are interleaved.
	setValue(systemProfile.rootContainer(), in, in, out);
}

void Commands::setSystemValueCommandHandler(DataIn& in, DataOut& out) {
	DefaultMask defaultMask;
	setValue(systemProfile.systemContainer(), in, defaultMask, out);
}

void Commands::setSystemMaskValueCommandHandler(DataIn& in, DataOut& out) {
	setValue(systemProfile.systemContainer(), in, in, out);
}

/**
 * Consumes the definition data from the stream and returns a {@code NULL} pointer.
 */
Object* nullFactory(ObjectDefinition& def) {
    def.spool();
    return NULL;
}

enum RehydrateErrors {
	rehydrateNoError = 0,
	rehydrateFail = -1			// descriptive :)
};

/**
 * Rehydrate an object from a definition.
 * @param offset	The location in eeprom where this object definition exists. This points to the first byte
	in the object creation command, namely the command id.
 * @param in		The stream containing the object definition. First the id, then the object type, definition block length and
 * the definition block.
 * @return 0 on success, an error code on failure.
 */
uint8_t Commands::rehydrateObject(eptr_t offset, PipeDataIn& in, bool dryRun)
{
	container_id lastID;
	OpenContainer* target = lookupUserOpenContainer(systemProfile.rootContainer(), in, lastID);			// find the container where the object will be added

	Object* newObject = createObject(in, dryRun);			// read the type and create args

	uint8_t error = rehydrateFail;
	if (in.pipeOk() && lastID>=0 && newObject && target && target->add(lastID,newObject)) {		// if the lastID >=0 then it was fetched from a container
        // skip object create command, type and id.
        offset++; // skip creation id
        while (int8_t(eepromAccess.readByte(offset++))<0) {}	// skip contianer
		offset+=2;												// skip object type and length
        newObject->rehydrated(offset);
        error = rehydrateNoError;
    }

	if (error) {
		delete_object(newObject);
	}
	return error;
}

Object* Commands::createObject(DataIn& in, bool dryRun)
{
	uint8_t type = in.next();
	uint8_t len = in.next();
	RegionDataIn region(in, len);							// limit stream to actual data block
	ObjectDefinition def = {
#if !CONTROLBOX_STATIC
			eepromAccess, systemProfile.rootContainer(),
#endif
			&region, len, type
	};
	Object* newObject = createApplicationObject(def, dryRun);			// read the type and create args
	def.spool();			// ensure stream is read fully
	return newObject;
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
	uint8_t error_code = rehydrateObject(offset, in, false);
	if (!error_code) {
		eepromAccess.writeByte(offset, CMD_CREATE_OBJECT);	// finalize creation in eeprom
	}
	systemProfile.setOpenProfileEnd(writer.offset());	// save end of open profile
	out.write(error_code);						// status is index it was created at
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

uint8_t Commands::deleteObject(DataIn& id) {
	int8_t lastID;
	uint8_t error = -1;
	OpenContainer* obj = lookupUserOpenContainer(systemProfile.rootContainer(), id, lastID);	// find the container and the ID in the chain to remove
	if (obj && lastID>=0 && lastID<obj->size()) {
		obj->remove(lastID);
		error = 0;
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
	uint8_t error = deleteObject(idPipe);
	if (!error)
		removeEepromCreateCommand(idCapture);
	out.write(error);
}

/**
 * Walks the eeprom and writes out the construction definitions.
 */
void Commands::listObjectsCommandHandler(DataIn& _in, DataOut& out)
{
	profile_id_t profile = _in.next();
	systemProfile.listEepromInstructionsTo(profile, out);
}

void fetchNextSlot(Object* obj, DataIn& in, DataOut& out)
{
	container_id slot = -1;
	if (isOpenContainer(obj)) {
		OpenContainer* container = (OpenContainer*)obj;
		slot = container->next();
	}
	out.write(slot);
}

void Commands::freeSlotCommandHandler(DataIn& in, DataOut& out)
{
	Object* obj = lookupUserObject(systemProfile.rootContainer(), in);
	fetchNextSlot(obj, in, out);
}

void Commands::freeSlotRootCommandHandler(DataIn& in, DataOut& out)
{
	Object* obj = systemProfile.rootContainer();
	fetchNextSlot(obj, in, out);
}

void Commands::deleteProfileCommandHandler(DataIn& in, DataOut& out) {
	uint8_t profile_id = in.next();
	uint8_t result = systemProfile.deleteProfile(profile_id);
	out.write(result);
}

void Commands::createProfileCommandHandler(DataIn& in, DataOut& out) {
	uint8_t result = systemProfile.createProfile();
	out.write(result);
}

/**
 * Writes an ID chain to the stream.
 */
void writeID(const container_id* id, DataOut& out) {
	do {
		out.write(*id);
	} while (*id++<0);
}

/**
 * Hierarchy traversal callback
 */
bool logValuesCallback(Object* o, void* data, const container_id* id, const container_id* end, bool enter) {
	DataOut& out = *(DataOut*)data;
	if (enter && isLoggedValue(o)) {
		Value* r = (Value*)o;
		writeID(id, out);
		out.write(r->streamSize());
		r->readTo(out);
	}
	return false;
}

void Commands::logValuesImpl(container_id* ids, DataOut& out) {
	walkRoot(systemProfile.rootContainer(), logValuesCallback, NULL, ids);
}

void Commands::logValuesCommandHandler(DataIn& in, DataOut& out) {
	uint8_t flags = in.next();
    // read the ID into a buffer
    container_id ids[MAX_CONTAINER_DEPTH];

    if (systemProfile.rootContainer()) {
		if (flags & 1)  {
			uint8_t idx = 0;
			uint8_t id;
			do
			{
				id = in.next();
				ids[idx++] = id;
			}
			while (id & 0x80);
			BufferDataIn buffer(ids);

			Object* source = lookupUserObject(systemProfile.rootContainer(), buffer);
			walkObject(source, logValuesCallback, &out, ids, ids+idx);
		}
		else {
			walkContainer(systemProfile.rootContainer(), logValuesCallback, &out, ids, ids);
		}
    }
}

void Commands::resetCommandHandler(DataIn& in, DataOut& out) {
	uint8_t flags = in.next();
	if (flags&1)
		systemProfile.initializeEeprom();
	handleReset(false);
	out.write(0);
	if (flags&2)
		comms.resetOnCommandComplete();
}

void Commands::activateProfileCommandHandler(DataIn& in, DataOut& out) {
	profile_id_t id = in.next();
	bool result = systemProfile.activateProfile(id);
	out.write(result ? 0 : 0xFF);
}

void Commands::listDefinedProfilesCommandHandler(DataIn& in, DataOut& out)
{
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
	&Commands::setMaskValueCommandHandler,		// 0x11
	&Commands::setSystemMaskValueCommandHandler // 0x12
};

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



