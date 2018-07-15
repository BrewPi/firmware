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
const uint8_t MAX_SYSTEM_PROFILES = 4;
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

    profile_id_t active_profile_id;
    uint8_t reserved[5];
    ProfileEntry profiles[MAX_SYSTEM_PROFILES];
    uint8_t data[2048-(profilesEepromStart + 2 + 1 + 5 + MAX_SYSTEM_PROFILES*4)];
};

const uint16_t profilesEepromEnd = profilesEepromStart + sizeof(ProfilesEepromLayout);

#define EepromLocation(x) (profilesEepromStart + offsetof(struct ProfilesEepromLayout, x))
#define EepromLocationEnd(x) (profilesEepromStart + offsetof(struct ProfilesEepromLayout, x) + sizeof(ProfilesEepromLayout::x))

static_assert(EepromLocationEnd(data) == 2048, "end of data area is end of 2kb EEPROM");


cb_static_decl(EepromDataOut SystemProfile::writer;)
cb_static_decl(profile_id_t SystemProfile::current;)
cb_static_decl(Container* SystemProfile::root = nullptr;)
cb_static_decl(Container& SystemProfile::systemRoot = systemRootContainer();)


#if !CONTROLBOX_STATIC
SystemProfile::SystemProfile(EepromAccess& access, Container& systemRootContainer)
: root(nullptr), systemRoot(systemRootContainer), writer(access), system_id(access,SYSTEM_PROFILE_ID_OFFSET,1), eepromAccess(access) {}

#endif


#if CONTROLBOX_STATIC
#define invoke_cmd_method(x) commands.x
#else
#define invoke_cmd_method(x) commands_ptr->x
#endif

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

	current = SYSTEM_PROFILE_DEFAULT;
	uint16_t storedHeader;
	eepromAccess.get(EepromLocation(header), storedHeader);
	if (storedHeader == SYSTEM_PROFILE_EEPROM_HEADER) {
		// no initialization required
	}
	else {
	    eepromAccess.put(EepromLocation(active_profile_id), SYSTEM_PROFILE_NONE);

		// clear the start and end addresses
        writeEepromRange(eepromAccess, EepromLocation(profiles), EepromLocationEnd(profiles), 0);

		// reset profile data store
		writeEepromRange(eepromAccess, EepromLocation(data), EepromLocationEnd(data), 0xFF);

		// now write the version string once the eeprom is stable and initialized.
		eepromAccess.put(EepromLocation(header), SYSTEM_PROFILE_EEPROM_HEADER);

#if !SYSTEM_PROFILE_ENABLE
		activateProfile(0);
#endif
	}
}

/**
 * Creates a new profile.
 * This enumerates the profile slots, looking for one that has the value 0, meaning unused, and then initializes
 * the slot to the location of the end of the last used profile.
 * The current profile is not changed, although if the current profile was previously open, it will be closed so that
 * no new data can be created.
 */
profile_id_t SystemProfile::createProfile() {

	profile_id_t idx = errorCode(insufficient_persistent_storage);

	closeOpenProfile();

#if SYSTEM_PROFILE_ENABLE
	eptr_t start = EepromLocation(data);

	// look for a free slot
	for (profile_id_t i=0; i<MAX_SYSTEM_PROFILES; i++) {
		if (!getProfileStart(i) && idx < 0) {
			idx = i;
		}
		eptr_t end = getProfileEnd(i);
		if (start <= end){
		    start = end;
		}
	}

	if (idx > -1) {
		setProfileStart(idx, start);
	}
#endif
	return idx;
}

/**
 * Activates the named profile. If the current profile is the same as the requested active profile, the method silently
 * returns. The new active profile ID is persisted to eeprom before successful activation so that any errors that cause a reset will
 * ensure the system comes back up with the correct profile.
 * @param profile	The profile id to activate. If this is -1, the current profile is deactivated and no profile is activated.
 */
bool SystemProfile::activateProfile(profile_id_t profile) {

	if (profile>=MAX_SYSTEM_PROFILES || !SYSTEM_PROFILE_ENABLE)
		return false;

	// todo - maybe simpler to write new profile to eeprom then reboot.
	// this will avoid fragmentation of the heap.
	if (current!=profile) {
#if SYSTEM_PROFILE_ENABLE
		deactivateCurrentProfile();
#endif
		bool activated = true;

		if (profile>=0 && !getProfileStart(profile)) {
			activated = false;
			profile = -1;
		}
		setCurrentProfile(profile);								// persist the change
		if (profile>=0) {
			root = invoke_cmd_method(createRootContainer());
			EepromDataIn eepromReader cb_nonstatic_decl((eepromAccess));
			profileReadRegion(profile, eepromReader);			// get region in eeprom for the profile
			streamObjectDefinitions(eepromReader);
			profileWriteRegion(writer, true);		// reset to available region (allow open profile)
		}
		return activated;
	}
	return true;
}


