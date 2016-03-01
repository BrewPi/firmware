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

#define BOOST_NO_EXCEPTIONS
#define BOOST_DISABLE_ASSERTS

#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include "Platform.h"
#include "Ticks.h"
#include "Comms.h"
#include "Commands.h"
#include "Integration.h"


#include "CompositeDataStream.h"

#include <vector>
#include <algorithm>
#include <type_traits>
#include <array>

#if CONTROLBOX_EMULATE
class MockSerial : public Stream
{
	public:
	void print(char c) {}
	void print(const char* c) {}
	void printNewLine() {}
	void println() {}
	int read() { return -1; }
	int available() { return -1; }
	void begin(unsigned long) {}
	size_t write(uint8_t w) { return 1; }
	int peek() { return -1; }
	void flush() { };
	operator bool() { return true; }
};

static MockSerial commsDevice;

#elif !defined(ARDUINO) && !defined(SPARK)
class Stream {};

class StdIO : public Stream {
public:
    StdIO();

    void print(char c);
    void print(const char* c);
    void printNewLine();
    void println();
    int available();
    void begin(unsigned long);

    int read();
    int peek();

    size_t write(uint8_t w);
    size_t write(const uint8_t* data, uint8_t len) {
        return write(data, size_t(len));
    }
    void flush();
    operator bool() { return true; }

private:
    std::istream& in;
    FILE* out;
};

#include <CommsStdIO.inc>

StdIO commsDevice;
#else
#define commsDevice Serial
	#define CONTROLBOX_COMMS_USE_FLUSH 0
// for serial, flush waits until the output has been flushed. The flush is there just to ensure the output is not
// buffered, which it never is with serial.
#endif

#ifndef CONtROLBOX_COMMS_USE_FLUSH
#define CONTROLBOX_COMMS_USE_FLUSH 1
#endif


/**
 * Implements a DataIn interface on top of the static Comms stream.
 * @return
 */
class CommsIn : public DataIn
{
	bool hasNext() override { return commsDevice; }			// hasNext true if stream is still open.
	uint8_t next() override { return commsDevice.read(); }
	uint8_t peek() override { return commsDevice.peek(); }
        unsigned available() override { return commsDevice.available(); }
};

class CommsOut : public DataOut
{
public:
	void writeAnnotation(const char* s) {
		if (s && *s) {
			write('[');
			writeBuffer(s, strlen(s));
			write(']');
			write('\n');
		}
	}

	bool write(uint8_t data) {
		commsDevice.write(data);
		return true;
	}
	void flush() {
	#if CONTROLBOX_COMMS_USE_FLUSH		// only flush for those stream types that require it
		commsDevice.flush();
	#endif
	}
};

// low-level binary in/out streams
CommsIn commsIn;
CommsOut commsOut;



/**
 * Wraps a stream to provide the DataOut interface.
 */
template <class S>
class StreamDataOut : public DataOut
{
protected:
    S stream;
public:

    StreamDataOut(const S& _stream) : stream(_stream) {}

    bool write(uint8_t data) {
        return stream.write(data)!=0;
    }

    bool writeBuffer(const uint8_t* data, size_t length) {
        return stream.write(data, length)==length;
    }
    void close();
};

#ifdef SPARK
template<> void StreamDataOut<TCPClient>::close()
{
    stream.stop();
};
#endif

/**
 * Adapts a Stream instance to DataIn.
 */
template <class S>
class StreamDataIn : public DataIn
{
protected:
    S stream;
public:

    StreamDataIn(const S& _stream) : stream(_stream) {}

    virtual bool hasNext() override {
        return stream.available()>0;
    }

    virtual uint8_t next() override {
        return stream.read();
    };

    virtual uint8_t peek() override {
        return stream.peek();
    }

    virtual unsigned available() override {
        return stream.available();
    }

};



/**
 * All active connections are maintained in a vector.
 * A connection has
 * why is a connection not a stream? (meaning arduino's combined read/write stream.)
 * no universal way to know if the stream is still connected/open.
 */

