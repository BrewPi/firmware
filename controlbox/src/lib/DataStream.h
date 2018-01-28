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

#include <stdint.h>
#include <algorithm>

#ifndef DATASTREAM_ANNOTATIONS
#define DATASTREAM_ANNOTATIONS DEBUG
#endif

typedef uint8_t stream_size_t;

/**
 * An output stream that supports writing data. Optionally. annotations may also be written
 * to the stream, although these are entirely optional and should provide only
 * supplimental information.
 * @param data
 * @return
 */
struct DataOut
{
	virtual void writeAnnotation(const char* /*data*/) {}

	/**
	 * Writes a byte to the stream.
	 * @return {@code true} if the byte was successfully written, false otherwise.
	 */
	virtual bool write(uint8_t data)=0;

	bool write(int8_t data) { return write(uint8_t(data)); }
	bool write(char data) { return write(uint8_t(data)); }
	bool write(int data) { return write(uint8_t(data)); }

	/**
	 * Writes a number of bytes to the stream.
	 * @param data	The address of the data to write.
	 * @param len	The number of bytes to write.
	 * @return {@code true} if the byte was successfully written, false otherwise.
	 */
	virtual bool writeBuffer(const void* data, stream_size_t len) {
		const uint8_t* d = (const uint8_t*)data;
		while (len-->0) {
			if (!write(*d++))
				return false;
		}
		return true;
	}

	virtual void flush() {}

	virtual void close() {}
};

/**
 * An output stream that buffers data before writing.
 */
class BufferDataOut : public DataOut {
	uint8_t* buffer;
	uint8_t size;
	uint8_t pos;
public:

	BufferDataOut(uint8_t* _buffer, uint8_t _size)
		: buffer(_buffer), size(_size), pos(0)
	{
	}

	void reset() {
		pos = 0;
	}

	virtual bool write(uint8_t data) override;

	uint8_t bytesWritten() { return pos; }

	const uint8_t* data() {
		return buffer;
	}
};

/**
 * A DataOut implementation that discards all data.
 */
struct BlackholeDataOut : public DataOut {
	virtual bool write(uint8_t /*data*/) { return true; }
};

/**
 * A data input stream. The stream contents may be determined asynchronously.
 * hasNext() returns true if the stream may eventually produce a new item, false if the stream is closed.
 * next() fetches the next item from the stream. return value is undefined if available()==0.
 * peek() retrieves the next data in the stream without removing it. Result is undefined if
 * available() returns 0.
 * available() the number of times read can be called to retrieve valid data.
 */
struct DataIn
{
	/*
	 * Determines if there is potentially more data in this stream.
	 * Note that this is not dependent upon time and asynchronous delivery of data, but if the stream is still open.
	 */
	virtual bool hasNext() =0;

	/**
	 * Retrieves the next byte of data. The return value is only valid when `hasNext()` returns true.
	 */
	virtual uint8_t next() =0;

	/**
	 * Retrieves the next byte of data without removing it from the stream. The result is only valid if `available`
	 * previously returned a non-zero value.
	 */
	virtual uint8_t peek() =0;

	/**
	 * Determines how many bytes are available for reading from the stream without blocking.
	 */
	virtual unsigned available() =0;

	virtual ~DataIn() {}

	/*
	 * Unconditional read of {@code length} bytes.
	 */
	void read(void* t, uint8_t length) {
		uint8_t* target = (uint8_t*)t;
		while (length-->0 && hasNext()) {
			*target++ = next();
		}
	}

        /**
         * Writes the contents of this stream to an output stream.
         * @param out
         * @param length
         */
	void push(DataOut& out, uint8_t length) {
		while (length-->0 && hasNext()) {
			out.write(next());
		}
	}
};


/**
 * A DataIn that provides no data.
 */
class EmptyDataIn : public DataIn
{
	virtual bool hasNext() override { return false; }
	virtual uint8_t next() override { return 0; }
	virtual uint8_t peek() override { return 0; }
	virtual unsigned available() override { return 0; }
};

/*
 * Reads data from a DataIn, and writes the fetched bytes (if any) to DataOut.
 */
class PipeDataIn : public DataIn
{
	DataIn* _in;
	DataOut* _out;
	bool success;

public:
	PipeDataIn(DataIn& in, DataOut& out)
		: _in(&in), _out(&out), success(true)
	{
	}

	bool pipeOk() { return success; }

	DataOut& pipeOut() { return *_out; }

	virtual uint8_t next() override {
		uint8_t val = _in->next();
		bool result = _out->write(val);
		success = success && result;
		return val;
	}

	virtual bool hasNext() override { return _in->hasNext(); }
	virtual uint8_t peek() override { return _in->peek(); }
	virtual unsigned available() override { return _in->available(); }

};


/**
 * Provides a DataIn stream from a static buffer of data. The caller is assumed to know how long the buffer is..??!!?
 */
class BufferDataIn : public DataIn {
	const uint8_t* _data;
// todo - pass the size of the data array?
public:
	BufferDataIn(const void* data) : _data((const uint8_t*)data) {}

	uint8_t next() override { return *_data++; }
	bool hasNext() override { return true; }
	uint8_t peek() override { return *_data; }
	unsigned available() override { return 1; }
};

/**
 * Limits reading from the stream to the given number of bytes.
 */
class RegionDataIn : public DataIn {
	DataIn* in;
	uint8_t len;
public:
	RegionDataIn(DataIn& _in, uint8_t _len)
	: in(&_in), len(_len) {}

	bool hasNext() override { return len && in->hasNext(); }
	uint8_t next() override { return hasNext() ? len--, in->next() : 0; }
	uint8_t peek() override { return in->peek(); }
	unsigned available() override { return std::min(unsigned(len), in->available()); }

};

#define WRITE_ANNOTATION_STR(out, value) \
	WRITE_ANNOTATION(out, value)

#if DATASTREAM_ANNOTATIONS
	#define WRITE_ANNOTATION(out, value) \
		out->writeAnnotation(value);
#else
	#define WRITE_ANNOTATION(out, value)
#endif
