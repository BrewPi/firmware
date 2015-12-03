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

#include "temperatureFormats.h"
#include <iostream>
#include <cstdio>
#include <boost/test/output_test_stream.hpp>
using boost::test_tools::output_test_stream;

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

BOOST_AUTO_TEST_CASE(unary_minus)
{
    temp_t a = 1.0;
    temp_long_t b = 2.0;
    temp_precise_t c = 3.0;

    temp_t d = -1.0;
    temp_long_t e = -2.0;
    temp_precise_t f = -3.0;

    BOOST_CHECK_EQUAL(a, -d);
    BOOST_CHECK_EQUAL(b, -e);
    BOOST_CHECK_EQUAL(c, -f);

    BOOST_CHECK_EQUAL(a, -(-a));
}

BOOST_AUTO_TEST_CASE(multiplication_by_uint16){
    temp_t a = 2.0;
    uint16_t b = 5;
    temp_long_t c = a*b;

    BOOST_CHECK_EQUAL(c, temp_long_t(10.0));

    BOOST_CHECK_EQUAL(temp_t(127.0) * uint16_t(UINT16_MAX), temp_long_t(127.0*65535));

    temp_precise_t d = 2.0;
    temp_long_t e = d*b;

    BOOST_CHECK_EQUAL(e, temp_long_t(10.0));

    BOOST_CHECK_EQUAL(temp_precise_t(127.0) * uint16_t(UINT16_MAX), temp_long_t(127.0*65535));

    // check multiplying very low values, in lower precision bits of temp_precise
    // values below are choosen to be able to be stored without loss of precision
    BOOST_CHECK_EQUAL(temp_precise_t(0.0078125) * uint16_t(10000), temp_long_t(78.125)); // lower bits of temp_precise_t are not lost

    // check multiplying high precision values, but with upper bits not zero.
    BOOST_CHECK_EQUAL(temp_precise_t(0.5078125) * uint16_t(100), temp_long_t(50.78125));

    // conversion between temp_precise and temp_long rounds down, due to the non rounded arithmetic shift.
    // That's why the result below is 0.933594 instead of 0.9375
    BOOST_CHECK_EQUAL(temp_precise_t(0.0156109333) * uint16_t(60), temp_long_t(0.933594));
}


BOOST_AUTO_TEST_CASE(right_shift)
{
    temp_t t = 2.0;
    unsigned char a = 1;
    temp_t t2 = t>>a;
    temp_t t3 = t>>uint8_t(1); //cast needed to prevent ambiguity

    BOOST_REQUIRE_EQUAL(double(t2), 1.0);
    BOOST_REQUIRE_EQUAL(double(t3), 1.0);
}

BOOST_AUTO_TEST_CASE(sign_function)
{
    BOOST_CHECK_EQUAL(temp_t(0.0).sign(), 0);
    BOOST_CHECK_EQUAL(temp_t(2.0).sign(), 1);
    BOOST_CHECK_EQUAL(temp_t(-2.0).sign(), -1);
    BOOST_CHECK_EQUAL(temp_precise_t(0.0).sign(), 0);
    BOOST_CHECK_EQUAL(temp_precise_t(2.0).sign(), 1);
    BOOST_CHECK_EQUAL(temp_precise_t(-2.0).sign(), -1);
    BOOST_CHECK_EQUAL(temp_long_t(0.0).sign(), 0);
    BOOST_CHECK_EQUAL(temp_long_t(2.0).sign(), 1);
    BOOST_CHECK_EQUAL(temp_long_t(-2.0).sign(), -1);
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

BOOST_AUTO_TEST_CASE(temp_conversion_to_and_from_fixed_length_string_in_F_absolute){
    for(double d = -127; d < 127; d += 0.1){
        temp_t t = d;
        char s1[12];
        char s2[12];

        // use 2 decimals here. Internal format is not accurate to 4, so returned format has some precision errors
        t.toTempString(s1, 2, 10, 'F', true);
        snprintf(s2, 12, "%9.02f", double(t)*9.0/5.0 + 32.0);
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\" in F " << " converting " << t);

        // use 4 decimals here, to ensure that input accuracy is higher than internal format
        t.toTempString(s1, 4, 10, 'F', true);
        snprintf(s2, 12, "%9.04f", double(t)*9.0/5.0 + 32.0);
        temp_t t2;
        BOOST_REQUIRE_MESSAGE(t2.fromTempString(s1, 'F', true), "Could not parse \"" << s1 << "\" in F to temp");
        BOOST_REQUIRE_EQUAL(t,t2);
    }

    temp_t t = 1.0;
    char const * s = "  266.0";
    BOOST_REQUIRE_MESSAGE(t.fromString((char *) s) == false, "Value outside limits should not be accepted");
    BOOST_REQUIRE_MESSAGE(t == temp_t(1.0), "and value should remain unchanged");

    char const * s2 = " -202.0";
    BOOST_REQUIRE_MESSAGE(t.fromString((char *) s2) == false, "Value outside limits should not be accepted");
    BOOST_REQUIRE_MESSAGE(t == temp_t(1.0), "and value should remain unchanged");
}

