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
#include "DataStreamEeprom.h"
#include "GenericContainer.h"
#include "ValuesEeprom.h"
#include "EepromBlock.h"
#include "Object.h"
#include "Container.h"

namespace cbox {

const uint8_t SYSTEM_PROFILE_MAGIC = 0x69;
const uint8_t SYSTEM_PROFILE_VERSION = 0x01;

#if CONTROLBOX_STATIC
/**
 * Application-provided function to create the object of the given type.
 * @param in	The data input stream that provides the object details. The format is:
 * <pre>
 *  uint8_t             profiles (bit field)
 *	uint16_t		    object type
 *  uint8_t[repeated]   data
  * </pre>
 */
extern uint8_t createObject(Object*& result, DataIn& in, bool dryRun=false);


#endif

/**
 * SystemProfile - a special container used to provide access to system profiles.
 * A system profile is a set of object definitions.
 */
class SystemProfile {
    SystemProfile(EepromAccess& eeprom,
                Container & rootContainer,
                Container & systemRootContainer):
        eepromAccess(eeprom),
        root(rootContainer),
        systemRoot(systemRootContainer),
        active_profiles(0)
        {};

    ~SystemProfile() = default;

	/**
	 * the active profiles.
	 */
	uint8_t active_profiles;

	/**
	 * The application root container for the selected profile.
	 */
	Container & root;

	/**
	 * The system container. This provides fixed services independently from the selected profile.
	 */
	Container& systemRoot;

    /**
     * The EEPROM object, used to persist objects
     */
	EepromAccess& eepromAccess;

	void setActiveProfiles(uint8_t profiles);

	void applyActiveProfilesFromEeprom();

	eptr_t compactObjectDefinitions();

	void streamObjectDefinitions(EepromDataIn& eepromReader);

	void processPersistedObject(DataIn& reader);

	/**
	 * Resets the stream to the region in eeprom for to write new objects
	 */
	void profileWriteRegion(EepromStreamRegion& region);

	/**
	 * The eeprom stream that maintains the current write position in eeprom for the current profile.
	 * For open profiles, this keeps a pointer to the end of the profile.
	 */
	EepromDataOut writer;

	/**
	 * TODO - should we include these default objects in the system container? I feel they should be left to the application.
	 */
	EepromBlock system_id;


public:
	/**
	 * Initialize this system profile handler.
	 */
	void initialize();

	/**
	 * Fetches the root container for the currently active profile.
	 * Even if no profile is active, still returns a valid root container with just the current profile
	 * value.
	 */
	Container* rootContainer() {
		return &root;
	}

	/**
	 * Retrieves the system container. The system container exists independently from any profile.
	 */
	Container* systemContainer() {
		return &systemRoot;
	}

	/**
	 * Change which profiles are active.
	 * @param profiles  A bit mask with a 1 for each profile that should be active. LSB is profile 0.
	 * This is persisted to EEPROM.
	 */
	bool setActiveProfiles(uint8_t profiles);


	/**
	 * Returns the bit field of active profiles
	 * @return The currently active profiles as a bit field. If the bit at position x is 1, the profile is active. LSB is profile 0.
	 */
	uint8_t getActiveProfiles();

	void listEepromInstructionsTo(DataOut& out);

	void initializeEeprom();

	EepromDataOut& persistence() {
		return writer;
	}
};

/**
 * Parses a stream of object definitions, piping the valid definitions to an output stream.
 * Stops when the input stream doesn't contain a recognized object definition.
 */
class ObjectDefinitionWalker {

	DataIn* _in;		// using pointer to avoid non-POD warnings

public:
	ObjectDefinitionWalker(DataIn& in): _in(&in){}

	/**
	 * Writes the next object definition from the data input to the given output.
	 * When the input stream is exhausted or the current position is not an object creation command,
	 * the method returns false and the stream location is unchanged.
	 */
	bool writeNext(DataOut& out);
};

#if CONTROLBOX_STATIC
extern SystemProfile systemProfile;
#endif
} // end namespace cbox

