#pragma once

#include "stddef.h"
#include "stdint.h"
#include "DataStream.h"
#include "EepromAccess.h"

typedef int8_t container_id;

const container_id INVALID_ID = (container_id)(-1);

typedef uint16_t prepare_t;


enum ObjectType {		
	otObject = 0,		
	otValue = 4,			// 0x000001xx are for value types. Base value type is stream only readable.
	otValueWrite = 5,		// value is writable (either state and/or stream as indicated.)
	otValueState = 6,		// value state is readable 
	otValueWriteState = 7,	// value state is writable (and readable) and streamable
	otWritableFlag = 1,		// flag for writable values
	otValueStateFlag = 2,	// flag for values that can get set state
	otContainer = 8,
	otOpenContainerFlag = 1,// value to flag that a container supports the OpenContainer interface (that the container is writable.)
	otNotLogged = 16,		// flag to indicate that a value is not logged normally	
	otStaticlyAllocated = 32
};

typedef uint8_t object_t;

// if no objects require cleanup, then we can do away with the virtual destructor, saving quite a bit of space (several hundred bytes.)
#ifndef OBJECT_VIRTUAL_DESTRUCTOR
#define OBJECT_VIRTUAL_DESTRUCTOR 0
#endif

#if OBJECT_VIRTUAL_DESTRUCTOR
#define delete_object(x) delete (x)
#else
// cast to a byte array and delete that. Net effect is that just the memory is freed without running any destructors.
#define delete_object(x) delete ((uint8_t*)x)
#endif

// have a hook for all object creations.
#define new_object(x) new x

#define cast_object_ptr(t, x) ((t*)x)

struct Object
{
	virtual object_t objectType() { return otObject; }

	/**
	 * Notifies this object that it has been created and is operational in the system.
	 * The eeprom address that contains the object's definition is provided for instances
	 * that want to retrieve or amend their definition details. 
	 * @param eeprom_address offset in eeprom that defines the data for this object. the length 		
	 * Preceeding this address is the length, then id_chain, and before that, the creation command. (0x03)
	 */
	virtual void rehydrated(eptr_t eeprom_address) {}

	/**
	 * Prepare this object for subsequent updates. 
	 * The returned value is the number of milliseconds the object needs before updates can be performed.
	 */
	virtual prepare_t prepare() { return 0; }
	
	/**
	 * Called after prepare to update this object's state.
	 */
	virtual void update() { }
		
	
#if OBJECT_VIRTUAL_DESTRUCTOR	
	virtual ~Object() {}
#endif		
};

const uint8_t MAX_CONTAINER_DEPTH = 8;
const container_id MAX_CONTAINER_ID = 127;

/**
 * A container that you cannot open. You can see the objects inside the container, but not add new ones.
 */
struct Container : public Object
{
	virtual object_t objectType() { return otContainer; }

	/**
	 * Fetches the object with the given id.
	 * /return the object with that id, which may be null.
	 *
	 * After retrieving the item, callers must call returnItem()
	 */
	virtual Object* item(container_id id) { return NULL; }
		
	/**
	 * Returns a previously fetched item back the container.
	 */
	virtual void returnItem(Object* item) { }
	
	/*
	 * The maximum number of items in this container. Calling {@link #item()} at an index less than this value 
	 * may return {@code NULL}. This is provided so callers know
	 * the upper limit for indexes to iterate over for this container.
	 */
	virtual container_id size() { return 0; }
		
};


/**
 * A container that creates it's contained items on demand.
 */
class FactoryContainer : public Container {
public:	
	
	/**
	 * Deletes the item. This assumes item was created on-demand by the item() method.
	 */
	virtual void returnItem(container_id id, Object* item) {
		delete_object(item);
	}
};


/**
 * A container of objects that is open - i.e. you can put things in it.
 */
class OpenContainer : public Container
{
public:	
	object_t objectType() { return otContainer | otOpenContainerFlag; }

