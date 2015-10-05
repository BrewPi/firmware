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


#include "TempSensorMock.h"
#include "runner.h"
#include <boost/test/unit_test.hpp>
#include <math.h>

BOOST_AUTO_TEST_SUITE(TempSensorMockTest)

BOOST_AUTO_TEST_CASE (mock_sensor_init){
    TempSensorBasic * s = new TempSensorMock(20.0);
    temp_t t = s->read();

    BOOST_CHECK_EQUAL(t, temp_t(20.0));
}

BOOST_AUTO_TEST_CASE(test_limited_precision){
    TempSensorMock t(20.0);

    for(double d = -10; d < 120; d += 0.01){
        t.setTemp(d);
        temp_t val = t.read();
        BOOST_REQUIRE_EQUAL(fmod(double(val), 0.0625), 0);
    }
}

BOOST_AUTO_TEST_SUITE_END()
