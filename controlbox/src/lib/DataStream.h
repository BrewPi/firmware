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

namespace cbox {

#ifndef DATASTREAM_ANNOTATIONS
#define DATASTREAM_ANNOTATIONS CBOX_DEBUG
#endif

typedef uint16_t stream_size_t;

/**
 * An output stream that supports writing data. Optionally. annotations may also be written
 * to the stream, although these are entirely optional and should provide only
 * supplemental information.
 * @param data
 * @return
 */
struct DataOut
{
	DataOut() = default;
	virtual ~DataOut() = default;

    virtual void writeAnnotation(const char* /*data*/) {}
	virtual void writeResponseSeparator() {}

	/**
	 * Writes a byte to the stream.
	 * @return {@code true} if the byte was successfully written, false otherwise.
	 */
	virtual bool write(uint8_t data)=0;

	bool write(int8_t data) { return write(uint8_t(data)); }
	bool write(char data) { return write(uint8_t(data)); }
	bool write(int data) { return write(uint8_t(data)); }

	template<typename T>
	bool put(T t){
	    return writeBuffer(&t, sizeof(T));
	}

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
	stream_size_t size;
	stream_size_t pos;
public:

	BufferDataOut(uint8_t* _buffer, stream_size_t _size)
		: buffer(_buffer), size(_size), pos(0)
	{
	}

	void reset() {
		pos = 0;
	}

	virtual bool write(uint8_t data) override {
	    if (pos<size) {
	        buffer[pos++] = data;
	        return true;
	    }
	    return false;
	}

	stream_size_t bytesWritten() { return pos; }

	const uint8_t* data() {
		return buffer;
	}

};

/**
 * A DataOut implementation that discards all data.
 */
struct BlackholeDataOut : public DataOut {
    BlackholeDataOut() = default;
    virtual ~BlackholeDataOut() = default;
	virtual bool write(uint8_t /*data*/) override final { return true; }
};

/**
 * A DataOut implementation that discards all data, but counts each byte;
 */
struct CountingBlackholeDataOut : public DataOut {
    CountingBlackholeDataOut() : counted(0) {};
    virtual ~CountingBlackholeDataOut() = default;
    virtual bool write(uint8_t /*data*/) override final {
        ++counted;
        return true;
    }

    stream_size_t count(){
        return counted;
    }

    stream_size_t counted;
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
	virtual stream_size_t available() =0;

	virtual ~DataIn() {}

	/*
	 * Unconditional read of {@code length} bytes.
	 */
	bool read(void* t, stream_size_t length) {
		uint8_t* target = (uint8_t*)t;
		while (length-->0) {
            if(!hasNext()){
                return false;
            }
			*target++ = next();
		}
		return true;
	}

    template<typename T>
    bool get(T& t){
        return read(&t, sizeof(T));
    }

    /**
     * Writes the contents of this stream to an output stream.
     * @param out
     * @param length
     * @return length was written
     */
	bool push(DataOut& out, stream_size_t length) {
		while (length-->0 && hasNext()) {
			out.write(next());
		}
		return length == 0;
	}

    /**
     * Skips ahead a number of bytes
     * @param length
     * @return length was skipped (didn't encounter end of stream)
     */
	bool skip(stream_size_t length) {
        while (length-->0 && hasNext()) {
        }
        return length == 0;
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
	virtual stream_size_t available() override { return 0; }
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
	virtual stream_size_t available() override { return _in->available(); }

};

class TeeDataOut : public DataOut
{
public:
    TeeDataOut(DataOut& out1, DataOut& out2) : _out1(out1), _out2(out2){};
    virtual ~TeeDataOut() = default;


    virtual bool write(uint8_t data) override {
        bool res1 = _out1.write(data);
        bool res2 = _out2.write(data);
        return res1 || res2;
    }
private:
    DataOut & _out1;
    DataOut & _out2;
};


/**
 * Provides a DataIn stream from a static buffer of data.
 */
class BufferDataIn : public DataIn {
	const uint8_t* _data;
	stream_size_t size;
    stream_size_t pos;

public:
	BufferDataIn(const uint8_t *data, stream_size_t len) : _data(data), size(len), pos(0) {}

	virtual uint8_t next() override { return _data[pos++]; }
	virtual bool hasNext() override { return pos < size; }
	virtual uint8_t peek() override { return _data[pos]; }
	virtual stream_size_t available() override { return size - pos; }
	void reset() { pos = 0; };
	stream_size_t bytes_read() { return pos; };
};



/**
 * Limits reading from the stream to the given number of bytes.
 */
class RegionDataIn : public DataIn {
	DataIn& in;
	stream_size_t len;
public:
	RegionDataIn(DataIn &_in, stream_size_t _len)
	: in(_in), len(_len) {}

	bool hasNext() override {
	    return len && in.hasNext();
	}

	uint8_t next() override {
	    return hasNext() ? len--, in.next() : 0;
	}

	uint8_t peek() override {
	    return in.peek();
	}

	stream_size_t available() override {
	    return std::min(len, in.available());
	}

	void setLength(stream_size_t len_){
	    len = len_;
	}

    void spool() {
        while (in.hasNext()){
            in.next();
        }
    }
};

/**
 * Limits writing to the stream to the given number of bytes.
 */
class RegionDataOut : public DataOut {
    DataOut& out;
    stream_size_t len;
public:
    RegionDataOut(DataOut& _out, stream_size_t _len)
    : out(_out), len(_len) {}

    virtual bool write(uint8_t data) override {
        if(len-- > 0){
            return out.write(data);
        }
        return false;
    }
    void setLength(stream_size_t len_){
        len = len_;
    }
    stream_size_t availableForWrite(){
        return len;
    }
};

#define WRITE_ANNOTATION_STR(out, value) \
	WRITE_ANNOTATION(out, value)

#if DATASTREAM_ANNOTATIONS
	#define WRITE_ANNOTATION(out, value) \
		out->writeAnnotation(value);
#else
	#define WRITE_ANNOTATION(out, value)
#endif

}
