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

typedef uint16_t stream_size_t;

/**
 * An output stream that supports writing data. Optionally. annotations may also be written
 * to the stream, although these are entirely optional and should provide only
 * supplemental information.
 * @param data
 * @return
 */
class DataOut
{
public:
	DataOut() = default;
	virtual ~DataOut() = default;

    virtual void writeAnnotation(const char* /*data*/) = 0;
	virtual void writeResponseSeparator() = 0;
	virtual void writeListSeparator() = 0;
	virtual void endMessage() = 0;

	/**
	 * Writes a byte to the stream.
	 * @return {@code true} if the byte was successfully written, false otherwise.
	 */
	virtual bool write(uint8_t data) = 0;

	bool write(int8_t data) { return write(uint8_t(data)); }
	bool write(char data) { return write(uint8_t(data)); }
	bool write(int data) { return write(uint8_t(data)); }

	template<typename T>
    bool put(const T & t){
        return writeBuffer(std::addressof(t), sizeof(T));
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

	virtual void flush() = 0;
};

/**
 * An output stream that buffers data before writing.
 */
class BufferDataOut : public DataOut {
private:
	uint8_t* buffer;
	stream_size_t size;
	stream_size_t pos;
public:

	BufferDataOut(uint8_t* _buffer, stream_size_t _size)
		: buffer(_buffer), size(_size), pos(0)
	{
	}
	virtual ~BufferDataOut() = default;

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

    virtual void writeAnnotation(const char* data) override final {};
    virtual void writeResponseSeparator() override final {};
    virtual void writeListSeparator() override final {};
    virtual void endMessage() override final {};
    virtual void flush() override final {};

};

/**
 * A DataOut implementation that discards all data.
 */
class BlackholeDataOut : public DataOut {
public:
    BlackholeDataOut() = default;
    virtual ~BlackholeDataOut() = default;
	virtual bool write(uint8_t /*data*/) override final { return true; }
};

/**
 * A DataOut implementation that discards all data, but counts each byte;
 */
class CountingBlackholeDataOut final : public DataOut {
private:
	stream_size_t counted;
public:    
	CountingBlackholeDataOut() : counted(0) {};
    virtual ~CountingBlackholeDataOut() = default;
    virtual bool write(uint8_t /*data*/) override final {
        ++counted;
        return true;
    }

    stream_size_t count(){
        return counted;
    }

    virtual void writeAnnotation(const char* data) override final {};
    virtual void writeResponseSeparator() override final {};
    virtual void writeListSeparator() override final {};
    virtual void endMessage() override final {};
    virtual void flush() override final {};
};

/**
 * A data input stream. The stream contents may be determined asynchronously.
 * hasNext() returns true if the stream may eventually produce a new item, false if the stream is closed.
 * next() fetches the next item from the stream. return value is undefined if available()==0.
 * peek() retrieves the next data in the stream without removing it. Result is undefined if
 * available() returns 0.
 * available() the number of times read can be called to retrieve valid data.
 */
class DataIn
{
public:
    virtual ~DataIn() = default;
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
		while (length > 0 && hasNext()) {
			out.write(next());
			--length;
		}
		return length == 0;
	}

	/**
     * Writes the contents of this stream to an output stream, until input stream is empty
     * @param out
     */
	bool push(DataOut& out) {
		bool success = true;
		while (hasNext()) {
			success &= out.write(next());
		}
		return success;
	}

    /**
     * Discards a number of bytes.
     * Can be overridden by child classes to skip actual reads for performance if the underlying structure can be accessed by index
     *
     * @param length
     * @return length was skipped (didn't encounter end of stream)
     */
	virtual bool spool(stream_size_t skip_length){
	    stream_size_t to_skip = skip_length;
	    while (to_skip > 0 && hasNext()){
	        next();
	        --to_skip;
	    }
        return to_skip == skip_length;
    }

	virtual void spool(){
	    while (hasNext()){
	        next();
		}
    }
};


/**
 * A DataIn that provides no data.
 */
class EmptyDataIn : public DataIn
{
    EmptyDataIn() = default;
    virtual ~EmptyDataIn() = default;
public:
	virtual bool hasNext() override { return false; }
	virtual uint8_t next() override { return 0; }
	virtual uint8_t peek() override { return 0; }
	virtual stream_size_t available() override { return 0; }
};

/*
 * Reads data from a DataIn, and also writes the fetched bytes (if any) to a DataOut.
 */
