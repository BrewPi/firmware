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


#include "Values.h"

/**
 * @param obj - Assumed to be a container.
 * @param id
 * @param end   The end of the current id chain. When equal to id, the system is walking the root container.
 */
bool walkContainer(Container* c, EnumObjectsFn callback, void* data, container_id* id, container_id* end)
{
	uint8_t count = c->size();
	for (int8_t i=0; i<count; i++) {
		Object* o = c->item(i);
		*end = i;
		if (walkObject(o, callback, data, id, end+1))
			return true;
	}
	return false;
}

/**
 * Recursively walks all objects in a container hierarchy.
 * Assumes that the id chain has already been populated with the id of the object.
 */
bool walkObject(Object* obj, EnumObjectsFn callback, void* data, container_id* id, container_id* end) {
	if (callback(obj, data, id, end, true))
		return true;

	if (isContainer(obj)) {
		*end++ |= 0x80;		// flag as not last element in id chain
		walkContainer((Container*)obj, callback, data, id, end);
		*--end &= 0x7F;		// remove last bit
	}

	if (callback(obj, data, id, end, false))
		return true;

	return false;
}

/**
 * Fetches the object at a given index in a container.
 * @param o	The object containing the object to fetch. This may be NULL and may or may not be a container.
 * @param id	The id to fetch. May include the 0x80 flag for on the wire encoding that this is not the last
 *   id in the chain.
 * @return The fetched object, or {@code NULL} if the object could not be fetched.
 */
Object* fetchContainedObject(Object* o, uint8_t id)
{
	Object* result = NULL;
	id &= 0x7F;
	if (isContainer(o))
	{
		Container* c = (Container*)o;
		if (id<c->size())
			result = c->item(id);
	}
	else {
		// special case of 0 is also allowed as a self reference for non-container objects
		// this allows ids to be padded with 0 bytes without affecting the lookup
		if (!id)
			result = o;
	}
	return result;
}

/**
 * Lookup the object fetching the id chain from a stream.
 * @param data	The data stream to read the id chain.
 * @return The fetched object, or {@code NULL} if the id in the stream doesn't correspond to
 */
Object* lookupObject(Object* current, DataIn& data) {
	int8_t id = -1;
	while (data.hasNext() && id<0)
	{
		id = int8_t(data.next());							// msb set if there is more bytes in the id.
		current = fetchContainedObject(current, uint8_t(id));
	}
	return current;
}

// todo - factor lookupObject/lookupContainer

/**
 * Fetches the container and last id that corresponds to the id chain read from the data stream.
 * @param data	The stream providing the id chain of the object to lokup
 * @param lastID	[outval] Receives the last part of the id chain.
 *
 * For example, given a stream encoding the id chain "2.3.5", the container returned would correspond with
 * object "2.3" and lastID would be set to 5.
 */
OpenContainer* lookupOpenContainer(Object* current, DataIn& data, int8_t& lastID)
{
	int8_t id = int8_t(data.next());
	while (id<0 && data.hasNext())
	{
		current = fetchContainedObject(current, uint8_t(id));
		id = int8_t(data.next());
	}
	lastID = id;

	return isOpenContainer(current) ? (OpenContainer*)current : NULL;
}

Object* lookupUserObject(Container* root, DataIn& data) {
	return lookupObject(root, data);
}

OpenContainer* lookupUserOpenContainer(Container* root, DataIn& data, int8_t& lastID) {
	return lookupOpenContainer(root, data, lastID);
}


void ObjectDefinition::spool() {
	while (in->hasNext())
		in->next();
}


int16_t read2BytesFrom(Value* value) {
	uint8_t result[2];
	BufferDataOut out(result, 2);
	value->readTo(out);
	return int16_t(result[0])<<8 | result[1];
}

