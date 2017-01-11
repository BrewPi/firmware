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


#include "GenericContainer.h"

#define DYNAMIC_CONTAINER_BOUNDS_CHECKS 0

container_id DynamicContainer::next() {
	container_id sz = _size();
	for (container_id i=0; i<sz;i++)
		if (!_items[i])
			return i;
	return !((sz+1)&0x80) ? sz : -1;
}

bool DynamicContainer::expand(unsigned sz_)
{
	if (sz_>=unsigned(std::numeric_limits<container_id>::max()))
		return false;
	container_id sz = container_id(sz_);

	// some useful details on malloc/realloc here - http://www.nongnu.org/avr-libc/user-manual/malloc.html
	void* _newitems = realloc(_items, sz_*sizeof(Object*));
	if (_newitems) {
		container_id prev_sz = size();
		_items = (Object**)_newitems;
		while (prev_sz<sz)	{
			assign(prev_sz++, NULL);
		}
        setSize(container_id(sz));
	}
	return _newitems!=0;
}

bool DynamicContainer::add(container_id slot, Object* item) {
	if (slot<0 || (slot>=size() && !expand(unsigned(slot)+1)))
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
