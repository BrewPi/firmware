/*
 * Copyright 2014-2015 Matthew McGowan.
 * Copyright 2018 BrewBlox / Elco Jacobs
 *
 * This file is part of Controlbox.
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

#include "DataStream.h"
#include <string>

namespace cbox {

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

#if 0 // unused currently. Comments in input not supported.
class TextIn : public DataIn {
    DataIn* _in;
    uint8_t data;
    bool hasData;
    bool inLine;
    int8_t commentLevel; // -1 indicates end of stream

    void fetchNextData(bool optional);

public:
    TextIn(DataIn& in)
        : _in(&in)
        , data(0)
        , hasData(0)
        , inLine(false)
        , commentLevel(0)
    {
    }

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

    stream_size_t available() override
    {
        return hasNext();
    }

    bool isClosed()
    {
        return commentLevel < 0;
    }
};
#endif

inline bool
isdigit(char c)
{
    return c >= '0' && c <= '9';
}

inline bool
isxdigit(char c)
{
    return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

/**
 * Converts a hex digit to the corresponding binary value.
 */
inline uint8_t
h2d(unsigned char hex)
{
    if (hex > '9') {
        hex -= 7; // 'A' is 0x41, 'a' is 0x61. -7 =  0x3A, 0x5A
    }
    return uint8_t(hex & 0xf);
}

inline uint8_t
d2h(uint8_t bin)
{
    return uint8_t(bin + (bin > 9 ? 'A' - 10 : '0'));
}

/*
 * Converts pairs of hex digit characters into the corresponding binary value.
 */
class HexTextToBinaryIn : public DataIn {
    DataIn& textIn;
    uint8_t char1; // Text character for upper nibble
    uint8_t char2; // Text character for lower nibble

    void fetchNextByte();

    bool hasData() { return char2; }

    bool peekEndline()
    {
        auto inByte = textIn.peek();
        return (inByte == '\r' || inByte == '\n');
    }

public:
    HexTextToBinaryIn(DataIn& _textIn)
        : textIn(_textIn)
        , char1(0)
        , char2(0)
    {
    }

    bool hasNext() override
    {
        return hasData() || (textIn.hasNext() && !peekEndline());
    }

    uint8_t peek() override
    {
        while (!hasData() && textIn.hasNext()) {
            fetchNextByte();
        }
        return uint8_t((h2d(char1) << 4) | h2d(char2));
    }

    uint8_t next() override
    {
        uint8_t r = peek();
        char1 = 0;
        char2 = 0;
        return r;
    }

    stream_size_t available() override
    {
        fetchNextByte();
        return hasData() ? 1 : 0;
    }

    void unBlock()
    {
        while (peekEndline()) {
            textIn.next();
        }
    }
};

/**
 * A DataOut decorator that converts from the 8-bit data bytes to ASCII Hex.
 */
class BinaryToHexTextOut final : public DataOutEncoded {
private:
    DataOut& out;

public:
    BinaryToHexTextOut(DataOut& _out)
        : out(_out)
    {
    }

    virtual void writeResponseSeparator() override final
    {
        out.write('|');
    }

    virtual void writeListSeparator() override final
    {
        out.write(',');
    }

    /**
	 * Data is written as hex-encoded
	 */
    virtual bool write(uint8_t data) override final
    {
        out.write(d2h(uint8_t(data & 0xF0) >> 4));
        out.write(d2h(uint8_t(data & 0xF)));
        //	out.write(' ');
        return true;
    }

    /**
	 * Rather than closing the global stream, write a newline to signify the end of this command.
	 */
    virtual void endMessage() override final
    {
        out.write('\n');
    }
};

// helper function for testing. Appends the CRC to a hex string, the same way CrcDataOut would do
std::string
addCrc(const std::string& in);
std::string
crc(const std::string& in);

} // end namespace cbox
