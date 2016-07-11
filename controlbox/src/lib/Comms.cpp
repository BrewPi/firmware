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

#define BOOST_NO_EXCEPTIONS
#define BOOST_DISABLE_ASSERTS

#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

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

cb_static_decl(static MockSerial commsDevice;)

#elif !defined(ARDUINO) && !defined(SPARK)

#include <CommsStdIO.h>

cb_static_decl(StdIO commsDevice;)
#else
#define commsDevice Serial
	#define CONTROLBOX_COMMS_USE_FLUSH 0
// for serial, flush waits until the output has been flushed. The flush is there just to ensure the output is not
// buffered, which it never is with serial.
#endif

#ifndef CONTROLBOX_COMMS_USE_FLUSH
#define CONTROLBOX_COMMS_USE_FLUSH 1
#endif

#if CONTROLBOX_STATIC
/**
 * Implements a DataIn interface on top of the static Comms stream.
 * @return
 */
class CommsIn : public DataIn
{
public:
	bool hasNext() override { return commsDevice; }			// hasNext true if stream is still open.
	uint8_t next() override { return commsDevice.read(); }
	uint8_t peek() override { return commsDevice.peek(); }
	unsigned available() override { return commsDevice.available(); }
};
#endif

struct CommsWriter
{
	virtual void write(uint8_t data)=0;
};

/**
 * Implements the `DataOut` interface in terms of the current comms device.
 */
class CommsOut : public DataOut
{
#if !CONTROLBOX_STATIC
	CommsWriter& commsDevice;
public:
	CommsOut(CommsWriter& s) : commsDevice(s) {}
#endif

public:
	void writeAnnotation(const char* s) {
		if (s && *s) {
			write('[');
			writeBuffer(s, strlen(s));
			write(']');
			write('\n');
		}
		flush();
	}

	bool write(uint8_t data) {
		commsDevice.write(data);
		return true;
	}
	void flush() {
	#if CONTROLBOX_STATIC && CONTROLBOX_COMMS_USE_FLUSH		// only flush for those stream types that require it
		commsDevice.flush();
	#endif
	}
};

// low-level binary in/out streams
cb_static_decl(CommsIn commsIn;)
cb_static_decl(CommsOut commsOut;)

#if CONTROLBOX_STATIC

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

#endif

#ifdef SPARK

/**
 * Delegate DataOut::close() to TCPClient::stop()
 */
template<> void StreamDataOut<TCPClient>::flush()
{
	// TCPClient on 0.5.0 firmware discards any unwritten data
};


/**
 * Delegate DataOut::close() to TCPClient::stop()
 */
template<> void StreamDataOut<TCPClient>::close()
{
    stream->stop();
};

/**
 * An  AbstractTCPConnection uses a TCPClient as the stream type.
 */
template <typename D>
using AbstractTCPConnection = AbstractStreamValueConnection<TCPClient, D>;

/**
 * The TCPConnection type
 */
typedef AbstractTCPConnection<StandardConnectionDataType> TCPConnection;

/**
 * Specializes AbstractStreamConnectionType for TCPClient.
 */
template<> bool AbstractStreamConnectionType<TCPClient, StandardConnectionDataType>::connected()
{
    return connection->connected();
}

/**
 * A vector of TCPConnection instances for connected clients.
 */
using NetworkConnections = std::vector<TCPConnection>;
/**
 * The static list of connections.
 */
cb_static_decl(NetworkConnections connections;)
#endif

/**
 * A std::function that fetches the corresponding DataOut interface from a connection.
 */
template <typename T>
struct ConnectionToDataOut : public std::function<DataOut&(T&)>
{
    typedef typename std::function<DataOut&(T&)> base_type;
    typedef typename base_type::result_type result_type;
    typedef typename base_type::argument_type argument_type;

    inline auto operator()(argument_type connection) const -> result_type {
        return connection.getDataOut();
    }
};

/**
 * A std::function that fetches the DataIn from a connection.
 */
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

cb_static_decl(typedef std::array<CommsConnection<StandardConnectionDataType>,1> CommsConnections;)

/**
 * The comms connection.
 */
cb_static_decl(CommsConnections commsConnections;)

template<typename C>
bool isDisconnected(C& connection)
{
    return !connection.connected();
}

#if defined(SPARK) && CONTROLBOX_STATIC

/**
 * Fetch a connection from the TCP server.
 */
TCPClient acceptConnection()
{
    static TCPServer server(8332);
    static bool started = false;
    if (!started) {
    		started = true;
    		server.begin();
    }
    TCPClient accept = server.available();
    return accept;
}

void manageConnection()
{
    // remove disconnected clients
    connections.erase(std::remove_if(connections.begin(), connections.end(), isDisconnected<TCPConnection>), connections.end());

    TCPClient client = acceptConnection();
    if (client.connected()) {
    		TCPConnection connection(client);
        connections.push_back(connection);
    }
}
#else
void manageConnection()
{

}
#endif

/**
 * A standard function that converts a connection to a pointer.
 */
struct ConnectionAsReference : public std::function<StandardConnection&(StandardConnection&)>
{
    typedef typename std::function<StandardConnection&(StandardConnection&)> base_type;
    typedef typename base_type::result_type result_type;
    typedef typename base_type::argument_type argument_type;

