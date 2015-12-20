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
#include <math.h>

BOOST_AUTO_TEST_SUITE( filter_suite )

BOOST_AUTO_TEST_CASE( filter_init )
{
    FilterCascaded f;
    BOOST_REQUIRE_EQUAL(f.readOutput(), temp_precise_t(0.0));
}

long delayTime(FilterCascaded f){
    temp_precise_t target = 100.0;
    f.init(temp_precise_t(0.0));
    long addedSamples = 0;
    do{
        f.add(target);
        addedSamples++;
    } while(double(f.readOutput()) < 50.0);
    return addedSamples-1; // delay is added samples - 1, because after 1 sample would be 0 delay.
}

BOOST_AUTO_TEST_CASE( filter_delay_time )
{
    /* Cascaded(3x) filter delay times (analyzed with matlab, see FixedFilter.h)
     *       a=4,    b=0,    delay time = 9
     *       a=6,    b=1,    delay time = 20
     *       a=8,    b=2,    delay time = 43
     *       a=10,   b=3,    delay time = 88
     *       a=12,   b=4,    delay time = 179
     *       a=14,   b=5,    delay time = 360
     *       a=16,   b=6,    delay time = 723
     */

    FilterCascaded f; // initialized to zero

    f.setFiltering(0);
    BOOST_CHECK_EQUAL(delayTime(f), 9);

    f.setFiltering(1);
    BOOST_CHECK_EQUAL(delayTime(f), 20);

    f.setFiltering(2);
    BOOST_CHECK_EQUAL(delayTime(f), 43);

    f.setFiltering(3);
    BOOST_CHECK_EQUAL(delayTime(f), 88);

    f.setFiltering(4);
    BOOST_CHECK_EQUAL(delayTime(f), 179);

    f.setFiltering(5);
    BOOST_CHECK_EQUAL(delayTime(f), 360);

    f.setFiltering(6);
    BOOST_CHECK_EQUAL(delayTime(f), 723);
}

BOOST_AUTO_TEST_CASE( filter_has_no_overshoot_and_approaches_input )
{
    FilterCascaded f;
    f.setFiltering(3); // delay time 88
    temp_precise_t input = 10.0;
    for(int i=0; i<1000; i++){
        f.add(input);
        BOOST_REQUIRE_MESSAGE(double(f.readOutput()) <= 10.01, "output = " << f.readOutput());
    }

    BOOST_CHECK_CLOSE(double(f.readOutput()), 10.0, 0.1);
}


BOOST_AUTO_TEST_CASE( peak_detect )
{
    FilterCascaded f;
    f.setFiltering(2); // delay time 88
    double amplitude = 10.0;
    double period = 1000.0;
    int peaks_detected = 0;

    for(double t=0; t<5000; t++){
        temp_precise_t val = amplitude * sin( t * 2 * M_PI / period );
        f.add(val);
        temp_precise_t peak;
        if(t>10.0){
            if(f.detectPosPeak(&peak)){
                BOOST_CHECK_CLOSE(double(peak), 10.0, 1);  // accept 1% margin due to filtering
                peaks_detected++;
            }
            if(f.detectNegPeak(&peak)){
                BOOST_CHECK_CLOSE(double(peak), -10.0, 1);
                peaks_detected++;
            }
        }
    }
    BOOST_CHECK_EQUAL(peaks_detected, 10);
}

BOOST_AUTO_TEST_CASE( coefficients_and_delay )
{
    FilterCascaded f;

    for(uint16_t d = 10; d < 1000; d++){
        f.setFilteringForDelay(d);
        BOOST_CHECK_MESSAGE(f.getDelay() <= d, "Filter delay ("
                << f.getDelay() << ")is more than maximum delay " << d);
    }

    BOOST_CHECK_EQUAL(f.getDelay(), 723); // ends at max delay

}
BOOST_AUTO_TEST_SUITE_END()


