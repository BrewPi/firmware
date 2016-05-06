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


#pragma once

#include <stdint.h>
#include "Static.h"
#include "Values.h"
#include "DataStreamEeprom.h"
#include "GenericContainer.h"
#include "ValuesEeprom.h"
#include "EepromBlock.h"

#ifndef SYSTEM_PROFILE_ENABLE
#define SYSTEM_PROFILE_ENABLE 1
#endif

typedef container_id profile_id_t;

/**
 * Internal value used to select the default profile. The default profile is the one that was most recently used.
 */
static const profile_id_t SYSTEM_PROFILE_DEFAULT = -2;

/**
 * Profile value used to indicate that no profile should be activated.
 */
static const profile_id_t SYSTEM_PROFILE_NONE = -1;

const uint8_t SYSTEM_PROFILE_MAGIC = 0x69;
const uint8_t SYSTEM_PROFILE_VERSION = 0x01;

#if CONTROLBOX_STATIC

/**
 * Application-provided method to create a new root container for the profile.
 * The application can create default objects in the root container.
 */
extern Container* createRootContainer();

/**
 * Application-provided function to create the object of the given type.
 * @param in	The data input stream that provides the object details. The format is:
 * <pre>
 *	uint8_t			object type
 *  uint8_t			data length
 *  uint8_t[len]	data
 * </pre>
 */
extern Object* createObject(DataIn& in, bool dryRun=false);


#endif

class Commands;

/**
 * SystemProfile - a special container used to provide access to system profiles.
 * A system profile is a set of object definitions.
 */
class SystemProfile {

	/**
	 * the selected profile.
	 */
	cb_static profile_id_t current;

	/**
	 * The application root container for the selected profile. IF no profile is active, this is NULL.
	 */
	cb_static Container* root;

	/**
	 * The system container. This provides fixed services independently from the selected profile.
	 */
	cb_static Container& systemRoot;

	cb_static void setProfileOffset(profile_id_t id, eptr_t offset);
	cb_static eptr_t getProfileOffset(profile_id_t id);
	cb_static eptr_t getProfileEnd(profile_id_t id, bool includeOpen=false);
	cb_static void setCurrentProfile(profile_id_t id);

	cb_static void closeOpenProfile();
	cb_static eptr_t compactObjectDefinitions();


	cb_static void streamObjectDefinitions(EepromDataIn& eepromReader);

	/**
	 * Deactivate the current profile by deleting all objects. (TODO: ideally this should be in reverse order, but I'm counting on objects not being
	 * active during this time and that they have no resources to clean up.)
	 */
	cb_static void deactivateCurrentProfile();

	/**
	 * Resets the stream to the region in eeprom for the currently active profile.
	 * If there is no profile, it is set to the end of eeprom, length 0.
	 */
	cb_static void profileWriteRegion(EepromStreamRegion& region, bool includeOpen=false);

	/**
	 * The eeprom stream that maintains the current write position in eeprom for the current profile.
	 * For open profiles, this keeps a pointer to the end of the profile.
	 */
	cb_static EepromDataOut writer;

	/**
	 * TODO - should we include these default objects in the system container? I feel they should be left to the application.
	 */
	cb_static EepromBlock system_id;

#if !CONTROLBOX_STATIC
	/**
	 * The persistence implementation. For non-static instances, this is passed in the constructor.
	 * For static instances, a global instance named `eepromAccess` is used.
	 */
	EepromAccess&	eepromAccess;

	Commands* commands_ptr;

#endif	// !CONTROLBOX_STATIC


public:

#if !CONTROLBOX_STATIC
	/**
	 * Constructor that injects dependencies.
	 */
	SystemProfile(EepromAccess& eeprom, Container& systemRootContainer);

	void setCommands(Commands& cmds) {
		commands_ptr = &cmds;
	}

#endif	// !CONTROLBOX_STATIC

	/**
	 * Initialize this system profile handler.
	 */
	cb_static void initialize();

	/*
	 * Load the profile last persisted.
	 */
	cb_static void activateDefaultProfile();

	/**
	 * Fetches the root container for the currently active profile.
	 * Even if no profile is active, still returns a valid root container with just the current profile
	 * value.
	 */
	cb_static Container* rootContainer() {
		return root;
	}

	/**
	 * Retrieves the system container. The system container exists independently from any profile.
	 */
	cb_static Container* systemContainer() {
		return &systemRoot;
	}

	/**
	 * Create a new profile.
	 * @return the ID of the profile, or negative on error.
	 */
	cb_static profile_id_t createProfile();

	/**
	 * deletes a profile. All profiles with indices larger than this are moved down to one index lower.
	 * All settings for the profile stored in persistent storage are removed and the space is freed up.
	 * If the current profile is the one being deleted, the profile is deactivated first.
	 */
	cb_static profile_id_t deleteProfile(profile_id_t profile);

	/**
	 * Activate the selected profile.
	 * @param The profile to activate. Can be -1 to deactivate the profile.
	 * The active profile is persistent.
	 */
	cb_static bool activateProfile(profile_id_t index);


	/**
	 * Returns the id of the current profile, or -1 if no profile is active.
	 * @return The currently active profile index, or -1 if no profile is active.
	 */
	cb_static profile_id_t currentProfile();

	/**
	 * Sets the region on an eeprom stream to match the region where there are stored creation instructions for
	 * the given profile.
	 */
	cb_static void profileReadRegion(profile_id_t profile, EepromStreamRegion& region);


	cb_static void setOpenProfileEnd(eptr_t end) {
		if (end>getProfileOffset(-1))
			setProfileOffset(-1, end);
	}

	cb_static void listDefinedProfiles(DataIn& in, DataOut& out);

	cb_static void listEepromInstructionsTo(profile_id_t profile, DataOut& out);

	cb_static void initializeEeprom();

	cb_static EepromDataOut& persistence() {
		return writer;
	}
};

/**
 * Parses a stream of object definitions, piping the valid definitions to an output stream.
 * Stops when the input stream doesn't contain a recognized object definition.
 */
class ObjectDefinitionWalker {

	DataIn* _in;		// using pointer to avoid non-POD warnings
	Commands& _commands;

public:
	ObjectDefinitionWalker(Commands& commands, DataIn& in):
		_in(&in), _commands(commands) {}

	/**
	 * Writes the next object definition from the data input to the given output.
	 * When the input stream is exhausted or the current position is not an object creation command,
	 * the method returns false and the stream location is unchanged.
	 */
	bool writeNext(DataOut& out);
};

eptr_t readPointer(EepromAccess& eeprom, eptr_t address);
void writePointer(EepromAccess& eeprom, eptr_t address, eptr_t v);

#if CONTROLBOX_STATIC
extern SystemProfile systemProfile;
#endif