	/*
	 * Add the given object to the container at the given slot.
	 * The container guarantees the object will be available at the slot until removed.
	 * @param index	The index of the slot. >=0.
	 * @param item	The object to add.
	 * @return non-zero on success, zero on error. 
	 * The current size of the container may be less than the slot. If the container can resize
	 * to make additional slots available, it should do so, but this is an optional operation for
	 * fixed size containers.
	 */
	virtual bool add(container_id index, Object* item) { return false; }
		
	/**
	 * Determines the next available free slot in this container.
	 * @return A value greater or equal to zero - the next available free slot. Negative value
	 *	indicates no more free slots.
	 */
	virtual container_id next() { return -1; }
		
	/**
	 * Removes the item at the given index. 
	 * @param id	The id of the item to remove.
	 * If there is no item at the given index, or the item has already been removed the method does nothing.
	 */
	virtual void remove(container_id id) { }
	
};

/**
 * A basic value type. All values are as a minimum stream readable, meaning they can push their value to a stream 
 * (a streamed read operation.)
 */
class Value : public Object {
public:	
	virtual object_t objectType() { return otValue; }	// basic value type - read only stream
	virtual void readTo(DataOut& out)=0;
	virtual uint8_t streamSize()=0;			// the size this value occupies in the stream.
	
	virtual void writeMaskedFrom(DataIn& in, DataIn& mask){};	// default is a no-op. Caller always checks if item is writable first.	
};

class WritableValue : public Value {
public:	
	virtual object_t objectType() { return otValueWrite; }		
	virtual void writeMaskedFrom(DataIn& dataIn, DataIn& maskIn)=0;
	
	static uint8_t nextMaskedByte(uint8_t current, DataIn& dataIn, DataIn& maskIn) {
			uint8_t next = dataIn.next();
			uint8_t mask = maskIn.next();			
			return (next & mask) | (current & ~mask);		
	}
};

class RehydratedAwareObject : public Object
{
	eptr_t address;
public:

	void rehydrated(eptr_t _address) {
		address = _address;
	}

	eptr_t eeprom_offset() { return address; }
	uint8_t streamSize() { return eepromAccess.readByte(address-1); }
		
};

/**
 * Classes that can provide a representation of their state implement this interface.
 */
template<typename T> 
class Readable
{	
public:	
	/**
	 * Retrieve the state representing the value of this instance.
	 * @return The value.
	 */
	virtual T read()=0;
};

/**
 * Classes that can update their internal state from a given value implement this interface.
 */
template<class T> class Writable
{
public:	
	/**
	 * Writes to this value.
     * @param t	The new value this Value should have.
	 */
    virtual void write(T t)=0;
};

/**
 * A basic state- and stream- readable value. 
 * This class is intended as a base class for Value implementations.
 */
template<typename T> 
class MixinReadValue
{
    protected:
        T value;
        
		void writeFrom(DataIn& in) {
			in.read((uint8_t*)&this->value, sizeof(this->value));
		}
		
    public:
        MixinReadValue(T t) 
        : value(t)
        {}
        
		object_t objectType() {
			return otValue | otValueStateFlag;
		}
		
        T read() {
            return value;
        }
		
		/**
		 * This is not part of the writable interface, but provided for classes that are using this as a cache
		 * for some other value. Externally, this value is not writable, but the immediate client needs to be able to set the value.
		 */
		void assign(T t) {
			value = t;
		}
		
		void readTo(DataOut& out) {			
			out.writeBuffer(&value, sizeof(value));
		}
	
		uint8_t streamSize() { return sizeof(this->value); }
};

/**
 * A state and stream writable value. 
 */
template<typename T> 
class MixinReadWriteValue : public MixinReadValue<T>
{	
public:	
	MixinReadWriteValue(T initial=0)
	: MixinReadValue<T>(initial)
	{}
		
	object_t objectType() {
		return otValue | otValueStateFlag | otWritableFlag;
	}
	
};


/**
 * A Readable value.
 */
