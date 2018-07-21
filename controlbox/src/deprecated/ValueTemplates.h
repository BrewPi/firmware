#include "Object.h"

namespace cbox {

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

		object_flags_t objectFlags() {
			return ObjectFlags::Value | ObjectFlags::hasStateFlag;
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

		uint8_t readStreamSize() { return sizeof(this->value); }
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

	object_flags_t objectFlags() {
		return MixinReadValue<T>::objectFlags() | ObjectFlags::WritableFlag;
	}

};


/**
 * A Readable value.
 */
template<typename T> class BasicReadValue : public MixinReadValue<T>, public ReadableObject, public Readable<T>
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

	uint8_t readStreamSize() {
		return inherited::readStreamSize();
	}

};

/**
 * A readable and writable value.
 */
template <typename T>
class BasicReadWriteValue : public MixinReadWriteValue<T>, public ReadableObject, public Readable<T>, public Writable<T>
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

	uint8_t readStreamSize() {
		return inherited::readStreamSize();
	}
};


class ValueSource
{
	bool getValue(void* value, uint8_t id=0);
};

} // end namespace cbox