BOOST_AUTO_TEST_CASE(temp_conversion_to_and_from_fixed_length_string_in_F_relative){
    for(double d = -127; d < 127; d += 0.1){
        temp_t t = d;
        char s1[12];
        char s2[12];

        // use 2 decimals here. Internal format is not accurate to 4, so returned format has some precision errors
        t.toTempString(s1, 2, 10, 'F', false);
        snprintf(s2, 12, "%9.02f", double(t)*9.0/5.0);
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\" in F relative" << " converting " << t);

        // use 4 decimals here, to ensure that input accuracy is higher than internal format
        t.toTempString(s1, 4, 10, 'F', false);
        temp_t t2;
        BOOST_REQUIRE_MESSAGE(t2.fromTempString(s1, 'F', false), "Could not parse \"" << s1 << "\" in F relative to temp");
        BOOST_REQUIRE_EQUAL(t,t2);
    }

    temp_t t = 1.0;
    char const * s = "  266.0";
    BOOST_REQUIRE_MESSAGE(t.fromString((char *) s) == false, "Value outside limits should not be accepted");
    BOOST_REQUIRE_MESSAGE(t == temp_t(1.0), "and value should remain unchanged");

    char const * s2 = " -202.0";
    BOOST_REQUIRE_MESSAGE(t.fromString((char *) s2) == false, "Value outside limits should not be accepted");
    BOOST_REQUIRE_MESSAGE(t == temp_t(1.0), "and value should remain unchanged");
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

BOOST_AUTO_TEST_CASE(temp_long_conversion_to_and_from_fixed_length_string_in_F_absolute){
    for(double d = -200; d < 200; d += 0.1){
        temp_long_t t = d;
        char s1[12];
        char s2[12];

        // use 2 decimals here. Internal format is not accurate to 4, so returned format has some precision errors
        t.toTempString(s1, 2, 10, 'F', true);
        snprintf(s2, 12, "%9.02f", double(t)*9.0/5.0 + 32.0);
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\" in F " << " converting " << t);

        // use 4 decimals here, to ensure that input accuracy is higher than internal format
        t.toTempString(s1, 4, 12, 'F', true);
        temp_long_t t2;
        BOOST_REQUIRE_MESSAGE(t2.fromTempString(s1, 'F', true), "Could not parse \"" << s1 << "\" in F to temp_long_t");
        BOOST_REQUIRE_EQUAL(t,t2);
    }
}

BOOST_AUTO_TEST_CASE(temp_long_conversion_to_and_from_fixed_length_string_in_F_relative){
    for(double d = -200; d < 200; d += 0.1){
        temp_long_t t = d;
        char s1[12];
        char s2[12];

        // use 2 decimals here. Internal format is not accurate to 4, so returned format has some precision errors
        t.toTempString(s1, 2, 10, 'F', false);
        snprintf(s2, 12, "%9.02f", double(t)*9.0/5.0);
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\" in F relative" << " converting " << t);

        // use 4 decimals here, to ensure that input accuracy is higher than internal format
        t.toTempString(s1, 4, 12, 'F', false);
        temp_long_t t2;
        BOOST_REQUIRE_MESSAGE(t2.fromTempString(s1, 'F', false), "Could not parse \"" << s1 << "\" in F relative to temp_long_t");
        BOOST_REQUIRE_EQUAL(t,t2);
    }
}

BOOST_AUTO_TEST_CASE(temp_precise_conversion_to_and_from_fixed_length_string){
    for(double d = -127; d < 127; d += 0.1){
        temp_precise_t t = d;
        char s1[12];
        char s2[12];
        t.toString(s1, 5, 11);
        snprintf(s2, 12, "%10.05f", double(t));
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


BOOST_AUTO_TEST_CASE(temp_precise_conversion_to_and_from_fixed_length_string_in_F_absolute){
    for(double d = -127; d < 127; d += 0.1){
        temp_precise_t t = d;
        char s1[12];
        char s2[12];

        t.toTempString(s1, 5, 11, 'F', true);
        snprintf(s2, 12, "%10.05f", double(t)*9.0/5.0 + 32.0);
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\" in F " << " converting " << t);

        temp_precise_t t2;
        BOOST_REQUIRE_MESSAGE(t2.fromTempString(s1, 'F', true), "Could not parse \"" << s1 << "\" in F to temp_precise_t");
        BOOST_REQUIRE_CLOSE(double(t),double(t2), 0.001); // check close to 0.001%, because input is not as precise as internal format
    }
}

BOOST_AUTO_TEST_CASE(temp_precise_conversion_to_and_from_fixed_length_string_in_F_relative){
    for(double d = -127; d < 127; d += 0.1){
        temp_precise_t t = d;
        char s1[12];
        char s2[12];

        t.toTempString(s1, 5, 11, 'F', false);
        snprintf(s2, 12, "%10.05f", double(t)*9.0/5.0);
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\" in F relative" << " converting " << t);

        temp_precise_t t2;
        BOOST_REQUIRE_MESSAGE(t2.fromTempString(s1, 'F', false), "Could not parse \"" << s1 << "\" in F relative to temp_precise_t");
        BOOST_REQUIRE_CLOSE(double(t),double(t2), 0.001); // check close to 0.001%, because input is not as precise as internal format
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


