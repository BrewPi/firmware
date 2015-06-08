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

BOOST_AUTO_TEST_SUITE( temperature_suite )


BOOST_AUTO_TEST_CASE( temperature_test )
{
    temp_diff x(3.0);
    temp_diff y(3.0);
    temp_diff z(9.0);

    BOOST_CHECK(x*y == z);

    temp t(0.0);
    temp_diff d(-48.0);
    BOOST_CHECK(t == d); // temperatures are stored with -48C offset
}

BOOST_AUTO_TEST_CASE( conversion_between_normal_and_long )
{
//    temp_diff td_normal(3.0);

//    temp_diff_long td_long = td_normal;

    //BOOST_CHECK(td_long == temp_long(3.0));*/
}



BOOST_AUTO_TEST_SUITE_END()
