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
#include "CboxError.h"
#include "ObjectFactory.h"
#include <memory>

namespace cbox {

class Box
{
private:
	// A single container is used for both system and user objects.
	// The application can add the system objects first, then set the start ID to a higher value.
	// The system objects with an ID lower than the start ID cannot be deleted.
    ObjectFactory& factory;
    ObjectContainer& objects;
	ObjectStorage& storage;
	// Commander receives commands from connections in the connection pool and streams back the answer to the same connection
	ConnectionPool& connections;
	uint8_t activeProfiles;

	// command handlers
	void noop(DataIn& _in, DataOut& out);
	void invalidCommand(DataIn& _in, DataOut& out);
	void readObject(DataIn& in, DataOut& out);
	void writeObject(DataIn& in, DataOut& out);
	void createObject(DataIn& in, DataOut& out);
	void deleteObject(DataIn& in, DataOut& out);
	void listActiveObjects(DataIn& in, DataOut& out);
	void listSavedObjects(DataIn& in, DataOut& out);
	void reboot(DataIn& in, DataOut& out);
	void factoryReset(DataIn& in, DataOut& out);

	void setActiveProfilesAndUpdateObjects(uint8_t newProfiles){
	    for(auto cit = objects.cbegin(); cit != objects.cend(); cit++) {
            obj_id_t objId = cit->id();
            uint8_t objProfiles = cit->profiles();
            obj_type_t objType = cit->object()->typeId();

            bool shouldBeActive = newProfiles & objProfiles;

            // replace entire 'contained object', not just the object inside.
            // this ensures that any smart pointers to the contained object are also invalidated

            if(shouldBeActive && objType == resolveTypeId<InactiveObject>()){
                // look for object in storage
            }

            if(!shouldBeActive && objType != resolveTypeId<InactiveObject>()){
                // replace object with inactive object
                objects.replace(std::make_unique<ContainedObject>(objId, objProfiles, std::make_shared<InactiveObject>(objType)), cit);
            }
        }
	}

public:
	Box(ObjectFactory& _factory, ObjectContainer& _objects, ObjectStorage& _storage, ConnectionPool & _connections) :
	    factory(_factory),
	    objects(_objects),
		storage(_storage),
		connections(_connections),
		activeProfiles(0x01)
	{}
	~Box() = default;

	void handleCommand(DataIn& data, DataOut& out);

	// process all incoming messages assuming they are hex encoded
    void hexCommunicate();

    auto getObject(const obj_id_t& id){
        return objects.fetch(id);
    }

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