    StandardConnection& operator()(StandardConnection& connection) const {
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

#if CONTROLBOX_STATIC
// determine the global list of connections

#if defined(SPARK)
auto all_connections() -> boost::range::joined_range<boost::range_detail::transformed_range<ConnectionAsReference, std::array<CommsConnection<StandardConnectionDataType>, 1u> >, boost::range_detail::transformed_range<ConnectionAsReference, std::vector<AbstractStreamValueConnection<TCPClient, StandardConnectionDataType> > > >
{
    auto first = boost::adaptors::transform(commsConnections, ConnectionAsReference());

    auto second = boost::adaptors::transform(connections, ConnectionAsReference());
    auto result = boost::join(first, second);
    return result;
}
#else
auto all_connections() -> decltype(boost::adaptors::transform(commsConnections, ConnectionAsReference()))
{
    return boost::adaptors::transform(commsConnections, ConnectionAsReference());
}
#endif

/**
 * Iterator type that transforms all connections using a given transformation functor.
 */
template <typename TransformationFunctor>
using ConnectionTransformIterator = typename decltype(boost::adaptors::transform(all_connections(), TransformationFunctor()))::iterator;

/**
 *
 */
template <typename TransformFunctor>
// was auto fetch_streams() -> boost::iterator_range<typename decltype(boost::adaptors::transform(all_connections(), TransformFunctor()))::iterator>
// but this causes arm gcc 5.2 to segfault during linking
auto fetch_streams() -> decltype(boost::adaptors::transform(all_connections(), TransformFunctor()))
{
    return boost::adaptors::transform(all_connections(), TransformFunctor());
}

/**
 * Converts connections to a DataOut reference.
 */
using ToDataOutFunctor = ConnectionToDataOut<StandardConnection>;

/**
 *
 */
using CompositeDatOutType = CompositeDataOut<ConnectionTransformIterator<ToDataOutFunctor>>;
/**
 * A composite
 */
CompositeDatOutType compositeOut(fetch_streams<ToDataOutFunctor>);

#if 0
void f()
{
    std::function<boost::iterator_range<ConnectionTransformIterator<ToDataOutFunctor>>()> streams = fetch_streams<ToDataOutFunctor>;
    CompositeDatOutType compositeOut(streams);
}
#endif

// using ToDataInFunctor = ConnectionToDataIn<StandardConnectionDataType>;
// using CompositeDatInType = CompositeDataIn<ConnectionTransformIterator<ToDataInFunctor>>;
// CompositeDatInType compositeIn(&fetch_streams<ToDataInFunctor>);

#endif	// CONTROLBOX_STATIC




/**
 * Transform each TCPClient into a DataOut instance.
 */

void Comms::init() {
	prevConnected = false;
	reset = false;
    cb_static_decl(commsDevice.begin(57600);)
}


#ifdef BOOST_NO_EXCEPTIONS

namespace boost
{
    void throw_exception(std::exception const & e)
    {
    }
}

#endif




#if !defined(ARDUINO) || !defined(SPARK)
bool isHexadecimalDigit(char c)
{
	return isdigit(c) || (c>='A' && c<='F') || (c>='a' && c<='f');
}
#endif

/**
 * Fetches the next significant data byte from the stream.
 * Sets hasData and data.
 * @param set to true if more data is optional, true if data is expected and should be waited for.
 */
void TextIn::fetchNextData(bool optional) {
    optional = !inLine;
	while (commentLevel>=0 && !hasData && (_in->hasNext())) {
		if (_in->available()) {
			data = 0xFF;
			uint8_t d = _in->next();
            inLine = true;
			if (d=='[') {
				commentLevel++;
			}
			else if (d==']') {
				commentLevel--;
			}
			else if (d=='\n' || d=='\r') {
				commentLevel = -1; data = 0;    // exit the loop on end of line
	            inLine = false;
            }
			else if (!commentLevel && isHexadecimalDigit(d)) {
				hasData = true;
				data = d;
			}
		}
	}

}


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
	if (char2)		// already have data
		return;

	DataIn& _text = *this->_text;
	if (!_text.hasNext())
		return;

	if (!char1) {
		char1 = blockingRead(_text, 0xFF);
	}

	if (!_text.hasNext())
		return;

	if (!char2) {
		char2 = blockingRead(_text, 0xFF);
	}
}


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
    StandardConnectionDataType& data = connection.getData();
    if (!data.connected || (data.callback_until_first_request && !data.request_received && (++data.next_announcement>100))) {
    		if (!data.connected) {
    			data.connected = true;
			connection_count++;
    		}
		BinaryToHexTextOut out(connection.getDataOut());
		cb_nonstatic_decl(comms.)connectionStarted(connection, out);
		data.next_announcement = 0;
    }

    return connection.getDataIn().available() ? &connection : nullptr;
}

#if CONTROLBOX_STATIC
#define cmd_callback(x) commands.x
#else
#define cmd_callback(x) commands_ptr->x
#endif

inline void Comms::connectionStarted(StandardConnection& connection, DataOut& out)
{
	cmd_callback(connectionStarted(connection, out));
}

inline void Comms::handleCommand(DataIn& in, DataOut& out)
{
	cmd_callback(handleCommand(in, out));
}

/**
 * Called when the connection has at least one byte of data for the next command line.
 */
void processCommand(
#if !CONTROLBOX_STATIC
		Comms& comms,
#endif
		StandardConnection* connection)
{
    DataIn& dataIn = connection->getDataIn();
    DataOut& dataOut = connection->getDataOut();
    while (dataIn.available()) {
		  // there is some data ready to be processed											// form this point on, the system will block waiting for a complete command or newline.
		  TextIn textIn(dataIn);
		  HexTextToBinaryIn hexIn(textIn);
		  BinaryToHexTextOut hexOut(dataOut);
		  if (hexIn.hasNext()) {				// ignore blank newlines, annotations etc..
				comms.handleCommand(hexIn, hexOut);
				while (hexIn.hasNext())	{			// todo - log a message about unconsumed data?
						  hexIn.next();
				}
				connection->getData().request_received = true;
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
					*this,
#endif
					c);
		}
	}
	if (reset) {
		cmd_callback(handleReset(true));					// do the hard reset
	}
}
