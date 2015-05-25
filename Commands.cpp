/*
 * Commands.cpp
 *
 * Created: 05/02/2014 22:35:10
 *  Author: mat
 */ 

#include "Brewpi.h"
#include "Values.h"
#include "Commands.h"
#include "DataStream.h"
#include "ValuesEeprom.h"
#include "GenericContainer.h"
#include "SystemProfile.h"
#include "Comms.h"

/**
 * A command handler function. This is the signature of commands.
 * @param in	The data stream providing input to the command.
 * @param out	The data stream that accepts output from the command.
 */
typedef void (*CommandHandler)(DataIn& in, DataOut& out);

/**
 * The no-op command simply echoes the response until the end of stream.
 */
void noopCommandHandler(DataIn& _in, DataOut& out)
{	
	while (_in.hasNext());
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
void readValueCommandHandler(DataIn& in, DataOut& out) {	
	readValue(rootContainer(), in, out);
}

void readSystemValueCommandHandler(DataIn& in, DataOut& out) {
	readValue(SystemProfile::systemContainer(), in, out);
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


void setValueCommandHandler(DataIn& in, DataOut& out) {	
	DefaultMask defaultMask;
	setValue(rootContainer(), in, defaultMask, out);
}

void setMaskValueCommandHandler(DataIn& in, DataOut& out) {
	// the mask is given as the input stream - the data are interleaved.
	setValue(rootContainer(), in, in, out);
}

void setSystemValueCommandHandler(DataIn& in, DataOut& out) {
	DefaultMask defaultMask;
	setValue(SystemProfile::systemContainer(), in, defaultMask, out);
}

void setSystemMaskValueCommandHandler(DataIn& in, DataOut& out) {
	setValue(SystemProfile::systemContainer(), in, in, out);
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
uint8_t rehydrateObject(eptr_t offset, PipeDataIn& in, bool dryRun) 
{	
	container_id lastID;
	OpenContainer* target = lookupUserOpenContainer(in, lastID);			// find the container where the object will be added

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

Object* createObject(DataIn& in, bool dryRun)
{
	uint8_t type = in.next();
	uint8_t len = in.next();
	RegionDataIn region(in, len);							// limit stream to actual data block
	ObjectDefinition def = { &region, len, type };
	Object* newObject = createApplicationObject(def, dryRun);			// read the type and create args
	def.spool();			// ensure stream is read fully
	return newObject;
}

/**
 * Creates a new object at a specific location. 
 */
void createObjectCommandHandler(DataIn& _in, DataOut& out)
{
	PipeDataIn in(_in, systemProfile.writer);		// pipe object creation command to eeprom
	
	// todo - streaming the object creation command to eeprom is elegant and simple (little code)
	// but wasteful of space. Each object definition requires a minimum of 4 bytes overhead:
	// 0x03 - object creation command
	// 0xXX+ - container id
	// 0xTT - object type id - will not need 256 of these, so this is a candidate for combining with the command id
	// 0xLL - number of data blocks - this could be implicit?
	
	eptr_t offset = systemProfile.writer.offset();          // save current eeprom pointer - this is where the object definition is written.
	// write the command id placeholder to eeprom (since that's already been consumed)
	systemProfile.writer.write(CMD_INVALID);			// value for partial write, will go a back when successfully completed and write this again
	uint8_t error_code = rehydrateObject(offset, in, false);
	if (!error_code) {
		eepromAccess.writeByte(offset, CMD_CREATE_OBJECT);	// finalize creation in eeprom		
	}
	systemProfile.setOpenProfileEnd(systemProfile.writer.offset());	// save end of open profile
	out.write(error_code);						// status is index it was created at	
}

/**
 * Finds the corresponding create object command in eeprom and marks it as invalid. The command is then ignored,
 * and will be removed from eeprom next time eeprom is compacted. 
 */
void removeEepromCreateCommand(BufferDataOut& id) {
	EepromDataIn eepromData;
	systemProfile.profileReadRegion(systemProfile.currentProfile(), eepromData);	
	ObjectDefinitionWalker walker(eepromData);
	uint8_t buf[MAX_CONTAINER_DEPTH+1];
	BufferDataOut capture(buf, MAX_CONTAINER_DEPTH+1);	// save the contents of the eeprom
	
	eptr_t offset = eepromData.offset();		// save the offset at the start of the creation block
	while (walker.writeNext(capture)) {			// parse creation data. The first part (command + object id) is stored in the capture buffer.
		if (capture.bytesWritten()) {					// valid entry written
			// compare captured id with the id we are looking for
			if (!memcmp(capture.data()+1, id.data(), id.bytesWritten())) {
				eepromAccess.writeByte(offset, CMD_DISPOSED_OBJECT);				
			}			
			capture.reset();
		}
		offset = eepromData.offset();			
	}
}

uint8_t deleteObject(DataIn& id) {
	int8_t lastID;
	uint8_t error = -1;
	OpenContainer* obj = lookupUserOpenContainer(id, lastID);	// find the container and the ID in the chain to remove
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
void deleteObjectCommandHandler(DataIn& in, DataOut& out)
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
void listObjectsCommandHandler(DataIn& _in, DataOut& out)
{
	profile_id_t profile = _in.next();
	SystemProfile::listEepromInstructionsTo(profile, out);
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

void freeSlotCommandHandler(DataIn& in, DataOut& out)
{
	Object* obj = lookupUserObject(in);
	fetchNextSlot(obj, in, out);
}

void freeSlotRootCommandHandler(DataIn& in, DataOut& out)
{
	Object* obj = rootContainer();
	fetchNextSlot(obj, in, out);
}

void deleteProfileCommandHandler(DataIn& in, DataOut& out) {
	uint8_t profile_id = in.next();
	uint8_t result = systemProfile.deleteProfile(profile_id);
	out.write(result);
}

void createProfileCommandHandler(DataIn& in, DataOut& out) {	
	uint8_t result = systemProfile.createProfile();
	out.write(result);
}

/**
 * Writes an ID chain to the stream.
 */
void writeID(container_id* id, DataOut& out) {
	do {
		out.write(*id);
	} while (*id++<0);
}

/**
 * Hierarchy traversal callback
 */
bool logValuesCallback(Object* o, void* data, container_id* id, bool enter) {
	DataOut& out = *(DataOut*)data;
	if (enter && isLoggedValue(o)) {
		Value* r = (Value*)o;
		writeID(id, out);
		out.write(r->streamSize());
		r->readTo(out);
	}
	return false;
}

void logValuesImpl(container_id* ids, DataOut& out) {
	walkRoot(logValuesCallback, NULL, ids);
}

void logValuesCommandHandler(DataIn& in, DataOut& out) {
	uint8_t flags = in.next();
	Object* source = rootContainer();
	if (flags & 1) {
		source = lookupUserObject(in);
	}
	if (source) {
		container_id ids[MAX_CONTAINER_DEPTH];
		walkObject(source, logValuesCallback, &out, ids, ids);
	}
}

void resetCommandHandler(DataIn& in, DataOut& out) {
	uint8_t flags = in.next();
	if (flags&1)
		systemProfile.initializeEeprom();
        handleReset(false);
	out.write(0);
	if (flags&2)
		Comms::resetOnCommandComplete();
}

void activateProfileCommandHandler(DataIn& in, DataOut& out) {
	profile_id_t id = in.next();
	bool result = SystemProfile::activateProfile(id);
	out.write(result ? 0 : -1);
}


CommandHandler handlers[] = {
	noopCommandHandler,				// 0x00
	readValueCommandHandler,		// 0x01
	setValueCommandHandler,			// 0x02
	createObjectCommandHandler,		// 0x03
	deleteObjectCommandHandler,		// 0x04
	listObjectsCommandHandler,		// 0x05
	freeSlotCommandHandler,			// 0x06
	createProfileCommandHandler,	// 0x07
	deleteProfileCommandHandler,	// 0x08
	activateProfileCommandHandler,	// 0x09
	logValuesCommandHandler,		// 0x0A
	resetCommandHandler,			// 0x0B
	freeSlotRootCommandHandler,		// 0x0C
	noopCommandHandler,				// 0x0D
	SystemProfile::listDefinedProfiles,	// 0x0E
	readSystemValueCommandHandler,	// 0x0F
	setSystemValueCommandHandler,	// 0x10
	setMaskValueCommandHandler,		// 0x11
	setSystemMaskValueCommandHandler// 0x12
};

/*
 * Processes the command request from a data stream.
 * @param dataIn The request data. The first byte is the command id. The stream is assumed to contain at least
 *   this data. 
 */
void handleCommand(DataIn& dataIn, DataOut& dataOut)
{
	PipeDataIn pipeIn = PipeDataIn(dataIn, dataOut);	// ensure command input is also piped to output
	uint8_t cmd_id = pipeIn.next();						// command type code
	if (cmd_id>sizeof(handlers)/sizeof(handlers[0]))	// check range
		cmd_id = 0;
	handlers[cmd_id](pipeIn, dataOut);					// do it!
}




