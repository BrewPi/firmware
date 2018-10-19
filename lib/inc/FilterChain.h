/*
 * Copyright 2018 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "IirFilter.h"
#include <limits>
#include <memory>
#include <stdint.h>
#include <vector>

class FilterChain {
private:
    struct Stage {
        IirFilter filter;
        uint8_t interval;
    };
    std::vector<Stage> stages;

    uint32_t counter = 0;

public:
    FilterChain(const std::vector<uint8_t>& params, const int32_t& stepThreshold = std::numeric_limits<int32_t>::max());
    FilterChain(const std::vector<uint8_t>& params, const std::vector<uint8_t>& intervals, const int32_t& stepThreshold = std::numeric_limits<int32_t>::max());
    ~FilterChain() = default;

    void add(const int32_t& val);
    void setParams(const std::vector<uint8_t>& params, const std::vector<uint8_t>& intervals, const int32_t& stepThreshold);
    void setStepThreshold(const int32_t& threshold);    // set the step detection threshold
    int32_t getStepThreshold() const;                   // get the step detection threshold of last filter
    int32_t read(uint8_t filterNr) const;               // read from specified filter
    int32_t read() const;                               // read from last filter
    uint32_t minSampleInterval(uint8_t filterNr) const; // get minimum sample interval of filter at index i
    uint32_t minSampleInterval() const;                 // get minimum sample interval of filter at last filter
    uint32_t getCount() const
    {
        return counter;
    }; // return count. Can be used to synchronize sensor switching
    uint32_t getCount(uint8_t filterNr) const
    {
        return counter / minSampleInterval(filterNr - 1);
    }; // return count for a specific filter
    uint8_t length() const
    {
        return stages.size();
    };
    uint8_t fractionBits(uint8_t idx) const;
    uint8_t fractionBits() const;
    int64_t readWithNFractionBits(uint8_t filterNr, uint8_t bits) const;
    int64_t readWithNFractionBits(uint8_t bits) const;
    int32_t readLastInput() const;
    IirFilter::DerivativeResult readDerivative(uint8_t filterIdx = 0) const;
    void reset(const int32_t& value);
};
