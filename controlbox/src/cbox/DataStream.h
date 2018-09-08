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

#include <algorithm>
#include <stdint.h>
#include <string>

namespace cbox {

typedef uint16_t stream_size_t;

/**
 * An output stream that supports writing data.
 * This is the base class for raw streams that do not encode their bytes as 2 hex characters
 */
class DataOut {
public:
    DataOut() = default;
    virtual ~DataOut() = default;

    /**
	 * Writes a byte to the stream.
	 * @return {@code true} if the byte was successfully written, false otherwise.
	 */
    virtual bool write(uint8_t data) = 0;

    bool write(int8_t data) { return write(uint8_t(data)); }
    bool write(char data) { return write(uint8_t(data)); }
    bool write(int data) { return write(uint8_t(data)); }

    template <typename T>
    bool put(const T& t)
    {
        return writeBuffer(std::addressof(t), sizeof(T));
    }

    /**
	 * Writes a number of bytes to the stream.
	 * @param data	The address of the data to write.
	 * @param len	The number of bytes to write.
	 * @return {@code true} if the byte was successfully written, false otherwise.
	 */
    virtual bool writeBuffer(const void* data, stream_size_t len)
    {
        const uint8_t* d = (const uint8_t*)data;
        while (len-- > 0) {
            if (!write(*d++))
                return false;
        }
        return true;
    }
};

/**
 * An output stream that supports encoded data. Each byte is encoded as hex.
 * Special characters are used for stream handling and injecting meta data:
 * input|output
 * <comment>
 * <!event>
 * , list separator
 * \n end of message
 */
class DataOutEncoded : public DataOut {
public:
    DataOutEncoded() = default;
    virtual ~DataOutEncoded() = default;

    virtual void writeResponseSeparator() = 0;
    virtual void writeListSeparator() = 0;
    virtual void endMessage() = 0;

    /**
	 * Annotations are written as is to the stream, surrounded by annotation marks.
	 */
    void writeAnnotation(std::string&& ann)
    {
        write('<');
        for (auto c : ann) {
            write(c);
        }
        write('>');
    }

    void writeEvent(std::string&& ann)
    {
        write('<');
        write('!');
        for (auto c : ann) {
            write(c);
        }
        write('>');
    }
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
        : buffer(_buffer)
        , size(_size)
        , pos(0)
    {
    }
    virtual ~BufferDataOut() = default;

    void reset()
    {
        pos = 0;
    }

    virtual bool write(uint8_t data) override
    {
        if (pos < size) {
            buffer[pos++] = data;
            return true;
        }
        return false;
    }

    stream_size_t bytesWritten() { return pos; }

    const uint8_t* data()
    {
        return buffer;
    }
};

/**
 * A DataOut implementation that discards all data.
 */
class BlackholeDataOut final : public DataOut {
public:
    BlackholeDataOut() = default;
    virtual ~BlackholeDataOut() = default;
    virtual bool write(uint8_t data) override final { return true; }
};

/**
 * A DataOut implementation that discards all data, but counts each byte;
 */
class CountingBlackholeDataOut final : public DataOut {
private:
    stream_size_t counted;

public:
    CountingBlackholeDataOut()
        : counted(0){};
    virtual ~CountingBlackholeDataOut() = default;
    virtual bool write(uint8_t data) override final
    {
        ++counted;
        return true;
    }

    stream_size_t count()
    {
        return counted;
    }
};

/**
 * A data input stream. The stream contents may be determined asynchronously.
 * hasNext() returns true if the stream may eventually produce a new item, false if the stream is closed.
 * next() fetches the next item from the stream. return value is undefined if available()==0.
 * peek() retrieves the next data in the stream without removing it. Result is undefined if
 * available() returns 0.
 * available() the number of times read can be called to retrieve valid data.
 */
class DataIn {
public:
    virtual ~DataIn() = default;
    /*
	 * Determines if there is potentially more data in this stream.
	 * Note that this is not dependent upon time and asynchronous delivery of data, but if the stream is still open.
	 */
    virtual bool hasNext() = 0;

    /**
	 * Retrieves the next byte of data. The return value is only valid when `hasNext()` returns true.
	 */
    virtual uint8_t next() = 0;

    /**
	 * Retrieves the next byte of data without removing it from the stream. The result is only valid if `available`
	 * previously returned a non-zero value.
	 */
    virtual uint8_t peek() = 0;

    /**
	 * Determines how many bytes are available for reading from the stream without blocking.
	 */
    virtual stream_size_t available() = 0;

    /**
	 * Discards all data until no new data is available
	 */
    void spool()
    {
        while (hasNext()) {
            next();
        }
    }

    /**
	 * Unconditional read of {@code length} bytes.
	 */
    bool read(void* t, stream_size_t length)
    {
        uint8_t* target = (uint8_t*)t;
        while (length-- > 0) {
            if (!hasNext()) {
                return false;
            }
            *target++ = next();
        }
        return true;
    }

