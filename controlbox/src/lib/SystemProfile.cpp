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


#include "Static.h"
#include "SystemProfile.h"
#include "Commands.h"
#include "EepromAccess.h"

namespace cbox {

const uint16_t SYSTEM_PROFILE_EEPROM_HEADER = uint16_t(SYSTEM_PROFILE_MAGIC)<<8 | SYSTEM_PROFILE_VERSION;
const uint16_t profilesEepromStart = 2;

struct __attribute__ ((packed)) ProfileEntry {
    uint16_t start;
    uint16_t end;
};

struct __attribute__ ((packed)) ProfilesEepromLayout {

    union {
        uint16_t header;
        struct {
            uint8_t magic;
            uint8_t version;
        };
    };

    uint8_t active_profiles;
    uint8_t reserved[5];
    uint8_t data[2048-(profilesEepromStart + 2 + 1 + 5)];
};

const uint16_t profilesEepromEnd = profilesEepromStart + sizeof(ProfilesEepromLayout);

#define EepromLocation(x) (profilesEepromStart + offsetof(struct ProfilesEepromLayout, x))
#define EepromLocationEnd(x) (profilesEepromStart + offsetof(struct ProfilesEepromLayout, x) + sizeof(ProfilesEepromLayout::x))

static_assert(EepromLocationEnd(data) == 2048, "end of data area is end of 2kb EEPROM");

/**
 * equivalent to /dev/null. Used for discarding output.
 */
BlackholeDataOut blackhole;

inline void writeEepromRange(EepromAccess& eepromAccess, eptr_t start, eptr_t end, uint8_t val) {
	while (start<end) {
		eepromAccess.writeByte(start++, val);
	}
}

void SystemProfile::initializeEeprom() {
	writeEepromRange(eepromAccess, profilesEepromStart, profilesEepromEnd, 0xFF);
}

void SystemProfile::initialize() {
	uint16_t storedHeader;
	eepromAccess.get(EepromLocation(header), storedHeader);
	if (storedHeader == SYSTEM_PROFILE_EEPROM_HEADER) {
		// no initialization required
	}
	else {
	    eepromAccess.put(EepromLocation(active_profiles), 0x00);

		// reset profile data store
		writeEepromRange(eepromAccess, EepromLocation(data), EepromLocationEnd(data), 0xFF);

		// now write the version string once the eeprom is stable and initialized.
		eepromAccess.put(EepromLocation(header), SYSTEM_PROFILE_EEPROM_HEADER);
	}
}

/**
 * Change which profiles are active. This will create objects for previously unactive profiles and delete objects for previously active
 * profiles that change value
 * @param profiles	The new bit field for which profiles are active
 */
void SystemProfile::setActiveProfiles(uint8_t profiles) {
    active_profiles = profiles;
    EepromDataIn eepromReader(eepromAccess);
    streamObjectDefinitions(eepromReader);
    eepromAccess.put(EepromLocation(active_profiles),active_profiles);
}

/**
 * Change which profiles are active. This will create objects for previously unactive profiles and delete objects for previously active
 * profiles that change value
 * @param profiles  The new bit field for which profiles are active
 */
uint8_t SystemProfile::getActiveProfiles() {
    return active_profiles;
}

void SystemProfile::applyActiveProfilesFromEeprom() {
    uint8_t profiles = eepromAccess.get(EepromLocation(active_profiles),active_profiles);
    setActiveProfiles(profiles);
}


/*
 * Read EEPROM block by block. For each create command that is found, create the block if the block is active in a currently active profile
 */

void SystemProfile::streamObjectDefinitions(EepromDataIn& eepromReader)
{
	while (eepromReader.hasNext()) {
		// if cmd is not create object then just parse the contents but don't instantiate.
		eptr_t offset = eepromReader.offset();
		uint8_t cmd = eepromReader.next();
		uint16_t blockSize = uint16_t((eepromReader.next())) << 8 | eepromReader.next();
		RegionDataIn blockLimitedReader(eepromReader, blockSize);

		if(cmd==Commands::CMD_CREATE_OBJECT){
		    processPersistedObject(blockLimitedReader);
		}
		while(blockLimitedReader.next()){
		    // consume remainder of block
		}
	}
}

void SystemProfile::processPersistedObject(RegionDataIn& reader)
{
    // todo
}

} // end namespace cbox
