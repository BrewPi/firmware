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

    /*
    FilterChain({0, 0}, {2, 1}, INT32_MAX, 0),                       // 28
        FilterChain({2, 0}, {4, 1}, INT32_MAX, 0),                   // 56
        FilterChain({2, 2, 0}, {4, 3, 1}, INT32_MAX, 0),             // 171
        FilterChain({2, 2, 2}, {4, 3, 1}, INT32_MAX, 0),             // 257
        FilterChain({2, 2, 2, 0}, {4, 4, 3, 1}, INT32_MAX, 0),       // 683
        FilterChain({2, 2, 2, 2}, {4, 4, 4, 1}, INT32_MAX, 0),       // 1343
        FilterChain({2, 2, 2, 2, 0}, {4, 4, 4, 3, 1}, INT32_MAX, 0), // 2729
    */
public:
    using value_type = T;

    FpFilterChain(const std::vector<uint8_t>& params)
        : chain(params)
    {
    }

    FpFilterChain(const std::vector<uint8_t>& params, const value_type& stepThreshold)
        : chain(params, cnl::unwrap(stepThreshold))
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
    value_type readLastInput()
    {
        return cnl::wrap<value_type>(chain.readLastInput());
    }

    // get the derivative from the chain with max precision and convert to the requested FP precision
    template <typename U>
    auto readDerivative()
    {
        auto derivative = chain.readDerivative();

        int64_t rawResult = (-U::exponent >= derivative.fractionBits) ? derivative.result << (-U::exponent - derivative.fractionBits) : derivative.result << (derivative.fractionBits + U::exponent);
        return U(rawResult);
    }

    void reset(const int32_t& value);
};
