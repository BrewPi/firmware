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

#include "Pid.h"
#include <cstdio>
#include <math.h>
#include "TempSensorMock.h"
#include "Actuator.h"

BOOST_AUTO_TEST_SUITE( pid_test )

BOOST_AUTO_TEST_CASE( pid_init )
{
    MockTempSensor sensor(20.0);
    ValueActuator actuator;

    Pid pid(&sensor, &actuator);
}

BOOST_AUTO_TEST_SUITE_END()

