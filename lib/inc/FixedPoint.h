#pragma once

#include "../cnl/include/cnl/elastic_integer.h"
#include "../cnl/include/cnl/fixed_point.h"
#include "../cnl/include/cnl/num_traits.h"
#include "../cnl/include/cnl/overflow_integer.h"
#include "../cnl/include/cnl/rounding_integer.h"

using namespace cnl::literals;

template <uint8_t I, uint8_t F, class Narrowest>
using elastic_fixed_point = cnl::fixed_point<
    cnl::elastic_integer<
        I + F,
        Narrowest>,
    -F>;

template <uint8_t I, uint8_t F, class Narrowest>
using safe_elastic_fixed_point = cnl::fixed_point<
    cnl::overflow_integer<
        cnl::elastic_integer<
            I + F,
            Narrowest>,
        cnl::saturated_overflow_tag>,
    -F>;