template <typename D>
struct Connection
{
    virtual DataOut& getDataOut()=0;
    virtual DataIn& getDataIn()=0;
    virtual bool connected()=0;

    virtual D& getData()=0;
    virtual void setData(D&& d)=0;

};

template <typename D>
class ConnectionData : public Connection<D>
{
    D data;
public:
    virtual D& getData() { return data; }
    virtual void setData(D&& d) { std::swap(data,d); }
};

/**
 * A connection that delegates to the global commsOut and commsIn instances.
 */
template<typename D>
struct CommsConnection : public ConnectionData<D>
{
    virtual DataOut& getDataOut() override {
        return commsOut;
    }

    virtual DataIn& getDataIn() override {
        return commsIn;
    }

    virtual bool connected() override {
        return commsDevice;
    }
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
    connection_type connection;
    in_type in;
    out_type out;


public:
    AbstractConnection() {}
    AbstractConnection(const connection_type& _connection, const in_type& _in, const out_type& _out) :
    connection(_connection), in(_in), out(_out) {}

    virtual DataIn& getDataIn() override { return in; }
    virtual DataOut& getDataOut() override { return out; }
    virtual bool connected() override;

};


template <typename S, typename D>
using AbstractStreamConnectionType = AbstractConnection<
    typename std::enable_if<std::is_base_of<Stream, S>::value, S>::type,
    StreamDataIn<S>,
    StreamDataOut<S>,
    D
>;

template <typename S, typename D>
struct AbstractStreamConnection : public AbstractStreamConnectionType<S,D>
{
    using base_type = AbstractStreamConnectionType<S,D>;

    AbstractStreamConnection(const S& _connection)
            : base_type(_connection, _connection, _connection) {}

};

typedef bool ConnectionDataType;

#ifdef SPARK
template <typename D>
using AbstractTCPConnection = AbstractStreamConnection<TCPClient, D>;

template<> bool AbstractStreamConnectionType<TCPClient,ConnectionDataType>::connected()
{
    return connection.connected();
}


typedef AbstractTCPConnection<ConnectionDataType> TCPConnection;
#endif

template <typename D>
struct ConnectionToDataOut : public std::function<DataOut&(Connection<D>&)>
{
    typedef typename std::function<DataOut&(Connection<D>&)> base_type;
    typedef typename base_type::result_type result_type;
    typedef typename base_type::argument_type argument_type;

    auto operator()(argument_type connection) const -> result_type {
        return connection.getDataOut();
    }
};

template <typename D>
struct ConnectionToDataIn : public std::function<DataIn&(Connection<D>&)>
{
    typedef std::function<DataIn&(Connection<D>&)> base_type;
    typedef typename base_type::result_type result_type;
    typedef typename base_type::argument_type argument_type;

    auto operator()(Connection<D>& connection) const -> result_type {
        return connection.getDataIn();
    }
};

typedef Connection<ConnectionDataType> StandardConnection;

#ifdef SPARK
typedef std::vector<TCPConnection> Connections;
/**
 *
 */
Connections connections;

#endif


// we could combine all connections into a variant type and store in the connections
// vector. Keeping them separate is simpler.
static std::array<CommsConnection<ConnectionDataType>,1> commsConnections;

template<typename C>
bool isDisconnected(C& connection)
{
    return !connection.connected();
}

#ifdef SPARK
TCPClient acceptConnection()
{
    static TCPServer server(8332);
    server.begin();
    TCPClient accept = server.available();
    return accept;
}

void manageConnection()
{
    // remove disconnected clients
    connections.erase(std::remove_if(connections.begin(), connections.end(), isDisconnected<TCPConnection>), connections.end());

    TCPClient client = acceptConnection();
    if (client)
        connections.push_back(TCPConnection(client));
}
#else
void manageConnection()
{

}
#endif

template <typename T>
struct ConnectionAsPointer : public std::function<StandardConnection&(T&)>
{
    typedef typename std::function<StandardConnection&(T)> base_type;
    typedef typename base_type::result_type result_type;
    typedef typename base_type::argument_type argument_type;

