#pragma once

#include "FixedPoint.h"
#include <cstdint>
#include <type_traits>

using temp_t = safe_elastic_fixed_point<7, 8, int16_t>;
using temp_precise_t = safe_elastic_fixed_point<7, 23, int32_t>;
using temp_long_t = safe_elastic_fixed_point<23, 8, int32_t>;

constexpr int16_t
to_base(const temp_t& t)
{
    temp_t temp = t;
    int16_t raw = *reinterpret_cast<const int16_t*>(&temp);
    return raw;
}

constexpr temp_t
temp_t_from_base(const int16_t& r)
{
    int16_t raw = r;
    temp_t temp = *reinterpret_cast<const temp_t*>(&raw);
    return temp;
}
