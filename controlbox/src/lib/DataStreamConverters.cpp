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

#include "DataStreamConverters.h"
#include "DataStream.h"
#include <stdint.h>

namespace cbox {
/**
 * Fetches the next significant data byte from the stream.
 * Sets hasData and data.
 * @param set to true if more data is optional, true if data is expected and should be waited for.
 */

void
TextIn::fetchNextData(bool optional)
{
    optional = !inLine;
    while (commentLevel >= 0 && !hasData && (_in->hasNext())) {
        if (_in->available()) {
            data = 0xFF;
            uint8_t d = _in->next();
            inLine = true;
            if (d == '<') {
                commentLevel++;
            } else if (d == '>') {
                commentLevel--;
            } else if (d == '\n' || d == '\r') {
                commentLevel = -1;
                data = 0; // exit the loop on end of line
                inLine = false;
            } else if (!commentLevel && isxdigit(char(d))) {
                hasData = true;
                data = d;
            }
        }
    }
}

uint8_t
blockingRead(DataIn& in, uint8_t closed)
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
void
HexTextToBinaryIn::fetchNextByte()
{
    if (char2) { // already have data
        return;
    }
    if (!textIn.hasNext()) {
        return;
    }

    if (!char1) {
        char1 = blockingRead(textIn, 0xFF);
    }

    if (!textIn.hasNext())
        return;

    if (!char2) {
        char2 = blockingRead(textIn, 0xFF);
    }
}

/*
 * calculates 2 CRC characters to a hex string, used for testing
 */
std::string
crc(const std::string& in)
{
    std::string result;
    uint8_t crc = 0;
    for (auto it = in.begin(); it != in.end() && it + 1 != in.end();) {
        char char1 = *(it++);
        char char2 = *(it++);
        uint8_t data = (h2d(char1) << 4) | h2d(char2);
        crc = *(dscrc_table + (crc ^ data));
    }
    result.push_back(d2h(uint8_t(crc & 0xF0) >> 4));
    result.push_back(d2h(uint8_t(crc & 0xF)));

    return result;
}

/*
 * Appends 2 CRC characters to a hex string, used for testing
 */
std::string
addCrc(const std::string& in)
{
    return in + crc(in);
}

} // end namespace cbox
