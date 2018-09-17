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
#include "FilterChain.h"
#include "FixedPoint.h"
#include <type_traits>

template <typename T>
class FpFilterChain {
private:
    FilterChain chain;

    struct FilterSpec {
        const std::vector<uint8_t> paramIdxs;
        const std::vector<uint8_t> intervals;
    };

    static FilterChain predefinedFilter(const uint8_t& specIdx)
    {
        static const FilterSpec predefinedFilters[] = {
            {{0, 0}, {2, 1}},                   // 28
            {{2, 0}, {4, 1}},                   // 56
            {{2, 2, 0}, {4, 3, 1}},             // 171
            {{2, 2, 2}, {4, 3, 1}},             // 257
            {{2, 2, 2, 0}, {4, 4, 3, 1}},       // 683
            {{2, 2, 2, 2}, {4, 4, 4, 1}},       // 1343
            {{2, 2, 2, 2, 0}, {4, 4, 4, 3, 1}}, // 2729
        };
        auto idx = (specIdx < 7) ? specIdx : 0;
        return FilterChain(predefinedFilters(idx).paramIdxs, predefinedFilters(idx).intervals);
    }

public:
    using value_type = T;

    FpFilterChain(const uint8_t& specIdx)
        : chain(predefinedFilter(specIdx))
    {
    }

    FpFilterChain(const std::vector<uint8_t>& params)
        : chain(params)
    {
    }

    FpFilterChain(const std::vector<uint8_t>& params, const value_type& stepThreshold)
        : chain(params, cnl::unwrap(stepThreshold))
    {
    }

    FpFilterChain(const std::vector<uint8_t>& params, const std::vector<uint8_t>& intervals)
        : chain(params, intervals)
    {
    }

    FpFilterChain(const std::vector<uint8_t>& params, const std::vector<uint8_t>& intervals, const value_type& stepThreshold)
        : chain(params, intervals, cnl::unwrap(stepThreshold))
    {
    }

    ~FpFilterChain<value_type>() = default;

    void add(const value_type& val)
    {
        chain.add(cnl::unwrap(val));
    }
    void add(const int32_t& val);
    void setParams(const std::vector<uint8_t>& params, const std::vector<uint8_t>& intervals, const value_type& stepThreshold)
    {
        chain.setParams(params, intervals, cnl::unwrap(stepThreshold));
    }
    void setStepThreshold(const value_type& stepThreshold)
    {
        chain.setStepThreshold(cnl::unwrap(stepThreshold));
    }
    value_type getStepThreshold()
    {
        return cnl::wrap<value_type>(chain.getStepThreshold());
    }
    value_type read()
    {
        return cnl::wrap<value_type>(chain.read());
    }

    value_type read(uint8_t filterNr) const
    {
        return cnl::wrap<value_type>(chain.read(filterNr));
    }

    value_type readLastInput()
    {
        return cnl::wrap<value_type>(chain.readLastInput());
    }

    uint8_t length() const
    {
        return chain.length();
    };

    // get the derivative from the chain with max precision and convert to the requested FP precision
    template <typename U>
    U readDerivative(uint8_t filterIdx = 0)
    {
        auto derivative = chain.readDerivative(filterIdx);
        uint8_t destFractionBits = -U::exponent;
        uint8_t filterFactionBits = -T::exponent + derivative.fractionBits;
        int64_t result;
        if (destFractionBits >= filterFactionBits) {
            result = derivative.result << (destFractionBits - filterFactionBits);
        } else {
            result = derivative.result >> (filterFactionBits - destFractionBits);
        }
        return cnl::wrap<U>(result);
    }

    void reset(const value_type& value)
    {
        chain.reset(cnl::unwrap(value));
    }
};
