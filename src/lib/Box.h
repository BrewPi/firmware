/*
 * Copyright 2014-2015 Matthew McGowan.
 *
 * This file is part of Nice Firmware.
 *
 * BrewPi is free software: you can redistribute it and/or modify
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
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Ticks.h"
#include "EepromAccess.h"
#include "Comms.h"
#include "Commands.h"
#include "SystemProfile.h"


/**
 * Top-level object for running a controlbox
 */
class Box
{
	EepromAccess& eepromAccess_;
	Ticks& ticks_;
	Comms comms_;
	SystemProfile systemProfile_;
	Commands commands_;

public:
	Box(StandardConnection& connection, EepromAccess& eepromAccess, Ticks& ticks, CommandCallbacks& callbacks, Object** values, size_t size)
	: eepromAccess_(eepromAccess), ticks_(ticks), comms_(connection),
	  systemProfile_(eepromAccess, size, values), commands_(comms_, systemProfile_, callbacks, eepromAccess)
	{
	}

	void initialize()
	{
		systemProfile_.initialize();
		systemProfile_.activateDefaultProfile();
		comms_.init();
	}
};


/**
 * Factor the callbacks into a separate class to avoid multiple inheritance.
 */
struct AllCallbacks
{
	/* Connection */
    virtual DataOut& getDataOut()=0;
    virtual DataIn& getDataIn()=0;
    virtual bool connected()=0;

    /**
     * Retrieve the most-recently assigned value to the user data item.
     */
    virtual StandardConnectionDataType& getData()=0;

    /**
     * Assign a value to the user data item.
     */
    virtual void setData(StandardConnectionDataType&& d)=0;



	/* Ticks */
	virtual ticks_millis_t millis()=0;

	/* Eeprom */

	virtual uint8_t readByte(eptr_t offset) const=0;
	virtual void writeByte(eptr_t offset, uint8_t value)=0;
	virtual void readBlock(void* target, eptr_t offset, uint16_t size) const=0;
	virtual void writeBlock(eptr_t target, const void* source, uint16_t size)=0;

	virtual size_t length() const=0;

	/* Callbacks */

	/**
	 * Application-provided function that creates an object from the object definition.
	 */
	virtual Object* createApplicationObject(ObjectDefinition& def, bool dryRun=false)=0;

	/**
	 * Function prototype expected by the commands implementation to perform
	 * a reset.
	 * @param exit false on first call, true on second call. The first call (exit==false) is
	 * during command processing, so that actions can be taken before the command response is sent.
	 * The second call (exit==true) is called to perform the actual reset.
	 */
	virtual void handleReset(bool exit=true)=0;

	virtual void connectionStarted(DataOut& out)=0;

	virtual Container* createRootContainer()=0;

	/* DataOut */

	virtual void writeAnnotation(const char* data)=0;

	/**
	 * Writes a byte to the stream.
	 * @return {@code true} if the byte was successfully written, false otherwise.
	 */
	virtual bool write(uint8_t data)=0;

	/**
	 * Writes a number of bytes to the stream.
	 * @param data	The address of the data to write.
	 * @param len	The number of bytes to write.
	 * @return {@code true} if the byte was successfully written, false otherwise.
	 */
	virtual bool writeBuffer(const void* data, stream_size_t len)=0;

	virtual void close()=0;
};


class AllCallbacksDelegate : public StandardConnection, public CommandCallbacks, public Ticks, public EepromAccess, public DataOut
{
	AllCallbacks& cb;

public:
	AllCallbacksDelegate(AllCallbacks& cb_) : cb(cb_) {}


	/* Connection */
    virtual DataOut& getDataOut() {
    		return cb.getDataOut();
    }
    virtual DataIn& getDataIn() {
    		return cb.getDataIn();
    }
    virtual bool connected() {
    		return cb.connected();
    }

    /**
     * Retrieve the most-recently assigned value to the user data item.
     */
    virtual StandardConnectionDataType& getData() {
    		return cb.getData();
    }

    /**
     * Assign a value to the user data item.
     */
    virtual void setData(StandardConnectionDataType&& d) {
    		cb.setData(std::move(d));
    }


	/* Ticks */
	virtual ticks_millis_t millis() {
		return cb.millis();
	}

	/* Eeprom */

	virtual uint8_t readByte(eptr_t offset) const {
		return cb.readByte(offset);
	}
	virtual void writeByte(eptr_t offset, uint8_t value) {
		return cb.writeByte(offset, value);
	}
	virtual void readBlock(void* target, eptr_t offset, uint16_t size) const {
		return cb.readBlock(target, offset, size);
	}
	virtual void writeBlock(eptr_t target, const void* source, uint16_t size) {
		return cb.writeBlock(target, source, size);
	}

	virtual size_t length() const {
		return cb.length();
	}

	/* Callbacks */

	/**
	 * Application-provided function that creates an object from the object definition.
	 */
	virtual Object* createApplicationObject(ObjectDefinition& def, bool dryRun=false) {
		return cb.createApplicationObject(def, dryRun);
	}

	/**
	 * Function prototype expected by the commands implementation to perform
	 * a reset.
	 * @param exit false on first call, true on second call. The first call (exit==false) is
	 * during command processing, so that actions can be taken before the command response is sent.
	 * The second call (exit==true) is called to perform the actual reset.
	 */
	virtual void handleReset(bool exit=true) {
		return cb.handleReset(exit);
	}

	virtual void connectionStarted(DataOut& out) {
		return cb.connectionStarted(out);
	}

	virtual Container* createRootContainer() {
		return cb.createRootContainer();
	}

	/* DataOut */

	virtual void writeAnnotation(const char* data) {
		cb.writeAnnotation(data);
	}

	/**
	 * Writes a byte to the stream.
	 * @return {@code true} if the byte was successfully written, false otherwise.
	 */
	virtual bool write(uint8_t data) {
		return cb.write(data);
	}

	/**
	 * Writes a number of bytes to the stream.
	 * @param data	The address of the data to write.
	 * @param len	The number of bytes to write.
	 * @return {@code true} if the byte was successfully written, false otherwise.
	 */
	virtual bool writeBuffer(const void* data, stream_size_t len) {
		return cb.writeBuffer(data, len);
	}

	virtual void close() {
		cb.close();
	}

};

/**
 * Convenience class that provides virtual methods for all pluggable functions.
 */
class AllInOneBox : public Box
{
public:
	AllInOneBox(AllCallbacksDelegate& cb, Object** values=nullptr, size_t size=0)
		: Box(cb, cb, cb, cb, values, size)
	{}


};

