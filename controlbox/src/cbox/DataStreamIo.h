/*
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
#include <iostream>

namespace cbox {

class IStreamDataIn : public DataIn {
    std::istream& in;

public:
    IStreamDataIn(std::istream& in_)
        : in(in_)
    {
    }

    virtual bool hasNext() override
    {
        int next = in.peek();
        // we don't want the peek to set the eof or fail flag because no input is available
        // this prevents streaming in new data for testing
        in.clear(in.rdstate() & std::istream::badbit); // only keep badbit
        return next != EOF;
    }

    virtual uint8_t next() override
    {
        char val = 0;
        if (hasNext()) {
            in.get(val);
        }
        return uint8_t(val);
    }

    virtual uint8_t peek() override
    {
        if (hasNext()) {
            return uint8_t(in.peek());
        }
        return 0;
    }

    virtual stream_size_t available() override
    {
        return hasNext() ? 1 : 0; // don't use in.eof() as the stream is already in error state then
    }
};

/**
 * Provides a DataOut stream by wrapping a std::ostream.
 */
class OStreamDataOut final : public DataOut {
    std::ostream& out;

public:
    OStreamDataOut(std::ostream& out_)
        : out(out_)
    {
    }

    virtual bool write(uint8_t data) override final
    {
        out.put(char(data));
        return true;
    }
};

} // end namespace cbox
