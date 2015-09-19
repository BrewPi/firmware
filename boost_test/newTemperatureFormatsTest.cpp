/*
 * Copyright 2015 BrewPi/Elco Jacobs.
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

#include <boost/test/unit_test.hpp>

#include "newTemperatureFormats.h"
#include <cstdio>

BOOST_AUTO_TEST_SUITE( temperature_suite )


BOOST_AUTO_TEST_CASE( assignment )
{
    temp_t a = 5.0;
    temp_t b = a;
    BOOST_CHECK_EQUAL(a, b);


    temp_long_t c = 5.0;
    temp_long_t d = c;
    BOOST_CHECK_EQUAL(c, d);

    temp_precise_t e = 5.0;
    temp_precise_t f = e;
    BOOST_CHECK_EQUAL(e, f);
}


BOOST_AUTO_TEST_CASE( conversion_from_normal_to_long)
{
    // normal variable to convert from
    temp_t t0 = 1.0;

    // conversion to longer format
    temp_long_t tl0 = 1.0;
    temp_long_t tl1 = t0;

    BOOST_CHECK_EQUAL(tl0, tl1);
}

BOOST_AUTO_TEST_CASE( conversion_from_long_to_normal_temp)
{
    // non-clipping/non-overflowing conversion
    temp_long_t tl = 0.5;

    // conversion to normal format
    temp_t t = tl;
    BOOST_CHECK_EQUAL(t, temp_t(0.5));

    // clipping conversion should be constrained to min/max
    tl = -150;
    t = tl;
    BOOST_CHECK_EQUAL(t, temp_t::min());

    tl = 150;
    t = tl;
    BOOST_CHECK_EQUAL(t, temp_t::max());
}

BOOST_AUTO_TEST_CASE( conversion_from_normal_to_precise)
{
    // normal variable to convert from
    temp_t t0 = 1.0;

    // conversion to more fraction bits
    temp_precise_t tp0 = 1.0;
    temp_precise_t tp1 = t0;

    BOOST_CHECK_EQUAL(tp0, tp1);
}

BOOST_AUTO_TEST_CASE( conversion_from_precise_to_normal_temp)
{
    // non-clipping/non-overflowing conversion
    temp_precise_t tp = 0.5;

    // conversion to normal format
    temp_t t = tp;
    BOOST_CHECK_EQUAL(t, temp_t(0.5));
}


BOOST_AUTO_TEST_CASE( conversion_from_small_to_normal_temp)
{
    // normal variable to convert from
    temp_small ts0 = 0.5;
    temp_t t0 = 0.5;

    // conversion to normal format
    temp_t t1 = ts0;

    BOOST_CHECK_EQUAL(t0, t1);
}


BOOST_AUTO_TEST_CASE(conversion_to_double)
{
    BOOST_CHECK_CLOSE(1.0, double(temp_t(1)), 1);
    BOOST_CHECK_CLOSE(1.0, double(temp_precise_t(1)), 1);
    BOOST_CHECK_CLOSE(1.0, double(temp_long_t(1)), 1);
}

BOOST_AUTO_TEST_CASE(test_min_max)
{
    BOOST_CHECK_CLOSE(double(-128), double(temp_t::min()), 1);
    BOOST_CHECK(double(-128) < double(temp_t::min())); // there are reserved values for invalid/disabled

    BOOST_CHECK_CLOSE(double( 128), double(temp_t::max()), 1);

    BOOST_CHECK_CLOSE(double(-128), double(temp_precise_t::min()), 1);
    BOOST_CHECK_CLOSE(double( 128), double(temp_precise_t::max()), 1);

    BOOST_CHECK_CLOSE(-pow(2,23), double(temp_long_t::min()), 1);
    BOOST_CHECK_CLOSE( pow(2,23), double(temp_long_t::max()), 1);
}

BOOST_AUTO_TEST_CASE(multiplication)
{
    temp_t t1 = 3.0;
    temp_t t2 = 3.0;

    temp_t t3 = t1 * t2;

    BOOST_CHECK_EQUAL(t3, temp_t(9.0));
}


BOOST_AUTO_TEST_CASE(overflowing_multiplication_is_constrained)
{
    temp_t t1 = 120;
    temp_t t2 = 120;

    BOOST_REQUIRE_EQUAL(t1*t2, temp_t::max());

    t1 = -120;
    t2 = 120;

    // only check close, min is 2 epsilon higher to reserve place for invalid and disabled temp
    // multiplication in base fixed point class does not take this +2 offset into account
    BOOST_REQUIRE_CLOSE(double(t1*t2), double(temp_t::min()),1);
}

BOOST_AUTO_TEST_CASE(multiplication_of_temp_and_temp_precise)
{
    temp_t t1 = 3.0;
    temp_precise_t t2 = 3.0;

    temp_t t3 = t1 * t2;
    temp_t t4 = t2 * t1;
    temp_precise_t t5 = t1 * t2;

    BOOST_CHECK_EQUAL(t3, temp_t(9.0));
    BOOST_CHECK_EQUAL(t4, temp_t(9.0));
    BOOST_CHECK_EQUAL(t5, temp_precise_t(9.0));
}

BOOST_AUTO_TEST_CASE(multiplication_of_temp_and_temp_long)
{
    temp_t t1 = 3.0;
    temp_long_t t2 = 3.0;
    temp_long_t t3 = 100.0;

    temp_t t4 = t1 * t2;
    temp_long_t t5 = t2 * t1;

    temp_t t6 = t1 * t3; // should be constrained to temp max
    temp_long_t t7 = t1 * t3; // should fit in t7

    BOOST_CHECK_EQUAL(t4, temp_t(9.0));
    BOOST_CHECK_EQUAL(t5, temp_long_t(9.0));
    BOOST_CHECK_EQUAL(t6, temp_t::max());
    BOOST_CHECK_EQUAL(t7, temp_long_t(300.0));
}

BOOST_AUTO_TEST_CASE(multiplication_of_temp_precise_and_temp_long)
{
    temp_precise_t t1 = 3.0;
    temp_long_t t2 = 3.0;
    temp_long_t t3 = 100.0;

    temp_precise_t t4 = t1 * t2;
    temp_long_t t5 = t2 * t1;

    temp_precise_t t6 = t1 * t3; // should be constrained to temp_precise max
    temp_long_t t7 = t1 * t3; // should fit in t7

    BOOST_CHECK_EQUAL(t4, temp_precise_t(9.0));
    BOOST_CHECK_EQUAL(t5, temp_long_t(9.0));
    // next check has lost precision in lower bits due to internal conversion to long before multiplication
    // use CHECK_CLOSE to see if it is within 0.1% instead of strict equality
    BOOST_CHECK_CLOSE(double(t6), double(temp_precise_t::max()), 0.1);
    BOOST_CHECK_EQUAL(t7, temp_long_t(300.0));
}


BOOST_AUTO_TEST_CASE(addition)
{
    temp_t t1 = 3.0;
    temp_t t2 = 3.0;

    temp_t t3 = t1 + t2;

    BOOST_CHECK_EQUAL(t3, temp_t(6.0));

    temp_t t4 = t1 + t2 + t3;

    BOOST_CHECK_EQUAL(t4, temp_t(12.0));
}


BOOST_AUTO_TEST_CASE(overflowing_addition_is_constrained)
{
    temp_t t1 = 120;
    temp_t t2 = 120;

    BOOST_REQUIRE_EQUAL(t1+t2, temp_t::max());

    t1 = -120;
    t2 = -120;

    // only check close, min is 2 epsilon higher to reserve place for invalid and disabled temp
    // multiplication in base fixed point class does not take this +2 offset into account
    BOOST_REQUIRE_CLOSE(double(t1+t2), double(temp_t::min()),1);
}

BOOST_AUTO_TEST_CASE(addition_of_temp_and_temp_precise)
{
    temp_t t1 = 3.0;
    temp_precise_t t2 = 3.0;

    temp_t t3 = t1 + t2;
    temp_t t4 = t2 + t1;
    temp_precise_t t5 = t1 + t2;

    BOOST_CHECK_EQUAL(t3, temp_t(6.0));
    BOOST_CHECK_EQUAL(t4, temp_t(6.0));
    BOOST_CHECK_EQUAL(t5, temp_precise_t(6.0));
}

BOOST_AUTO_TEST_CASE(addition_of_temp_and_temp_long)
{
    temp_t t1 = 30.0;
    temp_long_t t2 = 30.0;
    temp_long_t t3 = 200.0;

    temp_t t4 = t1 + t2;
    temp_long_t t5 = t2 + t1;

    temp_t t6 = t1 + t3; // should be constrained to temp max
    temp_long_t t7 = t1 + t3; // should fit in t7

    BOOST_CHECK_EQUAL(t4, temp_t(60.0));
    BOOST_CHECK_EQUAL(t5, temp_long_t(60.0));
    BOOST_CHECK_EQUAL(t6, temp_t::max());
    BOOST_CHECK_EQUAL(t7, temp_long_t(230.0));
}

BOOST_AUTO_TEST_CASE(addition_of_temp_precise_and_temp_long)
{
    temp_precise_t t1 = 30.0;
    temp_long_t t2 = 30.0;
    temp_long_t t3 = 100.0;

    temp_precise_t t4 = t1 + t2;
    temp_long_t t5 = t2 + t1;

    temp_precise_t t6 = t1 + t3; // should be constrained to temp_precise max
    temp_long_t t7 = t1 + t3; // should fit in t7

    BOOST_CHECK_EQUAL(t4, temp_precise_t(60.0));
    BOOST_CHECK_EQUAL(t5, temp_long_t(60.0));
    // next check has lost precision in lower bits due to internal conversion to long before addition
    // use CHECK_CLOSE to see if it is within 0.1% instead of strict equality
    BOOST_CHECK_CLOSE(double(t6), double(temp_precise_t::max()), 0.1);
    BOOST_CHECK_EQUAL(t7, temp_long_t(130.0));
}



BOOST_AUTO_TEST_CASE(subtraction)
{
    temp_t t1 = 9.0;
    temp_t t2 = 3.0;

    temp_t t3 = t1 - t2;

    BOOST_CHECK_EQUAL(t3, temp_t(6.0));

    temp_t t4 = t1 - t2 - t3;

    BOOST_CHECK_EQUAL(t4, temp_t(0.0));

    temp_t t5 = t2 - t1;

    BOOST_CHECK_EQUAL(t5, temp_t(-6.0));
}


BOOST_AUTO_TEST_CASE(overflowing_subtraction_is_constrained)
{
    temp_t t1 = 120;
    temp_t t2 = -120;

    BOOST_REQUIRE_EQUAL(t1-t2, temp_t::max());

    t1 = -120;
    t2 = 120;

    // only check close, min is 2 epsilon higher to reserve place for invalid and disabled temp
    // multiplication in base fixed point class does not take this +2 offset into account
    BOOST_REQUIRE_CLOSE(double(t1-t2), double(temp_t::min()),1);
}

BOOST_AUTO_TEST_CASE(subtraction_of_temp_and_temp_precise)
{
    temp_t t1 = 9.0;
    temp_precise_t t2 = 3.0;

    temp_t t3 = t1 - t2;
    temp_t t4 = t2 - t1;
    temp_precise_t t5 = t1 - t2;

    BOOST_CHECK_EQUAL(t3, temp_t(6.0));
    BOOST_CHECK_EQUAL(t4, temp_t(-6.0));
    BOOST_CHECK_EQUAL(t5, temp_precise_t(6.0));
}

BOOST_AUTO_TEST_CASE(subtraction_of_temp_and_temp_long)
{
    temp_t t1 = 90.0;
    temp_long_t t2 = 30.0;
    temp_long_t t3 = -200.0;

    temp_t t4 = t1 - t2;
    temp_long_t t5 = t2 - t1;

    temp_t t6 = t1 - t3; // should be constrained to temp max
    temp_long_t t7 = t1 - t3; // should fit in t7

    BOOST_CHECK_EQUAL(t4, temp_t(60.0));
    BOOST_CHECK_EQUAL(t5, temp_long_t(-60.0));
    BOOST_CHECK_EQUAL(t6, temp_t::max());
    BOOST_CHECK_EQUAL(t7, temp_long_t(290.0));
}

BOOST_AUTO_TEST_CASE(subtraction_of_temp_precise_and_temp_long)
{
    temp_precise_t t1 = 90.0;
    temp_long_t t2 = 30.0;
    temp_long_t t3 = -200.0;

    temp_precise_t t4 = t1 - t2;
    temp_long_t t5 = t2 - t1;

    temp_precise_t t6 = t1 - t3; // should be constrained to temp_precise max
    temp_long_t t7 = t1 - t3; // should fit in t7

    BOOST_CHECK_EQUAL(t4, temp_precise_t(60.0));
    BOOST_CHECK_EQUAL(t5, temp_long_t(-60.0));
    // next check has lost precision in lower bits due to internal conversion to long before addition
    // use CHECK_CLOSE to see if it is within 0.1% instead of strict equality
    BOOST_CHECK_CLOSE(double(t6), double(temp_precise_t::max()), 0.1);
    BOOST_CHECK_EQUAL(t7, temp_long_t(290.0));
}



BOOST_AUTO_TEST_CASE(left_shift)
{
    temp_t t = 2.0;
    unsigned char a = 1;
    temp_t t2 = t>>a;
    temp_t t3 = t>>uint8_t(1); //cast needed to prevent ambiguity

    BOOST_REQUIRE_EQUAL(double(t2), 1.0);
    BOOST_REQUIRE_EQUAL(double(t3), 1.0);
}


BOOST_AUTO_TEST_CASE(temp_conversion_to_and_from_fixed_length_string){
    for(double d = -127; d < 127; d += 0.1){
        temp_t t = d;
        char s1[12];
        char s2[12];
        t.toString(s1, 4, 10);
        snprintf(s2, 12, "%9.04f", double(t));
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);

        temp_t t2;
        BOOST_REQUIRE_MESSAGE(t2.fromString(s1), "Could not parse \"" << s1 << "\" to temp");
        BOOST_REQUIRE_EQUAL(t,t2);
    }

    temp_t t = 1.0;
    char const * s = "  130.0";
    BOOST_REQUIRE_MESSAGE(t.fromString((char *) s) == false, "Value outside limits should not be accepted");
    BOOST_REQUIRE_MESSAGE(t == temp_t(1.0), "and value should remain unchanged");

    char const * s2 = " -130.0";
    BOOST_REQUIRE_MESSAGE(t.fromString((char *) s2) == false, "Value outside limits should not be accepted");
    BOOST_REQUIRE_MESSAGE(t == temp_t(1.0), "and value should remain unchanged");
}


BOOST_AUTO_TEST_CASE(temp_conversion_to_and_from_variable_length_string){
    for(double d = -127; d < 127; d += 0.1){
        temp_t t = d;
        char s1[12];
        char s2[12];
        char * s3 = t.toString(s1, 3, 12);
        snprintf(s2, 12, "%.03f", double(t));
        BOOST_REQUIRE_MESSAGE(strcmp(s3, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);

        temp_t t2;
        BOOST_REQUIRE_MESSAGE(t2.fromString(s3), "Could not parse \"" << s3 << "\" to temp");
        BOOST_REQUIRE_EQUAL(t,t2);
    }
}

BOOST_AUTO_TEST_CASE(temp_long_conversion_to_and_from_fixed_length_string){
    for(double d = -200; d < 200; d += 0.1){
        temp_long_t t = d;
        char s1[12];
        char s2[12];
        t.toString(s1, 3, 9);
        snprintf(s2, 12, "%8.03f", double(t));
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);

        temp_long_t t2;
        BOOST_REQUIRE_MESSAGE(t2.fromString(s1), "Could not parse \"" << s1 << "\" to long temp");
        BOOST_REQUIRE_EQUAL(t,t2);
    }
}

BOOST_AUTO_TEST_CASE(temp_long_conversion_to_and_from_variable_length_string){
    for(double d = -200; d < 200; d += 0.1){
        temp_long_t t = d;
        char s1[12];
        char s2[12];
        char * s3 = t.toString(s1, 4, 10);
        snprintf(s2, 12, "%.04f", double(t));
        BOOST_REQUIRE_MESSAGE(strcmp(s3, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);

        temp_long_t t2;
        BOOST_REQUIRE_MESSAGE(t2.fromString(s1), "Could not parse \"" << s1 << "\" to long temp");
        BOOST_REQUIRE_EQUAL(t,t2);
    }
}

BOOST_AUTO_TEST_CASE(temp_precise_conversion_to_and_from_fixed_length_string){
    for(double d = -127; d < 127; d += 0.1){
        temp_precise_t t = d;
        char s1[12];
        char s2[12];
        t.toString(s1, 3, 9);
        snprintf(s2, 12, "%8.03f", double(t));
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);

        temp_precise_t t2;
        BOOST_REQUIRE_MESSAGE(t2.fromString(s1), "Could not parse \"" << s1 << "\"to precise temp");
        BOOST_REQUIRE_EQUAL(t,t2);
    }
}

BOOST_AUTO_TEST_CASE(temp_precise_conversion_to_and_from_variable_length_string){
    for(double d = -127; d < 127; d += 0.1){
        temp_precise_t t = d;
        char s1[12];
        char s2[12];
        char * s3 = t.toString(s1, 5, 12);
        snprintf(s2, 12, "%.05f", double(t));
        BOOST_REQUIRE_MESSAGE(strcmp(s3, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);

        temp_precise_t t2;
        BOOST_REQUIRE_MESSAGE(t2.fromString(s1), "Could not parse \"" << s1 << "\"to precise temp");
        BOOST_REQUIRE_EQUAL(t,t2);
    }
}

BOOST_AUTO_TEST_CASE(temp_small_conversion_to_and_from_fixed_length_string){
    for(double d = -8; d < 8; d += 0.0625){
        temp_small t = d;
        char s1[12];
        char s2[12];
        t.toString(s1, 3, 9);
        snprintf(s2, 12, "%8.03f", round(double(t) * 1000.0) / 1000.0); // don't let snprintf do the rounding, it uses bankers rounding / round to even
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);

        temp_small t2;
        BOOST_REQUIRE_MESSAGE(t2.fromString(s1), "Could not parse \"" << s1 << "\"to small temp diff");
        BOOST_REQUIRE_EQUAL(t,t2);
    }
}

BOOST_AUTO_TEST_CASE(temp_small_conversion_to_and_from_variable_length_string){
    for(double d = -8; d < 8; d += 0.0625){
        temp_small t = d;
        char s1[12];
        char s2[12];
        char * s3 = t.toString(s1, 3, 9);
        snprintf(s2, 12, "%.03f", round(double(t) * 1000.0) / 1000.0); // don't let snprintf do the rounding, it uses bankers rounding / round to even
        BOOST_REQUIRE_MESSAGE(strcmp(s3, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);

        temp_small t2;
        BOOST_REQUIRE_MESSAGE(t2.fromString(s1), "Could not parse \"" << s1 << "\"to small temp diff");
        BOOST_REQUIRE_EQUAL(t,t2);
    }
}

BOOST_AUTO_TEST_CASE(temp_disabled_or_invalid_prints_null){
    temp_t t = temp_t::disabled();
    char s1[10];
    const char * s2 = "null";
    t.toString(s1, 3, 9);
    BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);

    t = temp_t::invalid();
    t.toString(s1, 3, 9);
    BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);
}

BOOST_AUTO_TEST_SUITE_END()


