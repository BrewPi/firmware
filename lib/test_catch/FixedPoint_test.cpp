/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of the BrewBlox Control Library.
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

#include <catch.hpp>

#include "FixedPoint.h"
#include <boost/core/demangle.hpp>
#include <cstdint>
#include <iomanip>
#include <type_traits>

using temp_t = safe_elastic_fixed_point<7, 8, int16_t>;
using temp_precise_t = safe_elastic_fixed_point<7, 23, int32_t>;
using temp_wide_t = safe_elastic_fixed_point<23, 8, int32_t>;

SCENARIO("CNL fixed point formats", "[fixedpoint]")
{
    WHEN("assignment")
    {
        temp_t a = 5.0;
        temp_t b = a;
        CHECK(a == b);

        temp_wide_t c = 5.0;
        temp_wide_t d = c;
        CHECK(c == d);

        temp_precise_t e = 5.0;
        temp_precise_t f = e;
        CHECK(e == f);
    }

    WHEN("Size is as small as a normal raw integer")
    {
        CHECK(sizeof(temp_t) == 2);
        CHECK(sizeof(temp_precise_t) == 4);
        CHECK(sizeof(temp_wide_t) == 4);
    }

    WHEN("conversion between different precisions")
    {
        // normal variable to convert from
        temp_t t0 = 1.0;

        // conversion to longer format
        temp_wide_t tl0 = 1.0;
        temp_wide_t tl1 = t0;

        CHECK(tl0 == tl1);
    }

    WHEN("conversion_from_long_to_normal_temp")
    {
        // non-clipping/non-overflowing conversion
        temp_wide_t tl = 0.5;

        // conversion to normal format
        temp_t t = tl;
        CHECK(t == temp_t(0.5));

        // clipping conversion should be constrained to min/max
        tl = -150;
        t = tl;
        CHECK(t == cnl::numeric_limits<temp_t>::lowest());

        tl = 150;
        t = tl;
        CHECK(t == cnl::numeric_limits<temp_t>::max());
    }
    WHEN("conversion_from_normal_to_precise")
    {
        // normal variable to convert from
        temp_t t0 = 1.0;

        // conversion to more fraction bits
        temp_precise_t tp0 = 1.0;
        temp_precise_t tp1 = t0;

        CHECK(tp0 == tp1);
    }

    WHEN("conversion_from_precise_to_normal_temp")
    {
        // non-clipping/non-overflowing conversion
        temp_precise_t tp = 0.5;

        // conversion to normal format
        temp_t t = tp;
        CHECK(t == temp_t(0.5));
    }

    WHEN("conversion_to_double")
    {
        CHECK(Approx(1.0) == double(temp_t(short(1))));
        CHECK(Approx(1.0) == double(temp_precise_t(1)));
        CHECK(Approx(1.0) == double(temp_wide_t(1)));
    }

    WHEN("test_min_max")
    {
        CHECK(Approx(-128.0).margin(0.01) == double(cnl::numeric_limits<temp_t>::lowest()));
        CHECK(-128.0 < double(cnl::numeric_limits<temp_t>::max()));

        CHECK(Approx(128.0).margin(0.01) == double(cnl::numeric_limits<temp_t>::max()));

        CHECK(Approx(-128.0) == double(cnl::numeric_limits<temp_precise_t>::lowest()));
        CHECK(Approx(128.0) == double(cnl::numeric_limits<temp_precise_t>::max()));

        CHECK(Approx(0.0 - pow(2, 23)) == double(cnl::numeric_limits<temp_wide_t>::lowest()));
        CHECK(Approx(pow(2, 23)) == double(cnl::numeric_limits<temp_wide_t>::max()));
    }
    WHEN("multiplication")
    {
        temp_t t1 = 3.0;
        temp_t t2 = 3.0;

        temp_t t3 = t1 * t2;

        CHECK(t3 == temp_t(9.0));
    }

    WHEN("overflowing_multiplication_is_constrained, but only after assignment to a smaller type")
    {
        temp_t t1 = short(120);
        temp_t t2 = short(120);

        auto t3 = t1 * t2;
        temp_t t4 = t3;
        CHECK(t3 == 14400);
        CHECK(t4 == cnl::numeric_limits<temp_t>::max());

        t1 = short(-120);
        t2 = short(120);
        auto t5 = t1 * t2;
        temp_t t6 = t5;

        CHECK(t5 == -14400);
        CHECK(t6 == cnl::numeric_limits<temp_t>::lowest());
    }

    WHEN("multiplication_of_temp_and_temp_precise")
    {
        temp_t t1 = 3.0;
        temp_precise_t t2 = 3.0;

        temp_t t3 = t1 * t2;
        temp_t t4 = t2 * t1;
        temp_precise_t t5 = t1 * t2;

        CHECK(t3 == temp_t(9.0));
        CHECK(t4 == temp_t(9.0));
        CHECK(t5 == temp_precise_t(9.0));
    }

    WHEN("multiplication_of_temp_and_temp_long")
    {
        temp_t t1 = 3.0;
        temp_wide_t t2 = 3.0;
        temp_wide_t t3 = 100.0;

        temp_t t4 = t1 * t2;
        temp_wide_t t5 = t2 * t1;

        temp_t t6 = t1 * t3;      // should be constrained to temp max
        temp_wide_t t7 = t1 * t3; // should fit in t7

        CHECK(t4 == temp_t(9.0));
        CHECK(t5 == temp_wide_t(9.0));
        CHECK(t6 == cnl::numeric_limits<temp_t>::max());
        CHECK(t7 == temp_wide_t(300.0));
    }

    WHEN("multiplication_of_temp_precise_and_temp_long")
    {
        temp_precise_t t1 = 3.0;
        temp_wide_t t2 = 3.0;
        temp_wide_t t3 = 100.0;

        temp_precise_t t4 = t1 * t2;
        temp_wide_t t5 = t2 * t1;

        temp_precise_t t6 = t1 * t3; // should be constrained to temp_precise max
        temp_wide_t t7 = t1 * t3;    // should fit in t7

        CHECK(t4 == temp_precise_t(9.0));
        CHECK(t5 == temp_wide_t(9.0));
        CHECK(t6 == cnl::numeric_limits<temp_precise_t>::max());
        CHECK(t7 == temp_wide_t(300.0));
    }

    WHEN("addition")
    {
        temp_t t1 = 3.0;
        temp_t t2 = 3.0;

        temp_t t3 = t1 + t2;

        CHECK(t3 == temp_t(6.0));

        temp_t t4 = t1 + t2 + t3;

        CHECK(t4 == temp_t(12.0));
    }

    WHEN("overflowing_addition_is_constrained")
    {
        temp_t t1 = short(120);
        temp_t t2 = short(120);

        REQUIRE(temp_t(t1 + t2) == cnl::numeric_limits<temp_t>::max());

        t1 = short(-120);
        t2 = short(-120);

        REQUIRE(temp_t(t1 + t2) == cnl::numeric_limits<temp_t>::lowest());
    }

    WHEN("addition_of_temp_and_temp_precise")
    {
        temp_t t1 = 3.0;
        temp_precise_t t2 = 3.0;

        temp_t t3 = t1 + t2;
        temp_t t4 = t2 + t1;
        temp_precise_t t5 = t1 + t2;

        CHECK(t3 == temp_t(6.0));
        CHECK(t4 == temp_t(6.0));
        CHECK(t5 == temp_precise_t(6.0));
    }

    WHEN("addition_of_temp_and_temp_long")
    {
        temp_t t1 = 30.0;
        temp_wide_t t2 = 30.0;
        temp_wide_t t3 = 200.0;

        temp_t t4 = t1 + t2;
        temp_wide_t t5 = t2 + t1;

        temp_t t6 = t1 + t3;      // should be constrained to temp max
        temp_wide_t t7 = t1 + t3; // should fit in t7

        CHECK(t4 == temp_t(60.0));
        CHECK(t5 == temp_wide_t(60.0));
        CHECK(t6 == cnl::numeric_limits<temp_t>::max());
        CHECK(t7 == temp_wide_t(230.0));
    }

    WHEN("addition_of_temp_precise_and_temp_long")
    {
        temp_precise_t t1 = 30.0;
        temp_wide_t t2 = 30.0;
        temp_wide_t t3 = 100.0;

        temp_precise_t t4 = t1 + t2;
        temp_wide_t t5 = t2 + t1;

        temp_precise_t t6 = t1 + t3; // should be constrained to temp_precise max
        temp_wide_t t7 = t1 + t3;    // should fit in t7

        CHECK(t4 == temp_precise_t(60.0));
        CHECK(t5 == temp_wide_t(60.0));
        CHECK(t6 == cnl::numeric_limits<temp_precise_t>::max());
        CHECK(t7 == temp_wide_t(130.0));
    }

    WHEN("subtraction")
    {
        temp_t t1 = 9.0;
        temp_t t2 = 3.0;

        temp_t t3 = t1 - t2;

        CHECK(t3 == temp_t(6.0));

        temp_t t4 = t1 - t2 - t3;

        CHECK(t4 == temp_t(0.0));

        temp_t t5 = t2 - t1;

        CHECK(t5 == temp_t(-6.0));
    }

    WHEN("overflowing_subtraction_is_constrained")
    {
        temp_t t1 = 120;
        temp_t t2 = -120;

        REQUIRE(temp_t(t1 - t2) == cnl::numeric_limits<temp_t>::max());

        REQUIRE(temp_t(t2 - t1) == cnl::numeric_limits<temp_t>::lowest());
    }

    WHEN("subtraction_of_temp_and_temp_precise")
    {
        temp_t t1 = 9.0;
        temp_precise_t t2 = 3.0;

        temp_t t3 = t1 - t2;
        temp_t t4 = t2 - t1;
        temp_precise_t t5 = t1 - t2;

        CHECK(t3 == temp_t(6.0));
        CHECK(t4 == temp_t(-6.0));
        CHECK(t5 == temp_precise_t(6.0));
    }

    WHEN("subtraction_of_temp_and_temp_long")
    {
        temp_t t1 = 90.0;
        temp_wide_t t2 = 30.0;
        temp_wide_t t3 = -200.0;

        temp_t t4 = t1 - t2;
        temp_wide_t t5 = t2 - t1;

        temp_t t6 = t1 - t3;      // should be constrained to temp max
        temp_wide_t t7 = t1 - t3; // should fit in t7

        CHECK(t4 == temp_t(60.0));
        CHECK(t5 == temp_wide_t(-60.0));
        CHECK(t6 == cnl::numeric_limits<temp_t>::max());
        CHECK(t7 == temp_wide_t(290.0));
    }

    WHEN("subtraction_of_temp_precise_and_temp_long")
    {
        temp_precise_t t1 = 90.0;
        temp_wide_t t2 = 30.0;
        temp_wide_t t3 = -200.0;

        temp_precise_t t4 = t1 - t2;
        temp_wide_t t5 = t2 - t1;

        temp_precise_t t6 = t1 - t3; // should be constrained to temp_precise max
        temp_wide_t t7 = t1 - t3;    // should fit in t7

        CHECK(t4 == temp_precise_t(60.0));
        CHECK(t5 == temp_wide_t(-60.0));
        CHECK(t6 == cnl::numeric_limits<temp_precise_t>::max());
        CHECK(t7 == temp_wide_t(290.0));
    }

    WHEN("unary_minus")
    {
        temp_t a = 1.0;
        temp_wide_t b = 2.0;
        temp_precise_t c = 3.0;

        temp_t d = -1.0;
        temp_wide_t e = -2.0;
        temp_precise_t f = -3.0;

        CHECK(a == -d);
        CHECK(b == -e);
        CHECK(c == -f);

        CHECK(a == -(-a));
    }

    WHEN("multiplication_by_uint16")
    {
        temp_t a = 2.0;
        uint16_t b = 5;
        temp_wide_t c = a * b;

        CHECK(c == temp_wide_t(10.0));

        CHECK(temp_t(127.0) * uint16_t(UINT16_MAX) == 127.0 * 65535);

        temp_precise_t d = 2.0;
        temp_wide_t e = d * b;

        CHECK(e == temp_wide_t(10.0));

        CHECK(temp_precise_t(127.0) * uint16_t(UINT16_MAX) == 127.0 * 65535);

        // check multiplying very low values, in lower precision bits of temp_precise
        // values below are choosen to be able to be stored without loss of precision
        CHECK(temp_precise_t(0.0078125) * uint16_t(10000) == 78.125); // lower bits of temp_precise_t are not lost

        // check multiplying high precision values, but with upper bits not zero.
        CHECK(temp_precise_t(0.5078125) * uint16_t(100) == 50.78125);

        // conversion between temp_precise and temp_long rounds down, due to the non rounded arithmetic shift.
        // That's why the result below is 0.933594 instead of 0.9375
        CHECK(temp_wide_t(temp_precise_t(0.0156109333) * uint16_t(60)) == Approx(0.933594));
    }

    WHEN("right_shift")
    {
        temp_t t = 8.0;
        temp_t t2 = t >> 1;
        temp_t t3 = t >> 3;

        REQUIRE(double(t2) == 4.0);
        REQUIRE(double(t3) == 1.0);
    }

    WHEN("from base")
    {
        auto t1 = cnl::wrap<temp_t>(256);
        CHECK(t1 == 1.0);
    }

    WHEN("to base")
    {
        auto t1 = cnl::wrap<temp_t>(256);
        CHECK(cnl::unwrap(t1) == 256);
    }

    WHEN("intermediate results not truncating")
    {
        auto t1 = temp_t(1);
        auto t2 = t1 * t1;
        //auto t3 = t2 * t1;
        //auto t4 = t3 * t1; // now t1^4
        auto t5 = (t1 * t1) * (t1 * t1);
        auto t4 = t2 * t2;

        CHECK(t5 == 1.0);
        CHECK(t4 == 1.0);

        //WARN(boost::core::demangle(typeid(t2).name()));
        //WARN(boost::core::demangle(typeid(t3).name()));
        //WARN(boost::core::demangle(typeid(t4).name()));
        //WARN(boost::core::demangle(typeid(t5).name()));
    }

    WHEN("temp_t is converted to string")
    {
        CHECK(to_string_dec(temp_t(10), 1) == "10.0");
        CHECK(to_string_dec(temp_t(10), 2) == "10.00");
        CHECK(to_string_dec(temp_t(9.99), 1) == "10.0");
        CHECK(to_string_dec(temp_t(9.96), 1) == "10.0");
        CHECK(to_string_dec(temp_t(10.01), 1) == "10.0");
        CHECK(to_string_dec(temp_t(10.06), 1) == "10.1");
        CHECK(to_string_dec(temp_t(10.01499), 2) == "10.01");
        CHECK(to_string_dec(temp_t(10.054), 2) == "10.05");

        CHECK(to_string_dec(temp_t(-10), 1) == "-10.0");
        CHECK(to_string_dec(temp_t(-10), 2) == "-10.00");
        CHECK(to_string_dec(temp_t(-9.99), 1) == "-10.0");
        CHECK(to_string_dec(temp_t(-9.96), 1) == "-10.0");
        CHECK(to_string_dec(temp_t(-10.01), 1) == "-10.0");
        CHECK(to_string_dec(temp_t(-10.06), 1) == "-10.1");
        CHECK(to_string_dec(temp_t(-10.01499), 2) == "-10.01");
        CHECK(to_string_dec(temp_t(-10.054), 2) == "-10.05");
        CHECK(to_string_dec(temp_t(0), 2) == "0.00");
        CHECK(to_string_dec(temp_t(0.01), 2) == "0.01");
        CHECK(to_string_dec(temp_t(-0.01), 2) == "-0.01");

        auto bit = cnl::wrap<temp_t>(1);
        for (auto t = temp_t(-11); t < temp_t(11); t += bit) {
            auto rounder = (t >= temp_t(0)) ? double(0.005) : -double(0.005);
            double d = double(t);
            if (d > -0.00499999 && d < 0.0) {
                d = 0.0; // we don't use -0.00 for
            } else {
                d += rounder;
            }
            auto s = std::to_string(d);

            INFO(d);
            INFO(t);
            s.erase(s.find('.') + 3, std::string::npos);
            REQUIRE(to_string_dec(t, 2) == s);
        }
    }
}
