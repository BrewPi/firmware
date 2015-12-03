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
#include <cstdio>
#include "str_functions.h"

BOOST_AUTO_TEST_SUITE( str_functions )

BOOST_AUTO_TEST_CASE( my_strtol_test )
{
    char s[20];
    char * end;
    char * end2;
    long result;
    long result2;
    for(long i = -LONG_MIN; i<LONG_MAX*.99;i+=LONG_MAX*0.001){
        snprintf(s, 20, "%ld", i);
        result = strtol(s, &end, 10);
        result2 = my_strtol(s, &end2);

        BOOST_REQUIRE_EQUAL(end, end2);
        BOOST_REQUIRE_EQUAL(result, result2);
        BOOST_REQUIRE_EQUAL(i, result2);
    }
}


BOOST_AUTO_TEST_SUITE_END()


