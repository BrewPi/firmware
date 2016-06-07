/*
 * Copyright 2014-2015 Matthew McGowan.
 *
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

#include "Static.h"
#include "Comms.h"
#include "DataStream.h"
#include "Values.h"
#include "SystemProfile.h"
#include "Integration.h"

typedef char* pchar;
typedef const char* cpchar;

class Commands;

/**
 * A command handler function. This is the signature of commands.
 * @param in	The data stream providing input to the command.
 * @param out	The data stream that accepts output from the command.
 */
#if CONTROLBOX_STATIC
typedef void (*CommandHandler)(DataIn& in, DataOut& out);
#else
typedef void (Commands::*CommandHandler)(DataIn& in, DataOut& out);
#endif


#if CONTROLBOX_STATIC
/**
 * Application-provided function that creates an object from the object definition.
 */
extern Object* createApplicationObject(ObjectDefinition& def, bool dryRun=false);

/**
 * Function prototype expected by the commands implementation to perform
 * a reset.
 * @param exit false on first call, true on second call. The first call (exit==false) is
 * during command processing, so that actions can be taken before the command response is sent.
 * The second call (exit==true) is called to perform the actual reset.
 */
extern void handleReset(bool exit=true);


extern void connectionStarted(StandardConnection& connection, DataOut& out);


#else

struct CommandCallbacks
{
	/**
	 * Application-provided function that creates an object from the object definition.
	 */
	virtual Object* createApplicationObject(ObjectDefinition& def, bool dryRun=false)=0;

	/**
	 * Function prototype expected by the commands implementation to perform
	 * a reset.
	 * @param exit false on first call, true on second call. The first call (exit==false) is
	 * during command processing, so that actions can be taken before the command response is sent.
	 * The second call (exit==true) is called to perform the actual reset.
	 */
	virtual void handleReset(bool exit=true)=0;

	virtual void connectionStarted(StandardConnection& connection, DataOut& out)=0;

	virtual Container* createRootContainer()=0;
};

#endif // CONTROLBOX_STATIC



class Commands
{
	friend class ObjectDefinitionWalker;
	static CommandHandler handlers[];


	cb_static void noopCommandHandler(DataIn& _in, DataOut& out);
	cb_static void readValueCommandHandler(DataIn& in, DataOut& out);
	cb_static void setValueCommandHandler(DataIn& in, DataOut& out);
	cb_static void createObjectCommandHandler(DataIn& in, DataOut& out);
	cb_static void deleteObjectCommandHandler(DataIn& in, DataOut& out);
	cb_static void listObjectsCommandHandler(DataIn& in, DataOut& out);
	cb_static void freeSlotCommandHandler(DataIn& in, DataOut& out);
	cb_static void createProfileCommandHandler(DataIn& in, DataOut& out);
	cb_static void deleteProfileCommandHandler(DataIn& in, DataOut& out);
	cb_static void activateProfileCommandHandler(DataIn& in, DataOut& out);
	cb_static void logValuesCommandHandler(DataIn& in, DataOut& out);
	cb_static void resetCommandHandler(DataIn& in, DataOut& out);
	cb_static void freeSlotRootCommandHandler(DataIn& in, DataOut& out);
	cb_static void listDefinedProfilesCommandHandler(DataIn& in, DataOut& out);
	cb_static void readSystemValueCommandHandler(DataIn& in, DataOut& out);
	cb_static void setSystemValueCommandHandler(DataIn& in, DataOut& out);
	cb_static void setMaskValueCommandHandler(DataIn& in, DataOut& out);
	cb_static void setSystemMaskValueCommandHandler(DataIn& in, DataOut& out);


	cb_static Object* createObject(DataIn& in, bool dryRun);
	cb_static void removeEepromCreateCommand(BufferDataOut& id);

public:
	cb_static void logValuesImpl(container_id* ids, DataOut& out);

#if !CONTROLBOX_STATIC
private:
	Comms& comms;
	SystemProfile& systemProfile;
	CommandCallbacks& callbacks;
	EepromAccess& eepromAccess;
public:
	Commands(Comms& comms_, SystemProfile& systemProfile_, CommandCallbacks& callbacks_, EepromAccess& ea)
		: comms(comms_), systemProfile(systemProfile_), callbacks(callbacks_), eepromAccess(ea) {
		comms.setCommands(*this);
		systemProfile_.setCommands(*this);
	}

#define command_callback_fn(x) callbacks. x
#else
#define command_callback_fn(x) ::x
#endif

	inline cb_static Container* rootContainer() {
		return systemProfile.rootContainer();
	}

	inline cb_static Container* createRootContainer() {
		return command_callback_fn(createRootContainer());
	}

	inline cb_static void connectionStarted(StandardConnection& connection, DataOut& out) {
		command_callback_fn(connectionStarted(connection, out));
	}

	inline cb_static Object* createApplicationObject(ObjectDefinition& def, bool dryRun=false) {
		return (command_callback_fn(createApplicationObject(def, dryRun)));
	}

	inline cb_static void handleReset(bool exit=true) {
		command_callback_fn(handleReset(exit));
	}

	cb_static void handleCommand(DataIn& data, DataOut& out);

	/**
	 * Removes deleted object definitions from the current profile eeprom block.
	 * @return The end address of the object block in the current profile.
	 */
	cb_static eptr_t compactObjectDefinitions();

	/**
	 * @param	offset	The location in eeprom of this object
	 * @param	in		Commands for the object, starting after the command id.
	 * @return	>=0 on success, <0 on error.
	 */
	cb_static uint8_t rehydrateObject(eptr_t offset, PipeDataIn& in, bool dryRun=false);


	/**
	 * Delete an object (but not the definition in eeprom.)
	 */
	cb_static uint8_t deleteObject(DataIn& id);

	/**
	 * Prototype for object factories.
	 */
	typedef Object* (*ObjectFactory)(ObjectDefinition& def);


	enum CommandID {
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

};


/**
 * Factory that consumes the object definition stream and returns {@code NULL}.
 */
Object* nullFactory(ObjectDefinition& def);


#if CONTROLBOX_STATIC
extern Commands commands;
#endif
