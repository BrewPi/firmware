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
#include "ValuesEeprom.h"
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
    uint8_t data[2048-(profilesEepromStart + 2 + 1 + 5 + 2)];
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
	    eepromAccess.put(EepromLocation(active_profiles), SYSTEM_PROFILE_NONE);

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

void SystemProfile::processPersistedObject(DataIn& reader)
{
    uint8_t blockProfiles = reader.next(); // ?? do in commands?
}

/**
 * Deletes objects after any child objects have been deleted. Callback from container traversal.
 */
bool deleteDynamicallyAllocatedObject(Object* obj, void* data, const container_id* id, const container_id* end, bool enter) {
	if (!enter && isDynamicallyAllocated(obj)) {		// delete on exit, so that all children are processed first
		BufferDataIn idStream(id);						// stream the id
		Commands* cmds = (Commands*)(data);
		cmds->deleteObject(idStream);							// delete the object
	}
	return false;										// continue traversal
}

/**
 * Sets the stream region to be the writable portion of a profile storage.
 * @param region		The region to set
 * @param includeOpen	If the current profile is the open one. The end is then set to the end of eeprom.
 */
void SystemProfile::profileWriteRegion(EepromStreamRegion& region, bool includeOpen) {
	if (current>=0) {
		eptr_t offset = getProfileEnd(current);
		eptr_t end = getProfileEnd(current, includeOpen);
		region.reset(offset, end-offset);
	}
	else {
		region.reset(0,0);
	}
}

/**
 * Sets the stream to correspond with the start and end locations for the current profile.
 *
 */
void SystemProfile::profileReadRegion(profile_id_t profile, EepromStreamRegion& region) {
	if (profile>=0 && profile<MAX_SYSTEM_PROFILES) {
		eptr_t offset = getProfileStart(profile);
		eptr_t end = getProfileEnd(profile, false);
		region.reset(offset, end-offset);
	}
	else {
		region.reset(0,0);
	}
}

void SystemProfile::activateDefaultProfile()
{
	profile_id_t id = -1;
	eepromAccess.get(EepromLocation(active_profile_id), id);
	activateProfile(id);
}

void SystemProfile::listDefinedProfiles(DataIn& in, DataOut& out) {
	out.write(currentProfile());
#if SYSTEM_PROFILE_ENABLE
	// todo - this command format is not self-describing regaring length. The caller will have to consume bytes until the end to determine which profiles are active.
	for (profile_id_t i=0; i<MAX_SYSTEM_PROFILES; i++) {
		if (getProfileStart(i))
			out.write(i);
	}
#else
	out.write(0);
#endif
}

#if CONTROLBOX_STATIC
Container* rootContainer() {
	return systemProfile.rootContainer();
}
#endif

/**
 * Writes the next object definition from the data input to the given output.
 * When the input stream is exhausted or the current position is not an object creation command,
 * the method returns false and the stream location is unchanged.
 */
bool ObjectDefinitionWalker::writeNext(DataOut& out) {
	if (!_in->hasNext())
		return false;

	uint8_t next = _in->peek();
	bool valid =  ((next&0x7F)==Commands::CMD_CREATE_OBJECT);
	if (valid) {
		PipeDataIn pipe(*_in, next<0 ? blackhole : out);	// next<0 if command not fully completed, so output is discarded
		pipe.next();										// fetch the next value already peek'ed at so this is written to the output stream
		/*Object* target = */lookupUserObject(_commands.rootContainer(), pipe);			// find the container where the object will be added
		// todo - could flag warning if target is NULL
		Object* obj;
		_commands.createObject(obj, pipe, true);							// dry run for create object, just want data to be output
	}
	return valid;
}

/**
 * Compacts the eeprom instruction store by removing deleted object definitions.
 *
 * @return The offset where the next eeprom instruction will be stored.
 * This method assumes SystemProfile::writer points to the last written location at the end of the profile.
 */
eptr_t SystemProfile::compactObjectDefinitions() {
	EepromDataOut eepromData cb_nonstatic_decl((eepromAccess));
	profile_id_t current = SystemProfile::currentProfile();
	profileReadRegion(current, eepromData);
	listEepromInstructionsTo(current, eepromData);
	return eepromData.offset();
}

/**
 * Enumerates all the create object instructions in eeprom to an output stream.
 */
void SystemProfile::listEepromInstructionsTo(profile_id_t profile, DataOut& out) {
	EepromDataIn eepromData cb_nonstatic_decl((eepromAccess));
	profileReadRegion(profile, eepromData);
	Commands& cmds =
#if CONTROLBOX_STATIC
			commands
#else
			*commands_ptr
#endif
				;
	ObjectDefinitionWalker walker(cmds, eepromData);
	while (walker.writeNext(out)) {}
}

} // end namespace cbox
