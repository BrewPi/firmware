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

#include "Object.h"
#include "Container.h"
#include "Connections.h"
#include "DataStream.h"
#include "DataStreamConverters.h"
#include "EepromObjectStorage.h"

namespace cbox {

enum class CommandError : uint8_t {
    no_error = 0,
    unknown_error = 1,
    command_parse_error = 2,
	stream_error = 3,
    
    insufficient_persistent_storage = 16,
    insufficient_heap = 17,

    object_not_writable = 32,
    object_not_readable = 33,
    object_not_creatable = 34,
    object_not_deletable = 35,
    
    invalid_command = 63,
    invalid_parameter = 64,
    invalid_object_id = 65,
    invalid_type = 66,
    invalid_size = 67,
    invalid_profile = 68,
};

inline uint8_t errorCode(CommandError e){
    return static_cast<uint8_t>(e);
}

class Box
{
private:
	// A single container is used for both system and user objects.
	// The application can add the system objects first, then set the start ID to a higher value.
	// The system objects with an ID lower than the start ID cannot be deleted.
	ObjectContainer& objects;
	ObjectStorage& storage;
	// Commander receives commands from connections in the connection pool and streams back the answer to the same connection
	ConnectionPool& connections;

	// command handlers
	void noop(DataIn& _in, DataOut& out);
	void invalidCommand(DataIn& _in, DataOut& out);
	void readObject(DataIn& in, DataOut& out);
	void writeObject(DataIn& in, DataOut& out);
	void createObject(DataIn& in, DataOut& out);
	void deleteObject(DataIn& in, DataOut& out);
	void listActiveObjects(DataIn& in, DataOut& out);
	void listStoredObjects(DataIn& in, DataOut& out);
	void reboot(DataIn& in, DataOut& out);
	void factoryReset(DataIn& in, DataOut& out);
	
	// helper functions
	void createObjectFromStorage(obj_id_t id);

public:
	Box(ObjectContainer& _objects, ObjectStorage& _storage, ConnectionPool & _connections) :
		objects(_objects),
		storage(_storage),
		connections(_connections)
	{}
	~Box() = default;

	void handleCommand(DataIn& data, DataOut& out);

	enum CommandID : uint8_t {
		NONE = 0,				// no-op
	   	READ_OBJECT = 1,		// stream an object to the data out
	   	WRITE_OBJECT = 2,		// stream new data into an object from the data in
	   	CREATE_OBJECT = 3,		// add a new object
		DELETE_OBJECT = 4,		// delete an object by id
		LIST_ACTIVE_OBJECTS = 5,// list objects saved to persistent storage
		LIST_STORED_OBJECTS = 6,// list objects saved to persistent storage
		REBOOT = 7,				// reboot the system
		FACTORY_RESET = 8,		// erase all settings and reboot
	};
};

} // end namespace cbox
