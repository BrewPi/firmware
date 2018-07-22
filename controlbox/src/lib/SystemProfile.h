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
#include "Object.h"
#include "Container.h"

namespace cbox {

const uint8_t SYSTEM_PROFILE_MAGIC = 0x69;
const uint8_t SYSTEM_PROFILE_VERSION = 0x01;

/**
 * SystemProfile - a special container used to provide access to system profiles.
 * A system profile is a set of object definitions.
 */
class SystemProfile {
    SystemProfile(EepromAccess& eeprom,
                ObjectContainer & rootContainer,
                ObjectContainer & systemRootContainer):
        active_profiles(0),
        root(rootContainer),
        systemRoot(systemRootContainer),
        eepromAccess(eeprom)
        {};

    ~SystemProfile() = default;

	/**
	 * the active profiles.
	 */
	uint8_t active_profiles;

	/**
	 * The application root container for the selected profile.
	 */
	ObjectContainer & root;

	/**
	 * The system container. This provides fixed services independently from the selected profile.
	 */
	ObjectContainer& systemRoot;

    /**
     * The EEPROM object, used to persist objects
     */
	EepromAccess& eepromAccess;

	void applyActiveProfilesFromEeprom();

	eptr_t compactObjectDefinitions();

	void streamObjectDefinitions(EepromDataIn& eepromReader);

	void processPersistedObject(RegionDataIn& reader);

	/**
	 * Resets the stream to the region in eeprom for to write new objects
	 */
	void profileWriteRegion(EepromStreamRegion& region);

	/**
	 * The eeprom stream that maintains the current write position in eeprom for the current profile.
	 * For open profiles, this keeps a pointer to the end of the profile.
	 */
	EepromDataOut writer;

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
	ObjectContainer* rootContainer() {
		return &root;
	}

	/**
	 * Retrieves the system container. The system container exists independently from any profile.
	 */
	ObjectContainer* systemContainer() {
		return &systemRoot;
	}

	/**
	 * Change which profiles are active.
	 * @param profiles  A bit mask with a 1 for each profile that should be active. LSB is profile 0.
	 * This is persisted to EEPROM.
	 */
	void setActiveProfiles(uint8_t profiles);


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

#if CONTROLBOX_STATIC
extern SystemProfile systemProfile;
#endif
} // end namespace cbox

