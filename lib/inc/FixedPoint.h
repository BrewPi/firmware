#include "../cnl/include/cnl/all.h"

using namespace cnl::literals;

template <
    int IntegerDigits,
    int FractionalDigits,
    class Narrowest>
using safe_elastic_fixed_point = cnl::fixed_point<
    cnl::overflow_integer<
        cnl::elastic_integer<
            IntegerDigits + FractionalDigits,
            Narrowest>,
        cnl::saturated_overflow_tag>,
    -FractionalDigits>;
