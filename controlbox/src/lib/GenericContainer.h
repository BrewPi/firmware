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


#pragma once

#include <stdlib.h>
#include "Values.h"
#include "Memops.h"

typedef void (*ObjectHandler)(Object*, void* data);

inline void do_update(Object* o, void* /*data*/) {
	o->update();
}

inline void do_prepare(Object* o, void* data) {
	prepare_t* result = (prepare_t*)data;
        prepare_t p = o->prepare();
        if (p>*result)
                *result = p;
}

#define DYNAMIC_CONTAINER_MAINTAIN_SIZE 1

/**
 * A container whose backing store is allocated dynamically as objects are added.
 */
class DynamicContainer: public OpenContainer
{
	private:
		Object** _items;	// the items in this container.
#if DYNAMIC_CONTAINER_MAINTAIN_SIZE
		container_id sz;
#endif
		void assign(container_id id, Object* item) {
			_items[id] = item;
		}

		bool expand(unsigned sz);

	public:
		static Object* create(ObjectDefinition& /*def*/) { return new DynamicContainer(); }

		void iterate_objects(void* data, ObjectHandler handler) {
			for (container_id i=0; i<_size(); i++) {
				// using the function to access the item is requires 10 bytes less space than
				// directly using _items[i]
				Object* obj = item(i);
				if (obj)
					handler(obj, data);
			}
		}

		virtual prepare_t prepare() override final {
			prepare_t time = 0;
			iterate_objects(&time, do_prepare);
			return time;
		}

		virtual void update() override final {
			iterate_objects(NULL, do_update);
		}

		DynamicContainer() {
			_items = (Object**)malloc(sizeof(Object*));
                        assign(0, NULL);
                        setSize(0);
		}

        inline void setSize(container_id size) {
#if DYNAMIC_CONTAINER_MAINTAIN_SIZE
            sz = size;
#endif
        }

		virtual Object*  item(container_id id) override final;

		virtual container_id next() override final;

		virtual bool add(container_id slot, Object* item) override final;

		virtual void remove(container_id id) override final;

		/*
		 * The number of items in this container.
		 */
		virtual container_id size() override final { return _size(); }

		inline container_id _size() {
#if DYNAMIC_CONTAINER_MAINTAIN_SIZE			// fetch the block size that preceeds the malloc'ed pointer
                        return sz;
#else
                        return container_id(((pointer_scalar)_items[-3])/sizeof(Object*));
#endif
		}


		virtual ~DynamicContainer() {
			for (int i=0; i<size();i++)
				delete_object(item(i));
			free(_items);
		}
};

/**
 * A container with space statically allocated.
 */
template<int SIZE> class StaticTemplateContainer : public OpenContainer
{
	private:
		Object* _items[SIZE];	// the items in this container.

		container_id freeSlot() {
			for (container_id i=0; i<SIZE;i++)
				if (!_items[i])
					return i;
			return -1;
		}

		void prepare(Object* item, prepare_t& time) {
			if (item)
				time = std::max(time, item->prepare());
		}

	public:
		prepare_t prepare() {
			prepare_t time = 0;
			for (container_id i=0; i<size(); i++ ) {
				prepare(item(i), time);
			}
			return time;
		}

		virtual void update() {
			for (container_id i=0; i<size(); i++ ) {
				Object* o = item(i);
				if (o)
				o->update();
			}
		}

		StaticTemplateContainer() {
			clear((uint8_t*)_items, SIZE*sizeof(Object*));
		}

		Object* item(container_id id) {
			return _items[id];
		}

		virtual container_id next() override final {
			return freeSlot();
		}

		virtual bool add(container_id slot, Object* item) override final {
			if (slot>=SIZE)
				return false;
			remove(slot);
			_items[slot] = item;
			return true;
		}

		virtual void remove(container_id id) override final {
			delete_object(_items[id]);
			_items[id] = NULL;
		}

		/*
		 * The number of items in this container.
		 */
		container_id size() { return SIZE; }

		virtual ~StaticTemplateContainer() {
			for (int i=0; i<SIZE;i++)
				delete_object(_items[i]);
		}
};


/**
 * A container with space statically allocated.
 */
class FixedContainer : public OpenContainer
{
	private:
		container_id SIZE;
		Object** _items;

		container_id freeSlot() {
			for (container_id i=0; i<size();i++)
				if (!_items[i])
					return i;
			return -1;
		}

		void prepare(Object* item, prepare_t& time) {
			if (item)
				time = std::max(time, item->prepare());
		}

	public:
		FixedContainer() : FixedContainer(0, nullptr) {}

		FixedContainer(container_id size, Object** items)
		: SIZE(size), _items(items) {
		}


		~FixedContainer() {
		    // We assume that a fixed container does not contain dynamically created objects, so they do not need to be deleted
		    // Trying to do so will result in segfault
		}

		virtual prepare_t prepare() override final {
			prepare_t time = 0;
			for (container_id i=0; i<size(); i++ ) {
				prepare(item(i), time);
			}
			return time;
		}

		virtual void update() override final {
			for (container_id i=0; i<size(); i++ ) {
				Object* o = item(i);
				if (o)
				o->update();
			}
		}


		virtual Object* item(container_id id) override final {
			return _items[id];
		}

		virtual container_id next() override final {
			return freeSlot();
		}

		virtual bool add(container_id slot, Object* item) override final {
			if (slot>=SIZE)
				return false;
			remove(slot);
			_items[slot] = item;
			return true;
		}

		virtual void remove(container_id id) override final{
			delete_object(_items[id]);
			_items[id] = NULL;
		}

		/*
		 * The number of items in this container.
		 */
		virtual container_id size() override final {
		    return SIZE;
        }


};
