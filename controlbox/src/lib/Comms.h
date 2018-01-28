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

#include "Static.h"
#include "DataStream.h"
#include <string.h>

#if !CONTROLBOX_STATIC
#include <array>
#include <functional>
#endif

class Commands;


/**
 * Represents a connection to an endpoint. The details of the endpoint are not provided here.
 * A connection has these components:
 *
 * - a stream for input data (DataIn)
 * - a stream for output data (DatOut)
 * - a connected flag: indicates if this connection can read/write data to the resource
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
    virtual const D& getData() const=0;

};

/**
 * Connection implementation helper that adds storage for the user data and trivial accessor/mutator methods.
 */
template <typename D>
class ConnectionData : public Connection<D>
{
    D data;
public:
    ConnectionData() {
        memset(&data, 0, sizeof(data)); // todo - must be a better way than this, e.g. templated initialization function?
    }
    virtual D& getData() override { return data; }
    virtual const D& getData() const override { return data; }
};


/**
 * Adapts a Stream instance to DataIn.
 */
template <class S>
class StreamDataIn : public DataIn
{
protected:
    S* stream;
public:

    StreamDataIn(S& _stream) : stream(&_stream) {}

    virtual bool hasNext() override {
        return stream->available()>0;
    }

    virtual uint8_t next() override {
        return uint8_t(stream->read());
    };

    virtual uint8_t peek() override {
        return uint8_t(stream->peek());
    }

    virtual unsigned available() override {
        return unsigned(stream->available());
    }

};


/**
 * Wraps a stream to provide the DataOut interface.
 */
template <class S>
class StreamDataOut : public DataOut
{
protected:
    /**
     * The stream type that is adapted to a DataOut instance.
     * non-NULL.
     */
	S* stream;
public:

    StreamDataOut(S& _stream) : stream(&_stream) {}

    bool write(uint8_t data) override {
        return stream->write(data)!=0;
    }

    virtual bool writeBuffer(const void* data, stream_size_t length) override {
    		return stream->write((const uint8_t*)data, length)==length;
    }

    void flush() override;

    /**
     * The close method is defined by the specific template instantiation.
     */
    void close() override;

	//StreamDataOut& operator=(const StreamDataOut& rhs)=delete;
};



template <typename C, typename I, typename O, typename D>
class AbstractConnection : public ConnectionData<D>
{
public:
    typedef C connection_type;
    using in_type = I;
    using out_type = O;
    using data_type = D;

protected:
    connection_type* connection;
    in_type* in;
    out_type* out;


public:
    AbstractConnection()=default;
    ~AbstractConnection()=default;
    AbstractConnection(connection_type& _connection, in_type& _in, out_type& _out) :
	    connection(&_connection), in(&_in), out(&_out) {}

    virtual DataIn& getDataIn() override { return *in; }
    virtual DataOut& getDataOut() override { return *out; }
    virtual bool connected() override { return true; }

};

#include "ControlboxWiring.h"
#if CONTROLBOX_WIRING
template <typename S, typename D>
using AbstractStreamConnectionType = AbstractConnection<
    typename std::enable_if<std::is_base_of<Stream, S>::value, S>::type,
    StreamDataIn<S>,
    StreamDataOut<S>,
    D
>;

/**
 * Maintains a reference to the stream.
 */
template <typename S, typename D>
class AbstractStreamConnection : public AbstractStreamConnectionType<S,D>
{
    using base_type = AbstractStreamConnectionType<S,D>;

    StreamDataIn<S> streamIn;
    StreamDataOut<S> streamOut;

public:
    AbstractStreamConnection(S& _connection) :
    		base_type(_connection, streamIn, streamOut), streamIn(_connection), streamOut(_connection) {}

    AbstractStreamConnection(const AbstractStreamConnection& other) = delete;

};

/**
 * Maintains the stream by value.
 */
template <typename S, typename D>
class AbstractStreamValueConnection : public AbstractStreamConnection<S,D>
{
    using base_type = AbstractStreamConnection<S,D>;

    S stream;
public:
    AbstractStreamValueConnection(S& _connection) : base_type(stream), stream(_connection) {}

