
#include "Brewpi.h"
#include "Values.h"

/**
 * /param obj - Assumed to be a container.
 */
bool walkContainer(Container* c, EnumObjectsFn callback, void* data, container_id* id, container_id* end) 
{
	uint8_t count = c->size();
	for (int8_t i=0; i<count; i++) {
		Object* o = c->item(i);
		*end = i;
		if (walkObject(o, callback, data, id, end))
			return true;
	}	
	return false;
}

/**
 * Recursively walks all objects in a container hierarchy.
 */
bool walkObject(Object* obj, EnumObjectsFn callback, void* data, container_id* id, container_id* end) {	
	if (callback(obj, data, id, true))
		return true;
	
	if (isContainer(obj)) {		
		*end++ |= 0x80;		// flag as not last element in id chain
		walkContainer((Container*)obj, callback, data, id, end);
		*--end &= 0x7F;		// remove last bit
	}

	if (callback(obj, data, id, false))
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

Object* lookupUserObject(DataIn& data) {
	return lookupObject(rootContainer(), data);
}

OpenContainer* lookupUserOpenContainer(DataIn& data, int8_t& lastID) {
	return lookupOpenContainer(rootContainer(), data, lastID);
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
