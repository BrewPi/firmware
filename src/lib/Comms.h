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

#include "Static.h"
#include "DataStream.h"

#if !CONTROLBOX_STATIC
#include <array>
#include <functional>
#endif

class Commands;


/**
 * A connection has these components:
 * - a stream for input data (DataIn)
 * - a stream for output data (DatOut)
 * - a connected flag
 * - associated user data
 *
 */
template <typename D>
struct Connection
{
    virtual DataOut& getDataOut()=0;
    virtual DataIn& getDataIn()=0;
    virtual bool connected()=0;

    /**
     * Retrieve the most-recently assigned value to the user data item.
     */
    virtual D& getData()=0;

    /**
     * Assign a value to the user data item.
     */
    virtual void setData(D&& d)=0;

};

/**
 * Connection implementation helper that adds storage for the user data and trivial accessor/mutator methods.
 */
template <typename D>
class ConnectionData : public Connection<D>
{
    D data;
public:
    virtual D& getData() { return data; }
    virtual void setData(D&& d) { std::swap(data,d); }
};


typedef bool StandardConnectionDataType;

/**
 * A connection with a boolean flag to indicate if it has been initialized or not.
 */
typedef Connection<StandardConnectionDataType> StandardConnection;


/**
 * Converts a hex digit to the corresponding binary value.
 */
inline uint8_t h2d(unsigned char hex)
{
	if (hex > '9')
		hex -= 7; // 'A' is 0x41, 'a' is 0x61. -7 =  0x3A, 0x5A
	return (hex & 0xf);
}

inline unsigned char d2h(uint8_t bin)
{
	return bin+(bin>9 ? 'A'-10 : '0');
}

/*
 * Converts pairs of hex digit characters into the corresponding binary value.
 */
class HexTextToBinaryIn : public DataIn
{
	DataIn* _text;	// store as pointer to avoid non-POD warnings
	uint8_t char1;	// Text character for upper nibble
	uint8_t char2;	// Text character for lower nibble

	void fetchNextByte();

public:
	HexTextToBinaryIn(DataIn& text) : _text(&text), char1(0), char2(0) {}

	bool hasNext() override {
		fetchNextByte();
		return char2;
	}

	uint8_t peek() override {
		fetchNextByte();
		return (h2d(char1)<<4) | h2d(char2);
	}

	uint8_t next() override  {
		uint8_t r = peek();
		char1 = 0; char2 = 0;
		return r;
	}

        unsigned available() override {
            return hasNext();
        }
};


/**
 * A DataOut decorator that converts from the 8-bit data bytes to ASCII Hex.
 */
class BinaryToHexTextOut : public DataOut {

	DataOut* _out;

public:

	BinaryToHexTextOut(DataOut& out) : _out(&out) {}

	/**
	 * Annotations are written as is to the stream, surrouned by annotation marks.
	 */
	void writeAnnotation(const char* data) {
		_out->write('[');
		_out->writeBuffer(data, strlen(data));
		_out->write(']');
	}

	/**
	 * Data is written as hex-encoded
	 */
	bool write(uint8_t data) {
		_out->write(d2h((data&0xF0)>>4));
		_out->write(d2h((data&0xF)));
		_out->write(' ');
		return true;
	}

	/**
	 * Rather than closing the global stream, write a newline to signify the end of this command.
	 */
	void close() {
		_out->write('\n');
	}
};




/**
 * The primary communications interface.
 */
class Comms {

	cb_static bool prevConnected;
	cb_static bool reset;

public:

#if !CONTROLBOX_STATIC
private:
	Commands* commands_ptr;
	StandardConnection& connection_;
	BinaryToHexTextOut hexOut;

public:
	Comms(StandardConnection& connection) : hexOut(connection.getDataOut()), prevConnected(false), reset(false), connection_(connection) {}

	void setCommands(Commands& commands)
	{
		commands_ptr = &commands;
	}

	inline std::array<std::reference_wrapper<StandardConnection>,1> all_connections()
	{
		std::array<std::reference_wrapper<StandardConnection>,1> result = { std::ref(connection_) };
		return result;
	}
#else

	/**
	 * The composite output used for unsolicited responses.
	 */
	static DataOut& hexOut;

#endif

	void connectionStarted(DataOut& out);

	void handleCommand(DataIn& in, DataOut& out);

	cb_static void init();

	/*
	 * Read and process from the commms link.
	 */
	cb_static void receive();

	cb_static void resetOnCommandComplete();

	/**
	 * Output stream. Used to write data after command processing.
	 */
	inline cb_static DataOut& dataOut() { return hexOut; }
};

#if CONTROLBOX_STATIC
	/**
	 * The global instance. Allows the same calling code to be used for static and non-static methods.
	 */
	extern Comms comms;
#endif
