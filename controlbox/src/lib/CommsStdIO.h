
#include "ControlboxWiring.h"

#if !CONTROLBOX_WIRING
#include "Comms.h"
#include <iostream>
#include <string>
#include <thread>
#include <queue>
#include <functional>

namespace cbox {

class Stream {};


// trying to peek from stdio is problematic
// it is much simpler to run a daemon thread that pulls data as it's available.

class InputStreamPoll : public DataIn
{
	std::istream& in;
    std::thread thread;
	std::queue<uint8_t> queue;

public:
	InputStreamPoll(std::istream& in_) : in(in_), thread(std::bind(&InputStreamPoll::run, this)) {}

	void run()
	{
		// make it a daemon thread
		thread.detach();

		while (!in.eof())
		{
			char c;
			in.get(c);
			queue.push(uint8_t(c));
		}
	}

	virtual stream_size_t available() override final
	{
		return queue.size()>0;
	}

	virtual bool hasNext() override final
	{
		return !in.eof();
	}

	virtual uint8_t next() override final
	{
		uint8_t front = peek();
		queue.pop();
		return front;
	}

	virtual uint8_t peek() override final
	{
		return queue.front();
	}
};


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
    		uint8_t count = len;
    		while (count --> 0) {
    			write(*data++);
    		}
    		return len;
    }

    void flush();
    operator bool() { return in.hasNext(); }

private:
    InputStreamPoll in;
    FILE* out;
};


class StdIOConnection : public AbstractConnection<StdIO, StreamDataIn<StdIO>, StreamDataOut<StdIO>, StandardConnectionDataType>
{
	using base = AbstractConnection<StdIO, StreamDataIn<StdIO>, StreamDataOut<StdIO>, StandardConnectionDataType>;

	StdIO stdio;
	base::in_type in;
	base::out_type out;

public:
	StdIOConnection()
		: base(stdio, in, out), in(stdio), out(stdio)
	{
	}

	bool connected() override
	{
		return stdio;
	}
};

} // end namespace cbox

#else
namespace cbox {} // ensure namespace is declared in this situation too

#endif