    AbstractStreamValueConnection(const AbstractStreamValueConnection& other) :
    		base_type(stream), stream(other.stream)
    {
    		this->getData() = other.getData();
    }

};


#endif


struct StandardConnectionDataType
{
	bool connected;

	bool callback_until_first_request;

	uint32_t next_announcement;

	/**
	 * Initially false, set to true
	 */
	bool request_received;

};

/**
 * A connection with a boolean flag to indicate if it has been initialized or not.
 */
typedef Connection<StandardConnectionDataType> StandardConnection;



/*
 * A DataIn filter - wraps a DataIn instance and provides also a DataIn interface.
 * Filters out non-significant text - comment markers, whitespace, unrecognized characters.
 * The stream automatically closes on newline and hasNext() returns false.
 * Once a character has been received, the underlying stream is continually polled for characters until
 * the stream is closed by the newline.
 *
 * The result of this is that lines are polled non-blocking while no data is available, and when data is available
 * the stream blocks for each character until the entire line is read.
 */
class TextIn : public DataIn {
    DataIn*	_in;
    uint8_t data;
    bool hasData;
    bool inLine;
    int8_t commentLevel;	// -1 indicates end of stream

    void fetchNextData(bool optional);

public:
    TextIn(DataIn& in)
            : _in(&in), data(0), hasData(0), inLine(false), commentLevel(0) {}

    bool hasNext() override
    {
        fetchNextData(true);
        return hasData;
    }

    uint8_t next() override
    {
        fetchNextData(false);
        hasData = false;
        return data;
    }

    uint8_t peek() override
    {
        fetchNextData(true);
        return data;
    }

    unsigned available() override
    {
        return hasNext();
    }


    bool isClosed()
    {
        return commentLevel<0;
    }
};


/**
 * Converts a hex digit to the corresponding binary value.
 */
inline uint8_t h2d(unsigned char hex)
{
	if (hex > '9')
		hex -= 7; // 'A' is 0x41, 'a' is 0x61. -7 =  0x3A, 0x5A
	return uint8_t(hex & 0xf);
}

inline uint8_t d2h(uint8_t bin)
{
	return uint8_t(bin+(bin>9 ? 'A'-10 : '0'));
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

    bool hasData() { return char2; }

public:
	HexTextToBinaryIn(DataIn& text) : _text(&text), char1(0), char2(0) {}

	bool hasNext() override {
		return hasData() || _text->hasNext();
	}

	uint8_t peek() override {
		while (!hasData() && _text->hasNext()) {
            fetchNextByte();
        }
		return uint8_t((h2d(char1)<<4) | h2d(char2));
	}

	uint8_t next() override  {
		uint8_t r = peek();
		char1 = 0; char2 = 0;
		return r;
	}

    unsigned available() override {
        fetchNextByte();
        return hasData() ? 1 : 0;
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
	 * Annotations are written as is to the stream, surrounded by annotation marks.
	 */
	virtual void writeAnnotation(const char* data) override final {
		_out->write('[');
		_out->writeBuffer(data, stream_size_t(strlen(data)));
		_out->write(']');
        _out->write('\n');
	}

	/**
	 * Data is written as hex-encoded
	 */
	virtual bool write(uint8_t data) override final {
		_out->write(d2h(uint8_t(data&0xF0)>>4));
		_out->write(d2h(uint8_t(data&0xF)));
		_out->write(' ');
		return true;
	}

	/**
	 * Rather than closing the global stream, write a newline to signify the end of this command.
	 */
	virtual void close() override final {
		_out->write('\r');
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
	Comms(StandardConnection& connection) : prevConnected(false), reset(false), connection_(connection), hexOut(connection.getDataOut()) {}

	void setCommands(Commands& commands)
	{
		commands_ptr = &commands;
	}

	inline std::array<std::reference_wrapper<StandardConnection>,1> all_connections()
	{
		std::array<std::reference_wrapper<StandardConnection>,1> result = { { std::ref(connection_) } };
		return result;
	}
#else

	/**
	 * The composite output used for unsolicited responses.
	 */
	static DataOut& hexOut;

#endif

	void connectionStarted(StandardConnection& connection, DataOut& out);

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