    StandardConnection& operator()(T& connection) const {
        return connection;
    }
};

/*
template <typename T>
inline ConnectionAsPointer<T> ConnectionAsPointerFunctor() {
    return ConnectionAsPointer<T>();
}

template <typename T>
inline auto as_connection_ptr(T& source) -> decltype(boost::adaptors::transform(source, ConnectionAsPointerFunctor())) {
    boost::adaptors::transform(source, ConnectionAsPointerFunctor());
}
*/

#ifdef SPARK
auto all_connections() -> boost::range::joined_range<
        boost::range_detail::transformed_range<ConnectionAsPointer<CommsConnection<ConnectionDataType> >, decltype(commsConnections) >,
        boost::range_detail::transformed_range<ConnectionAsPointer<TCPConnection>, decltype(connections)> >
{
    auto first = boost::adaptors::transform(commsConnections, ConnectionAsPointer<CommsConnection<ConnectionDataType>>());
    auto second = boost::adaptors::transform(connections, ConnectionAsPointer<TCPConnection>());

    auto result = boost::join(first, second);
    return result;
}
#else
auto all_connections() -> boost::range_detail::transformed_range<ConnectionAsPointer<CommsConnection<ConnectionDataType> >, decltype(commsConnections) >
{
	 auto first = boost::adaptors::transform(commsConnections, ConnectionAsPointer<CommsConnection<ConnectionDataType>>());
	 return first;
}
#endif

/**
 * Iterator type that transforms all connections using a given transformation functor.
 */
template <typename TransformationFunctor> using ConnectionTransformIterator = typename decltype(boost::adaptors::transform(all_connections(), TransformationFunctor()))::iterator;


template <typename TransformFunctor>
auto fetch_streams() -> boost::iterator_range<typename decltype(boost::adaptors::transform(all_connections(), TransformFunctor()))::iterator>
{
    return boost::adaptors::transform(all_connections(), TransformFunctor());
}


typedef ConnectionToDataOut<ConnectionDataType> ToDataOutFunctor;
typedef CompositeDataOut<ConnectionTransformIterator<ToDataOutFunctor>> CompositeDatOutType;

typedef ConnectionToDataIn<ConnectionDataType> ToDataInFunctor;
typedef CompositeDataIn<ConnectionTransformIterator<ToDataInFunctor>> CompositeDatInType;


void f()
{
    std::function<boost::iterator_range<ConnectionTransformIterator<ToDataOutFunctor>>()> streams = fetch_streams<ToDataOutFunctor>;
    CompositeDatOutType compositeOut(streams);
}


CompositeDatOutType compositeOut(&fetch_streams<ToDataOutFunctor>);
//CompositeDatInType compositeIn(fetch_streams<ToDataInFunctor>);



/**
 * Transform each TCPClient into a DataOut instance.
 */

void Comms::init() {
	prevConnected = false;
	reset = false;
    commsDevice.begin(57600);
}


#ifdef BOOST_NO_EXCEPTIONS


namespace boost
{

    void throw_exception(std::exception const & e)
    {

    }

}

#endif



/*
 * A DataIn filter - wraps a DataIn instance and provides also a DataIn interface.
 * Filters out non-significant text - comment markers, whitespace, unrecognized characters.
 * The stream automatically closes on newline and hasNext() returns false.
 */
class TextIn : public DataIn {
	DataIn*	_in;
	uint8_t data;
	bool hasData;
	int8_t commentLevel;	// -1 indicates end of stream

	void fetchNextData(bool optional);

public:
	TextIn(DataIn& in)
	: _in(&in), data(0), hasData(0), commentLevel(0) {}

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
};

#if !defined(ARDUINO) || !defined(SPARK)
bool isHexadecimalDigit(char c)
{
	return isdigit(c) || (c>='A' && c<='F') || (c>='a' || c<='f');
}
#endif

/**
 * Fetches the next significant data byte from the stream.
 * Sets hasData and data.
 */
