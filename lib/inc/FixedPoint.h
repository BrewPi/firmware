#pragma once

#include <../cnl/include/cnl/elastic_integer.h>
#include <../cnl/include/cnl/fixed_point.h>
#include <../cnl/include/cnl/overflow_integer.h>
#include <../cnl/include/cnl/rounding_integer.h>

using namespace cnl::literals;

template <
    int IntegerDigits,
    int FractionalDigits,
    class Narrowest>
using safe_elastic_fixed_point = cnl::fixed_point<
    //cnl::rounding_integer<
    cnl::overflow_integer<
        cnl::elastic_integer<
            IntegerDigits + FractionalDigits,
            Narrowest>,
        cnl::saturated_overflow_tag>,
    //    cnl::nearest_rounding_tag>,
    -FractionalDigits>;