class TeeDataIn : public DataIn
{
	DataIn& in;
	DataOut& out;
	bool success;

public:
	TeeDataIn(DataIn& _in, DataOut& _out)
		: in(_in), out(_out), success(true)
	{
	}
	virtual ~TeeDataIn() = default;

	bool teeOk() { return success; }

	virtual uint8_t next() override {
		uint8_t val = in.next();
		bool result = out.write(val);
		success = success && result;
		return val;
	}

	virtual bool hasNext() override { return in.hasNext(); }
	virtual void spool() override { return in.spool(); }
	virtual uint8_t peek() override { return in.peek(); }
	virtual stream_size_t available() override { return in.available(); }

};

/*
 * A DataOut that writes to two other DataOut streams.
 */
class TeeDataOut final : public DataOut
{
public:
    TeeDataOut(DataOut& _out1, DataOut& _out2) : out1(_out1), out2(_out2){};
    virtual ~TeeDataOut() = default;

    virtual bool write(uint8_t data) override {
        bool res1 = out1.write(data);
        bool res2 = out2.write(data);
        return res1 || res2;
    }

    virtual void writeAnnotation(const char* data) override final {
        out1.writeAnnotation(data);
        out2.writeAnnotation(data);
    };
    virtual void writeResponseSeparator() override final {
        out1.writeResponseSeparator();
        out2.writeResponseSeparator();
    };
    virtual void writeListSeparator() override final {
        out1.writeListSeparator();
        out2.writeListSeparator();
    };
    virtual void endMessage() override final {
        out1.endMessage();
        out2.endMessage();
    };
    virtual void flush() override final {
        out1.flush();
        out2.flush();
    };

private:
    DataOut & out1;
    DataOut & out2;
};


/**
 * Provides a DataIn stream from a static buffer of data.
 */
class BufferDataIn : public DataIn {
	const uint8_t* data;
	stream_size_t size;
    stream_size_t pos;

public:
	BufferDataIn(const uint8_t * buf, stream_size_t len) : data(buf), size(len), pos(0) {}

	virtual uint8_t next() override { return data[pos++]; }
	virtual bool hasNext() override { return pos < size; }
	virtual uint8_t peek() override { return data[pos]; }
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
	virtual ~RegionDataIn() = default;

	bool hasNext() override {
	    return len && in.hasNext();
	}

	uint8_t next() override {
	    return hasNext() ? --len, in.next() : 0;
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
};

/**
 * Limits writing to the stream to the given number of bytes.
 */
class RegionDataOut final : public DataOut {
    DataOut* out; // use pointer to have assignment operator
    stream_size_t len;
public:
    RegionDataOut(DataOut& _out, stream_size_t _len)
    : out(&_out), len(_len) {}
    virtual ~RegionDataOut() = default;

    virtual bool write(uint8_t data) override {
        if(len > 0){
            --len;
            return out->write(data);
        }
        return false;
    }

    void setLength(stream_size_t len_){
        len = len_;
    }
    stream_size_t availableForWrite(){
        return len;
    }

    virtual void writeAnnotation(const char* data) override final { out->writeAnnotation(data); };
    virtual void writeResponseSeparator() override final { out->writeResponseSeparator(); };
    virtual void writeListSeparator() override final { out->writeListSeparator(); };
    virtual void endMessage() override final { out->endMessage(); };
    virtual void flush() override final { out->flush(); };
};

// copied from OneWire class. Should be refactored to only define this one

static const uint8_t dscrc_table[] = {
    0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
    157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
    35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
    190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
    70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
    219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
    101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
    248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
    140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
    17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
    175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
    50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
    202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
    87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
    233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
    116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};


/**
 * CRC data out. Sends running CRC of data that was sent on endMessage()
 */
class CrcDataOut final : public DataOut {
    DataOut& out; // use pointer to have assignment operator
    uint8_t crc;

public:
    CrcDataOut(DataOut& _out, uint8_t startCrc = 0) :
        out(_out),
        crc(startCrc)
    {}
    virtual ~CrcDataOut() = default;

    virtual bool write(uint8_t data) override final {
        crc = *(dscrc_table + (crc ^ data));
        return out.write(data);
    }

    virtual void endMessage() override final {
        out.write(crc);
        out.endMessage();
    }

    virtual void writeAnnotation(const char* data) override final {
        out.writeAnnotation(data);
    };
    virtual void writeResponseSeparator() override final {
        out.write(crc);
        crc = 0;
        out.writeResponseSeparator();
    };
    virtual void writeListSeparator() override final {
        out.write(crc);
        crc = 0;
        out.writeListSeparator();
    };
    virtual void flush() override final { out.flush(); };
};

}