    template <typename T>
    bool get(T& t)
    {
        return read(&t, sizeof(T));
    }

    /**
     * Writes the contents of this stream to an output stream.
     * @param out
     * @param length
     * @return length was written
     */
    bool push(DataOut& out, stream_size_t length)
    {
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
    bool push(DataOut& out)
    {
        bool success = true;
        while (hasNext()) {
            success &= out.write(next());
        }
        return success;
    }
};

/**
 * A DataIn that provides no data.
 */
class EmptyDataIn : public DataIn {
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
class TeeDataIn : public DataIn {
    DataIn& in;
    DataOut& out;
    bool success;

public:
    TeeDataIn(DataIn& _in, DataOut& _out)
        : in(_in)
        , out(_out)
        , success(true)
    {
    }
    virtual ~TeeDataIn() = default;

    bool teeOk() { return success; }

    virtual uint8_t next() override
    {
        uint8_t val = in.next();
        bool result = out.write(val);
        success = success && result;
        return val;
    }

    virtual bool hasNext() override { return in.hasNext(); }
    virtual uint8_t peek() override { return in.peek(); }
    virtual stream_size_t available() override { return in.available(); }
};

/*
 * A DataOut that writes to two other DataOut streams.
 */
class TeeDataOut final : public DataOut {
public:
    TeeDataOut(DataOut& _out1, DataOut& _out2)
        : out1(_out1)
        , out2(_out2){};
    virtual ~TeeDataOut() = default;

    virtual bool write(uint8_t data) override
    {
        bool res1 = out1.write(data);
        bool res2 = out2.write(data);
        return res1 || res2;
    }

private:
    DataOut& out1;
    DataOut& out2;
};

/**
 * Provides a DataIn stream from a static buffer of data.
 */
class BufferDataIn : public DataIn {
    const uint8_t* data;
    stream_size_t size;
    stream_size_t pos;

public:
    BufferDataIn(const uint8_t* buf, stream_size_t len)
        : data(buf)
        , size(len)
        , pos(0)
    {
    }

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
class RegionDataIn final : public DataIn {
    DataIn& in;
    stream_size_t len;

public:
    RegionDataIn(DataIn& _in, stream_size_t _len)
        : in(_in)
        , len(_len)
    {
    }
    virtual ~RegionDataIn() = default;

    bool hasNext() override final
    {
        return len && in.hasNext();
    }

    uint8_t next() override final
    {
        return hasNext() ? --len, in.next() : 0;
    }

    uint8_t peek() override final
    {
        return in.peek();
    }

    stream_size_t available() override final
    {
        return std::min(len, in.available());
    }

    void setLength(stream_size_t len_)
    {
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
        : out(&_out)
        , len(_len)
    {
    }
    virtual ~RegionDataOut() = default;

    virtual bool write(uint8_t data) override
    {
        if (len > 0) {
            --len;
            return out->write(data);
        }
        return false;
    }

    void setLength(stream_size_t len_)
    {
        len = len_;
    }
    stream_size_t availableForWrite()
    {
        return len;
    }
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
    116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53};

/**
 * CRC data out. Sends running CRC of data that was sent on endMessage()
 */
class HexCrcDataOut final : public DataOutEncoded {
    DataOutEncoded& out;
    uint8_t crcValue;

public:
    HexCrcDataOut(DataOutEncoded& _out)
        : out(_out)
        , crcValue(0)
    {
    }
    virtual ~HexCrcDataOut() = default;

    virtual bool write(uint8_t data) override final
    {
        crcValue = *(dscrc_table + (crcValue ^ data));
        return out.write(data);
    }

    virtual void endMessage() override final
    {
        out.write(crcValue);
        crcValue = 0;
        out.endMessage();
    }

    virtual void writeResponseSeparator() override final
    {
        // don't add CRC for the input, because it is already sent with the input command
        crcValue = 0;
        out.writeResponseSeparator();
    };
    virtual void writeListSeparator() override final
    {
        out.write(crcValue);
        crcValue = 0;
        out.writeListSeparator();
    };

    uint8_t crc()
    {
        return crcValue;
    }
};

/**
 * CRC data out. Sends running CRC of data on request
 */
class CrcDataOut final : public DataOut {
    DataOut& out;
    uint8_t crcValue;

public:
    CrcDataOut(DataOut& _out, uint8_t initial = 0)
        : out(_out)
        , crcValue(initial)
    {
    }
    virtual ~CrcDataOut() = default;

    virtual bool write(uint8_t data) override final
    {
        crcValue = *(dscrc_table + (crcValue ^ data));
        return out.write(data);
    }

    bool writeCrc()
    {
        return out.write(crcValue);
    }

    bool writeInvalidCrc()
    {
        return out.write(crcValue + 1);
    }

    uint8_t crc()
    {
        return crcValue;
    }
};

} // end namespace cbox