void SystemProfile::streamObjectDefinitions(EepromDataIn& eepromReader)
{
	BlackholeDataOut nullOut;
	PipeDataIn reader(eepromReader, nullOut);	// rehydrateObject expects a pipe stream to save the object definition. we just throw it away.
	while (reader.hasNext()) {

		// if cmd is not create object then just parse the contents but don't instantiate.
		eptr_t offset = eepromReader.offset();
		uint8_t cmd = reader.next();
		if (invoke_cmd_method(rehydrateObject(offset, reader, cmd!=Commands::CMD_CREATE_OBJECT)) && cmd==Commands::CMD_CREATE_OBJECT) {
			// todo - what to do with errors? at least log errors.
			// if error creating object, attempt a reset. Write a reset count to eeprom to avoid endless resets.
		}
	}
}

/**
 * Deletes a profile. If the profile being deleted is the active profile,
 * the current profile is deactivated.
 * All profiles above this one in eeprom are shuffled down by the size of the
 * profile.
 */
int8_t SystemProfile::deleteProfile(profile_id_t profile) {
#if SYSTEM_PROFILE_ENABLE
	if (profile==current) {		// persistently unload profile if it's the one to be deleted
		activateProfile(-1);
	}

	eptr_t start = getProfileStart(profile);
	if (!start)             // profile not defined
		return errorCode(invalid_profile);

	eptr_t end = getProfileEnd(profile);

	setProfileStart(profile, 0);  // mark the slot as available

    // adjust all profile end points, including the end point for the open profile
	for (profile_id_t i=-1; i<MAX_SYSTEM_PROFILES; i++) {
		eptr_t e = getProfileStart(i);
		if (e>=end) {    // profile is above the one just deleted
			setProfileStart(i, e-(end-start));
		}
	}

	// block copy eeprom
	while (end<EepromLocationEnd(data)) {
		uint8_t b = eepromAccess.readByte(end++);
		eepromAccess.writeByte(start++, b);
	}
	writeEepromRange(eepromAccess, start, EepromLocationEnd(data), 0xFF);

	// update the start/end of the writable region
	profileWriteRegion(writer, true);
	return 0;
#else
	return invalid_profile;	// not supported
#endif
}

#if SYSTEM_PROFILE_ENABLE
eptr_t SystemProfile::getProfileStart(profile_id_t id) {
    if(id < 0 || id >= MAX_SYSTEM_PROFILES){
        return 0;
    }
    eptr_t start;
    eepromAccess.get(EepromLocation(profiles[id].start), start);
    return start;
}

/**
 * Locates the end (exclusive) of the current profile.
 * If the profile is open, it will point to the end of the data area
 */
eptr_t SystemProfile::getProfileEnd(profile_id_t id, bool includeOpen) {
    eptr_t end = EepromLocationEnd(data);
    if(includeOpen && current == id){
        return end;
    }
    eepromAccess.get(EepromLocation(profiles[id].end), end);
    return end;
}

void SystemProfile::setProfileStart(profile_id_t id, eptr_t offset) {
    if(id > 0 && id < MAX_SYSTEM_PROFILES){
        eepromAccess.put(EepromLocation(profiles[id].start), offset);
    }
}

void SystemProfile::setProfileEnd(profile_id_t id, eptr_t offset) {
    if(id > 0 && id < MAX_SYSTEM_PROFILES){
        eepromAccess.put(EepromLocation(profiles[id].end), offset);
    }
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

void SystemProfile::closeOpenProfile()
{
	// if this profile is open, be sure to compact eeprom
	if (current >= 0 && getProfileEnd(current) == EepromLocationEnd(data)) {
		eptr_t end = compactObjectDefinitions();
		setProfileEnd(current, end);
	}
	// close the writer region
	profileWriteRegion(writer, false);
}

/**
 * Deactivates the current profile. If there is no profile currently active this is a no-op.
 * Deactivation is transient - the eeprom still contains the previously active profile.
 */
void SystemProfile::deactivateCurrentProfile() {
	if (current<0)
		return;

	closeOpenProfile();

	// delete all the objects that were dynamically allocated.
	container_id id[MAX_CONTAINER_DEPTH];				// buffer for id during traversal
	walkRoot(rootContainer(), deleteDynamicallyAllocatedObject, NULL, id);
	current = -1;

	if (isDynamicallyAllocated(root))
		delete_object(root);
	root = NULL;
}
#endif

void SystemProfile::setCurrentProfile(profile_id_t id) {
	current = id;
	eepromAccess.put(EepromLocation(active_profile_id),id);
}

/**
 * Retrieves the current profile.
 * @return the id of the current profile loaded. Negative if no profile loaded.
 */
profile_id_t SystemProfile::currentProfile() {
	return current;
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
