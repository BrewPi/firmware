#include <iostream>
#include <string>
#include <thread>
#include <queue>
#include <functional>
#include "DataStream.h"
#include "Connections.h"

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


class StdIOConnection : public StreamConnection<StdIO>
{
	using StreamConnection<StdIO>::StreamConnection;
	virtual ~StdIOConnection() = default;

	bool isConnected() override
	{
		return true;
	}
};

} // end namespace cbox