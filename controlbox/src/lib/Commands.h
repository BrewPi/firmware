/*
 * Copyright 2014-2015 Matthew McGowan.
 * Copyright 2018 BrewBlox / Elco Jacobs
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

#include "Comms.h"
#include "DataStream.h"
#include "Object.h"
#include "Container.h"
#include "EepromObjectStorage.h"

namespace cbox {

enum class CommandError : uint8_t {
    no_error = 0,
    unknown_error = 1,
    stream_error = 2,
    profile_not_active = 3,

    insufficient_persistent_storage = 16,
    insufficient_heap = 17,

    object_not_writable = 32,
    object_not_readable = 33,
    object_not_creatable = 34,
    object_not_deletable = 35,
    object_not_container = 36,
    object_not_open_container = 37,
    container_full = 38,

    invalid_parameter = 64,
    invalid_object_id = 65,
    invalid_type = 66,
    invalid_size = 67,
    invalid_profile = 68,
    invalid_id = 69
};

inline uint8_t errorCode(CommandError e){
    return static_cast<uint8_t>(e);
}

class Commander
{
	CommandHandler handlers[];

	void noopCommandHandler(DataIn& _in, DataOut& out);
	void readObjectCommandHandler(DataIn& in, DataOut& out);
	void setObjectCommandHandler(DataIn& in, DataOut& out);
	void createObjectCommandHandler(DataIn& in, DataOut& out);
	void deleteObjectCommandHandler(DataIn& in, DataOut& out);
	void listObjectsCommandHandler(DataIn& in, DataOut& out);
	void freeSlotCommandHandler(DataIn& in, DataOut& out);
	void createProfileCommandHandler(DataIn& in, DataOut& out);
	void deleteProfileCommandHandler(DataIn& in, DataOut& out);
	void activateProfileCommandHandler(DataIn& in, DataOut& out);
	void logValuesCommandHandler(DataIn& in, DataOut& out);
	void resetCommandHandler(DataIn& in, DataOut& out);
	void freeSlotRootCommandHandler(DataIn& in, DataOut& out);
	void listDefinedProfilesCommandHandler(DataIn& in, DataOut& out);
	void readSystemValueCommandHandler(DataIn& in, DataOut& out);
	void setSystemValueCommandHandler(DataIn& in, DataOut& out);
	void removeEepromCreateCommand(BufferDataOut& id);

public:
	void logValuesImpl(obj_id_t * ids, DataOut& out);

private:
	Comms& comms;
	ObjectStorage& storage;
	// A single container is used for both system and user objects.
	// The application can add the system objects first, then set the start ID to a higher value.
	// The system objects with an ID lower than the start ID cannot be deleted.
	ObjectContainer & objects; 

public:
	Commands(Comms& comms_, ObjectStorage& storage_, Container& objects_)
		: comms(comms_), storage(storage_), objects(objects_)) {
	}

	void handleCommand(DataIn& data, DataOut& out);

	enum CommandID : uint8_t {
		NONE = 0,				// no-op
	   	READ_OBJECT = 1,		// stream an object to the data out
	   	WRITE_OBJECT = 2,		// stream new data into an object from the data in
	   	CREATE_OBJECT = 3,		// add a new object
		DELETE_OBJECT = 4,		// delete an object by id
		LIST_STORED_OBJECTS = 6,// list objects saved to persistent storage
		REBOOT = 7,				// reboot the system
		FACTORY_RESET = 8,		// erase all settings and reboot
	};
};

} // end namespace cbox
