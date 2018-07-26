/*
 * Copyright 2018 Elco Jacobs / BrewBlox.
 *
 * This file is part of BrewBlox.
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
#include "Object.h"
#include "Container.h"
#include "ObjectStorage.h"

namespace cbox {

class System {
    System(ObjectContainer & system, ObjectStorage & _storage) :
        systemObjects(system),
        active_profiles(0),
        storage(_storage){
    }

private:

    /**
     * Initialize this system profile handler.
     */
    void initialize();  // constructor?

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

	/**
	 * The user root container in which the user can create objects. Initialized empty.
	 */
	ObjectContainer userObjects;

	/**
	 * The system container. This provides fixed system level objects. Created by the application.
	 */
	ObjectContainer & systemObjects;

	/**
	 * This bit field encodes which of the 8 possible profiles are currently active.
	 * Objects are active if object_profiles & active_profiles != 0, in other words:
	 * if they are enabled in one of the profiles that is active globally.
	 */
	uint8_t active_profiles;

	/**
	 * The storage class for persisting objects and settings, provided by application
	 */
	ObjectStorage & storage;

	/**
     * Read from EEPROM which objects should be active and apply
     */
	void applyActiveProfilesFromEeprom();
};

} // end namespace cbox

