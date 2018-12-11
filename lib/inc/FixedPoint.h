#pragma once

#include "../cnl/include/cnl/_impl/fixed_point/constants.h"
#include "../cnl/include/cnl/_impl/fixed_point/convert.h"
#include "../cnl/include/cnl/_impl/fixed_point/declaration.h"
#include "../cnl/include/cnl/_impl/fixed_point/extras.h"
// #include "../cnl/include/cnl/_impl/fixed_point/fraction.h"
#include "../cnl/include/cnl/_impl/fixed_point/from_rep.h"
#include "../cnl/include/cnl/_impl/fixed_point/is_fixed_point.h"
#include "../cnl/include/cnl/_impl/fixed_point/named.h"
#include "../cnl/include/cnl/_impl/fixed_point/operators.h"
#include "../cnl/include/cnl/_impl/fixed_point/to_chars.h"
// #include "../cnl/include/cnl/_impl/fixed_point/to_string.h"
#include "../cnl/include/cnl/_impl/fixed_point/type.h"

#include "../cnl/include/cnl/elastic_integer.h"
#include "../cnl/include/cnl/num_traits.h"
#include "../cnl/include/cnl/overflow_integer.h"
#include "../cnl/include/cnl/rounding_integer.h"

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