void TextIn::fetchNextData(bool optional) {

	while (commentLevel>=0 && !hasData && _in->hasNext()) {
                if (_in->available()) {
                    data = 0xFF;
                    uint8_t d = _in->next();
                    if (d=='[') {
                        commentLevel++;
                    }
                    else if (d==']') {
                        commentLevel--;
                    }
                    else if (d=='\n' || d=='\r') {
                        commentLevel = -1; data = 0;    // exit the loop on end of line
                    }
                    else if (!commentLevel && isHexadecimalDigit(d)) {
                        hasData = true;
                        data = d;
                    }
                }
	}
}

/**
 * Converts a hex digit to the corresponding binary value.
 */
uint8_t h2d(unsigned char hex)
{
	if (hex > '9')
		hex -= 7; // 'A' is 0x41, 'a' is 0x61. -7 =  0x3A, 0x5A
	return (hex & 0xf);
}

unsigned char d2h(uint8_t bin)
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

uint8_t blockingRead(DataIn& in, uint8_t closed)
{
    uint8_t result = closed;
    while (in.hasNext()) {
        if (in.available()) {
            result = in.next();
            break;
        }
    }
    return result;
}

/**
 * Fetches the next byte from the stream.
 */
void HexTextToBinaryIn::fetchNextByte()
{
	if (char1)		// already have data
		return;

	DataIn& _text = *this->_text;
	if (!_text.hasNext())
		return;

	if (!char1) {
                char1 = blockingRead(_text, 0);
	}

	if (!_text.hasNext())
		return;

	if (!char2) {
		char2 = blockingRead(_text, 0);
	}
}

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

cb_static_decl(BinaryToHexTextOut hexOut(compositeOut);)
cb_static_decl(DataOut& Comms::hexOut = ::hexOut;)
cb_static_decl(bool Comms::prevConnected;)
cb_static_decl(bool Comms::reset;)
cb_static_decl(Comms comms;)


void Comms::resetOnCommandComplete() {
	reset = true;
}

StandardConnection* handleConnection(
#if !CONTROLBOX_STATIC
		Comms& comms,
#endif
		StandardConnection& connection)
{
    static uint16_t connection_count = 0;
    bool connected = connection.getData();
    if (!connected) {
        connection.setData(true);
        connection_count++;
		cb_nonstatic_decl(comms.)connectionStarted(connection.getDataOut());
    }

    return connection.getDataIn().available() ? &connection : nullptr;
}

#if CONTROLBOX_STATIC
#define cmd_callback(x) commands.x
#else
#define cmd_callback(x) commands_ptr->x
#endif

inline void Comms::connectionStarted(DataOut& out)
{
	cmd_callback(connectionStarted(out));
}

inline void Comms::handleCommand(DataIn& in, DataOut& out)
{
	cmd_callback(handleCommand(in, out));
}

void processCommand(
#if !CONTROLBOX_STATIC
		Comms& comms,
		DataOut& hexOut,
#endif
		StandardConnection* connection)
{
    DataIn& dataIn = connection->getDataIn();
    while (dataIn.hasNext()) {
		  // there is some data ready to be processed											// form this point on, the system will block waiting for a complete command or newline.
		  TextIn textIn(dataIn);
		  HexTextToBinaryIn hexIn(textIn);
		  if (hexIn.hasNext()) {				// ignore blank newlines, annotations etc..
				comms.handleCommand(hexIn, hexOut);
				while (hexIn.hasNext())	{			// todo - log a message about unconsumed data?
						  hexIn.next();
				}
		}
		hexOut.close();
    }
}

void Comms::receive()
{
	if (reset)
		return;

	manageConnection();

	for (auto& connection : all_connections())
	{
		StandardConnection* c = handleConnection(
#if !CONTROLBOX_STATIC
				*this,
#endif
				connection);
		if (c) {
			processCommand(
#if !CONTROLBOX_STATIC
					*this, hexOut,
#endif
					c);
		}
	}
	if (reset) {
		cmd_callback(handleReset(true));					// do the hard reset
	}
}

