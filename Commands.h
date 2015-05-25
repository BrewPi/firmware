#pragma once

/*
 * Commands.h
 *
 * Created: 05/02/2014 22:32:25
 *  Author: mat
 */ 

#include "DataStream.h"
#include "Values.h"
#include "ValuesEeprom.h"

typedef char* pchar;
typedef const char* cpchar;

void handleCommand(DataIn& data, DataOut& out);

/**
 * Function prototype expected by the commands implementation to perform
 * a reset.  
 * @param exit false on first call, true on second call. The first call (exit==false) is 
 * during command processing, so that actions can be taken before the command response is sent.
 * The second call (exit==true) is called to perform the actual reset.
 */
extern void handleReset(bool exit=true);

/**
 * Removes deleted object definitions from the current profile eeprom block.
 * @return The end address of the object block in the current profile.
 */
eptr_t compactObjectDefinitions();

/**
 * @param	offset	The location in eeprom of this object
 * @param	in		Commands for the object, starting after the command id.
 * @return	>=0 on success, <0 on error.
 */
uint8_t rehydrateObject(eptr_t offset, PipeDataIn& in, bool dryRun=false);


/**
 * Delete an object (but not the definition in eeprom.)
 */
uint8_t deleteObject(DataIn& id);

/**
 * Prototype for object factories. 
 */
typedef Object* (*ObjectFactory)(ObjectDefinition& def);

/**
 * Factory that consumes the object definition stream and returns {@code NULL}.
 */	
Object* nullFactory(ObjectDefinition& def);



enum Commands {
	CMD_NONE = 0,				// no-op

   	CMD_READ_VALUE = 1,			// read a value
   	CMD_WRITE_VALUE = 2,		// write a value
   	CMD_CREATE_OBJECT = 3,		// add object in a container
   	CMD_DELETE_OBJECT = 4,		// delete the object at the specified location
   	CMD_LIST_PROFILE = 5,		// list objects in a profile
	CMD_FREE_SLOT = 6,          // retrieves the next free slot in a container
	CMD_CREATE_PROFILE = 7,     // create a new profile
	CMD_DELETE_PROFILE = 8,     // delete a profile
	CMD_ACTIVATE_PROFILE = 9,	// activate a profile
	CMD_LOG_VALUES = 10,		// request to log all values	
	CMD_RESET = 11,				// perform a reset so that values are read in again from persistent storage.
	CMD_FREE_SLOT_ROOT = 12,	// retrieves the next free slot in the root container
	CMD_NOT_USED = 13,			// by chance this happened to be unused - this might appeal to superstitious minds
	CMD_LIST_PROFILES = 14,		// list the active profile and the available profiles
	CMD_READ_SYSTEM_VALUE = 15,	// read the value from a system object
	CMD_WRITE_SYSTEM_VALUE = 16,// write the value to a system object
	CMD_WRITE_MASK_VALUE = 17,	// write a value with a mask to preserve some of the existing value
	CMD_WRITE_SYSTEM_MASK_VALUE = 18,	// write a system value with a mask to preserve some of the existing value
	CMD_MAX = 127,				// max command value for user-visible commands
	CMD_SPECIAL_FLAG = 128,
	CMD_INVALID = CMD_SPECIAL_FLAG | CMD_NONE,						// special value for invalid command in eeprom. Used as a placeholder for incomplete data
	CMD_DISPOSED_OBJECT = CMD_CREATE_OBJECT | CMD_SPECIAL_FLAG,	// flag in eeprom for object that is now deleted. Allows space to be reclaimed later.
	CMD_LOG_VALUES_AUTO = CMD_LOG_VALUES | CMD_SPECIAL_FLAG,
};


void logValuesImpl(container_id* ids, DataOut& out);

/**
 * 
 */
extern Object* createApplicationObject(ObjectDefinition& def, bool dryRun=false);
