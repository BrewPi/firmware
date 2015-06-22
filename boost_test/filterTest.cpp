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

#include "FilterCascaded.h"
#include <cstdio>

BOOST_AUTO_TEST_SUITE( filter_suite )

BOOST_AUTO_TEST_CASE( filter_init )
{
    FilterCascaded f;
    BOOST_REQUIRE_EQUAL(f.readOutput(), temp_precise(0.0));
}

long delayTime(FilterCascaded f){
    temp_precise target = 100.0;
    f.init(temp_precise(0.0));
    long addedSamples = 0;
    do{
        f.add(target);
        addedSamples++;
    } while(double(f.readOutput()) < 50.0);
    return addedSamples;
}

BOOST_AUTO_TEST_CASE( filter_delay_time )
{
    /* Cascaded(3x) filter delay times (tested with matlab)
     *       a=4,    b=0,    delay time = 9
     *       a=6,    b=1,    delay time = 20
     *       a=8,    b=2,    delay time = 43
     *       a=10,   b=3,    delay time = 88
     *       a=12,   b=4,    delay time = 179
     *       a=14,   b=5,    delay time = 360
     *       a=16,   b=6,    delay time = 723
     */

    FilterCascaded f; // initialized to zero

    f.setCoefficients(0);
    BOOST_CHECK_EQUAL(delayTime(f), 10);

    f.setCoefficients(1);
    BOOST_CHECK_EQUAL(delayTime(f), 21);

    f.setCoefficients(2);
    BOOST_CHECK_EQUAL(delayTime(f), 44);

    f.setCoefficients(3);
    BOOST_CHECK_EQUAL(delayTime(f), 89);

    f.setCoefficients(4);
    BOOST_CHECK_EQUAL(delayTime(f), 180);

    f.setCoefficients(5);
    BOOST_CHECK_EQUAL(delayTime(f), 361);

    f.setCoefficients(6);
    BOOST_CHECK_EQUAL(delayTime(f), 724);
}

BOOST_AUTO_TEST_CASE( filter_has_no_overshoot_and_approaches_input )
{
    FilterCascaded f;
    f.setCoefficients(3); // delay time 88
    temp_precise input = 10.0;
    for(int i=0; i<1000; i++){
        f.add(input);
        BOOST_REQUIRE_MESSAGE(double(f.readOutput()) <= 10.01, "output = " << f.readOutput());
    }

    BOOST_CHECK_CLOSE(double(f.readOutput()), 10.0, 0.1);
}

BOOST_AUTO_TEST_SUITE_END()


