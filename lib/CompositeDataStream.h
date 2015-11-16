/*
 * Copyright 2014-2015 Matthew McGowan.
 *
 * This file is part of Nice Firmware.
 *
 * BrewPi is free software: you can redistribute it and/or modify
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
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "DataStream.h"
#include <functional>
#include <iterator>

#include <boost/range/iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>

template <typename Iterator> using DataInRange = boost::iterator_range<Iterator>;
template <typename Iterator> using DataOutRange = boost::iterator_range<Iterator>;

template <typename Iterator> using DataInRangeProvider = std::function<DataInRange<Iterator>(void)>;
template <typename Iterator> using DataOutRangeProvider = std::function<DataOutRange<Iterator>(void)>;


/**
 * A composite output stream.
 */
template <typename Iterator>
class CompositeDataOut : public DataOut
{
    using Streams = DataOutRangeProvider<Iterator>;
    Streams streams;

public:
    CompositeDataOut(Streams _streams) : streams(_streams) {}

    CompositeDataOut(const CompositeDataOut&) = delete;
    CompositeDataOut()=delete;

    virtual bool write(uint8_t data) override
    {
        bool result = true;
        for (auto& stream : streams()) {
            bool written = stream.write(data);
            result = result && written;
        }
        return result;
    }

    virtual bool writeBuffer(const void* data, stream_size_t len) override
    {
        bool result = true;
        for (auto& stream : streams()) {
            bool written = stream.writeBuffer(data, len);
            result = result && written;
        }
        return result;
    }

};

template <typename Iterator>
class CompositeDataIn : public DataIn
{
    using Streams = DataInRangeProvider<Iterator>;
    Streams streams;

    /**
     */
    DataIn* nextStream;


    void findNext() {
        if (!nextStream) {
            nextStream = findNextStream();
        }
    }

    /**
     * Retrieves the next stream that has a data byte ready now.
     * @return The stream that has a data byte ready. If there are still streams open
     * but none a ready, NULL is returned. If all streams are closed, this is returned.
     */
    DataIn* findNextStream() {
        bool open = false;
        for (auto& stream : streams()) {
            if (stream.hasNext()) {
                open = true;
                if (stream.peek()>=0) {
                    return &stream;
                }
            }
        }
        return open ? nullptr : this;
    }

public:
    CompositeDataIn(Streams _streams) : streams(_streams), nextStream(nullptr) {}
    CompositeDataIn() = delete;
    CompositeDataIn(const CompositeDataIn&) = delete;

    virtual bool hasNext() override {
        findNext();
        return this!=nextStream;
    }

    virtual uint8_t next() override {
        findNext();
        if (nextStream==nullptr || nextStream==this)
            return -1;
        return nextStream->next();
    }

    virtual uint8_t peek() override {
        findNext();
        if (nextStream==nullptr || nextStream==this)
            return -1;
        return nextStream->peek();
    }

    /**
     * Resets the state to look for another stream with data.
     */
    void reset() {
        nextStream = nullptr;
    }

};