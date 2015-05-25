#pragma once

#include "Brewpi.h"             // for max())
#include "Values.h"
#include "Memops.h"

typedef void (*ObjectHandler)(Object*, void* data);

inline void do_update(Object* o, void* data) {
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

		bool expand(uint8_t sz);
		
	public:
            static Object* create(ObjectDefinition& def) { return new DynamicContainer(); }
            
                void iterate_objects(void* data, ObjectHandler handler) {
			for (container_id i=0; i<_size(); i++) {
				// using the function to access the item is requires 10 bytes less space than
				// directly using _items[i]
				Object* obj = item(i);
				if (obj)
					handler(obj, data);
			}
		}
	
		prepare_t prepare() {
			prepare_t time = 0;
			iterate_objects(&time, do_prepare);
			return time;
		}
	
		virtual void update() {			
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
			
		Object* item(container_id id);
		
		container_id next();
		
		bool add(container_id slot, Object* item);
		
		void remove(container_id id);
		
		/*
		 * The number of items in this container.
		 */
		container_id size() { return _size(); }
			
		inline container_id _size() {
#if DYNAMIC_CONTAINER_MAINTAIN_SIZE			// fetch the block size that preceeds the malloc'ed pointer
                        return sz;
#else                    
                        return container_id(((pointer_scalar)_items[-3])/sizeof(Object*));
#endif                        
		}


#if OBJECT_VIRTUAL_DESTRUCTOR		
		// the contract says that before a container is deleted, the caller should ensure all contained objects
		// are also deleted. (if they were added.)
		~DynamicContainer() {			
			for (int i=0; i<size();i++)
				delete_object(item(i));
			free(_items);
		}
#endif		
	
};

/**
 * A container with space statically allocated.
 */
template<int SIZE> class StaticTemplateContainer : public OpenContainer
{
	private:
		Object* _items[SIZE];	// the items in this container.
		
		container_id freeSlot() {
			for (int i=0; i<SIZE;i++)
				if (!_items[i])
					return i;
			return -1;
		}
		
		void prepare(Object* item, prepare_t& time) {
			if (item)
				time = max(time, item->prepare());
		}

	public:
		prepare_t prepare() {
			prepare_t time = 0;
			for (int i=0; i<size(); i++ ) {
				prepare(item(i), time);
			}
			return time;
		}
	
		virtual void update() {
			for (int i=0; i<size(); i++ ) {
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
		
		container_id next() {
			return freeSlot();
		}
		
		bool add(container_id slot, Object* item) {
			if (slot>=SIZE)
				return false;				
			remove(slot);			
			_items[slot] = item;
			return true;
		}
		
		void remove(container_id id) {			
			delete_object(_items[id]);
			_items[id] = NULL;
		}
		
		/*
		 * The number of items in this container.
		 */
		container_id size() { return SIZE; }

#if OBJECT_VIRTUAL_DESTRUCTOR		
		// the contract says that before a container is deleted, the caller should ensure all contained objects
		// are also deleted. (if they were added.)
		~StaticContainer() {
			for (int i=0; i<SIZE;i++)
				delete_object(_items[i]);			
		}
#endif		
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
			for (int i=0; i<size();i++)
				if (!_items[i])
					return i;
			return -1;
		}
		
		void prepare(Object* item, prepare_t& time) {
			if (item)
				time = max(time, item->prepare());
		}

	public:
		FixedContainer(container_id size, Object** items)
		: SIZE(size), _items(items) {
			
			clear((uint8_t*)_items, SIZE*sizeof(Object*));
		}
		

		#if OBJECT_VIRTUAL_DESTRUCTOR
		// the contract says that before a container is deleted, the caller should ensure all contained objects
		// are also deleted. (if they were added.)
		~StaticContainer() {
			for (int i=0; i<SIZE;i++)
			delete_object(_items[i]);
		}
		#endif

		prepare_t prepare() {
			prepare_t time = 0;
			for (int i=0; i<size(); i++ ) {
				prepare(item(i), time);
			}
			return time;
		}
	
		virtual void update() {
			for (int i=0; i<size(); i++ ) {
				Object* o = item(i);
				if (o)
				o->update();
			}
		}
		
			
		Object* item(container_id id) {
			return _items[id];
		}
		
		container_id next() {
			return freeSlot();
		}
		
		bool add(container_id slot, Object* item) {
			if (slot>=SIZE)
				return false;				
			remove(slot);			
			_items[slot] = item;
			return true;
		}
		
		void remove(container_id id) {			
			delete_object(_items[id]);
			_items[id] = NULL;
		}
		
		/*
		 * The number of items in this container.
		 */
		container_id size() { return SIZE; }

		
};
