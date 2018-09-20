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

#include <functional>
#include <iterator>

#include "DataStream.h"

namespace cbox {

template <typename DataOutContainer>
class CompositeDataOut : public DataOut {
    using TransformFunc = std::function<DataOut&(typename DataOutContainer::value_type& v)>;

private:
    DataOutContainer& container;
    TransformFunc transformFunc;

public:
    CompositeDataOut(DataOutContainer& c, const TransformFunc& func)
        : container(c)
        , transformFunc(func)
    {
    }

    CompositeDataOut(const CompositeDataOut&) = delete;

    virtual bool write(uint8_t data) override
    {
        bool result = true;
        for (auto& source : container) {
            DataOut& out = transformFunc(source);
            bool written = out.write(data);
            result = result && written;
        }
        return result;
    }

    virtual bool writeBuffer(const void* data, stream_size_t len) override
    {
        bool result = true;
        for (auto& source : container) {
            DataOut& out = transformFunc(source);
            bool written = out.writeBuffer(data, len);
            result = result && written;
        }
        return result;
    }
};

} // end namespace cbox
