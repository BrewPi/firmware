
#include "GenericContainer.h"

#define DYNAMIC_CONTAINER_BOUNDS_CHECKS 0

container_id DynamicContainer::next() {
	container_id sz = _size();
	for (container_id i=0; i<sz;i++)
		if (!_items[i])
			return i;				
	return !((sz+1)&0x80) ? sz : -1;
}

bool DynamicContainer::expand(uint8_t sz)
{
	// some useful details on malloc/realloc here - http://www.nongnu.org/avr-libc/user-manual/malloc.html	
	void* _newitems = realloc(_items, sz*sizeof(Object*));
	if (_newitems) {
		uint8_t prev_sz = size();
		_items = (Object**)_newitems;
		while (prev_sz<sz)	{
			assign(prev_sz++, NULL);
		}
        setSize(sz);
	}
	return _newitems!=0;
}

bool DynamicContainer::add(container_id slot, Object* item) {
	if (slot<0 || (slot>=size() && !expand(slot+1)))
		return false;
	remove(slot);
	assign(slot, item);
	return true;
}

void DynamicContainer::remove(container_id id) {
#if DYNAMIC_CONTAINER_BOUNDS_CHECKS
	if (id<size()) 
#endif	
	{
		delete_object(item(id));
		assign(id, NULL);
	}
}


Object* DynamicContainer::item(container_id id) {
	return (id<_size()) || !DYNAMIC_CONTAINER_BOUNDS_CHECKS ? _items[id] : NULL;
}
