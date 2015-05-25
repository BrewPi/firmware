#pragma once

#include <stdint.h>
#include "Values.h"
#include "DataStreamEeprom.h"
#include "GenericContainer.h"

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


/**
 * SystemProfile - a special container used to provide access to system profiles.
 * A system profile is a set of object definitions.
 */
class SystemProfile {
	
	/**
	 * the selected profile.
	 */
	static profile_id_t current;
	
	/**
	 * The root container for the selected profile. IF no profile is active, this is NULL.
	 */
	static Container* root;
	
	static FixedContainer systemRoot;
	
	static void setProfileOffset(profile_id_t id, eptr_t offset);
	static eptr_t getProfileOffset(profile_id_t id);
	static eptr_t getProfileEnd(profile_id_t id, bool includeOpen=false);
	static void setCurrentProfile(profile_id_t id);
	
	static void closeOpenProfile();
	static eptr_t compactObjectDefinitions();
	
	
	static void streamObjectDefinitions(EepromDataIn& eepromReader);

	/**
	 * Deactivate the current profile by deleting all objects. (TODO: ideally this should be in reverse order, but I'm counting on objects not being
	 * active during this time and that they have no resources to clean up.)
	 */
	static void deactivateCurrentProfile();
	
	/**
	 * Resets the stream to the region in eeprom for the currently active profile. 
	 * If there is no profile, it is set to the end of eeprom, length 0.
	 */
	static void profileWriteRegion(EepromStreamRegion& region, bool includeOpen=false);

public:
	
	/**
	 * The eeprom stream that maintains the current write position in eeprom for the current profile.
	 * For open profiles, this keeps a pointer to the end of the profile.
	 */
	static EepromDataOut writer;

	
	/**
	 * Initialize this system profile handler.
	 */
	static void initialize();
	
	/*
	 * Load the profile last persisted. 
	 */
	static void activateDefaultProfile();
	
	/**
	 * Fetches the root container for the currently active profile.
	 * Even if no profile is active, still returns a valid root container with just the current profile
	 * value. 
	 */
	static Container* rootContainer() {
		return root;
	}
	
	static Container* systemContainer() {
		return &systemRoot;
	}
		
	/**
	 * Create a new profile.
	 * @return the ID of the profile, or negative on error.
	 */
	static profile_id_t createProfile();
		
	/**
	 * deletes a profile. All profiles with indices larger than this are moved down to one index lower.
	 * All settings for the profile stored in persistent storage are removed and the space is freed up.
	 * If the current profile is the one being deleted, the profile is deactivated first. 
	 */
	static profile_id_t deleteProfile(profile_id_t profile);
	
	/**
	 * Activate the selected profile.
	 * @param The profile to activate. Can be -1 to deactivate the profile.
	 * The active profile is persistent.
	 */
	static bool activateProfile(profile_id_t index);
	
	
	/**
	 * Returns the id of the current profile, or -1 if no profile is active.
	 * @return The currently active profile index, or -1 if no profile is active.
	 */
	static profile_id_t currentProfile();

	/**
	 * Sets the region on an eeprom stream to match the region where there are stored creation instructions for
	 * the given profile.
	 */
	static void profileReadRegion(profile_id_t profile, EepromStreamRegion& region);


	static void setOpenProfileEnd(eptr_t end) {
		if (end>getProfileOffset(-1))
			setProfileOffset(-1, end);
	}
	
	static void listDefinedProfiles(DataIn& in, DataOut& out);
	
	static void listEepromInstructionsTo(profile_id_t profile, DataOut& out);
	
	static void initializeEeprom();
};

/**
 * Parses a stream of object definitions, piping the valid definitions to an output stream.
 * Stops when the input stream doesn't contain a recognized object definition. 
 */
class ObjectDefinitionWalker {
	
	DataIn* _in;		// using pointer to avoid non-POD warnings
	
public:
	ObjectDefinitionWalker(DataIn& in):
		_in(&in) {}
						
	/**
	 * Writes the next object definition from the data input to the given output.
	 * When the input stream is exhausted or the current position is not an object creation command,
	 * the method returns false and the stream location is unchanged. 
	 */							
	bool writeNext(DataOut& out);
};

eptr_t readPointer(eptr_t address);
void writePointer(eptr_t address, eptr_t v);

extern SystemProfile systemProfile;