template<typename T> class BasicReadValue : public MixinReadValue<T>, public Value, public Readable<T>
{	
public:
	BasicReadValue(T t=T()) : MixinReadValue<T>(t) {}

	typedef MixinReadValue<T> inherited;


	T read() {
		return inherited::read();
	}
	
	void readTo(DataOut& out) {
		inherited::readTo(out);
	}
	
	uint8_t streamSize() {
		return inherited::streamSize();
	}

};

/**
 * A readable and writable value.
 */
template <typename T> 
class BasicReadWriteValue : public MixinReadWriteValue<T>, public Value, public Readable<T>, public Writable<T>
{
public:
	BasicReadWriteValue(T t=T()) : MixinReadWriteValue<T>(t) {}
		
	typedef MixinReadWriteValue<T> inherited;
		
	virtual void write(T t) {
		inherited::assign(t);
	}
	
	virtual void writeFrom(DataIn& in) {
		inherited::writeFrom(in);
	}

	T read() {
		return inherited::read();
	}	
	
	void readTo(DataOut& out) {
		inherited::readTo(out);
	}
	
	uint8_t streamSize() {
		return inherited::streamSize();
	}
};


class ValueSource
{
	bool getValue(void* value, uint8_t id=0);
};

/**
 * Definition parameters for creating a new object.
 */
struct ObjectDefinition {
	DataIn* in;		// stream providing definition data for this object
	uint8_t len;		// number of bytes in the stream for this object definition
	uint8_t type;

	void spool();
};

inline bool hasFlags(uint8_t value, uint8_t flags) {
    return ((value&flags)==flags);
}

inline bool isContainer(Object* o)
{
	return o!=NULL && (hasFlags(o->objectType(), otContainer));
}

inline bool isOpenContainer(Object* o)
{
	return o!=NULL && (hasFlags(o->objectType(), (otContainer|otOpenContainerFlag)));
}

inline bool isValue(Object* o)
{
	return o!=NULL && (hasFlags(o->objectType(), otValue));
}

inline bool isLoggedValue(Object* o)
{
	return o!=NULL && (o->objectType() & (otValue|otNotLogged))==otValue;
}

inline bool isDynamicallyAllocated(Object* o)
{
	return o!=NULL && (o->objectType() & otStaticlyAllocated)==0;
}

inline bool isWritable(Object* o)
{
	return o!=NULL && (hasFlags(o->objectType(), otWritableFlag));	
}


/**
 * The host app should provide the root container, configured with any defaults for the app.
 */
Container* rootContainer();

/*
 * Callback function for enumerating objects.
 * The function can return true to stop enumeration. 
 *
 * @param enter	When {@code true} this call is entering this portion of the hierarchy. This is called before any
 *   child objects have been enumerated. 		
 *		When {@code false} this call is exiting this portion of the hierarchy. This is called after all
 *   child objects have been enumerated.
 */
typedef bool (*EnumObjectsFn)(Object* obj, void* data, container_id* id, bool enter);


bool walkContainer(Container* c, EnumObjectsFn callback, void* data, container_id* id, container_id* end);

bool walkObject(Object* obj, EnumObjectsFn callback, void* data, container_id* id, container_id* end);

/**
 * Enumerate all objects the root container and child containers. 
 */
inline bool walkRoot(EnumObjectsFn callback, void* data, container_id* id) {
	return walkContainer(rootContainer(), callback, data, id, id);
}

/**
 * Read the id chain from the stream and resolve the corresponding object.
 */
Object* lookupObject(Object* current, DataIn& data);

/**
 * Read the id chain from the stream and resolve the container and the final index.
 */
Container* lookupContainer(Object* current, DataIn& data, int8_t& lastID);


/**
 * Read the id chain from the stream and resolve the corresponding object.
 */
Object* lookupUserObject(DataIn& data);

/**
 * Read the id chain from the stream and resolve the container and the final index.
 */
OpenContainer* lookupUserOpenContainer(DataIn& data, int8_t& lastID);

int16_t read2BytesFrom(Value* value);
