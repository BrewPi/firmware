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


#include "ActuatorSetPoint.h"
#include "defaultDevices.h"
#include "runner.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ActuatorSetPointTest)

BOOST_AUTO_TEST_CASE(set_value){
    SetPointSimple target(20.0);
    SetPointSimple reference(20.0);

    ActuatorSetPoint act(&target, &reference);

    act.setValue(10.0);

    BOOST_REQUIRE_EQUAL(reference.read(), temp_t(20.0));
    BOOST_REQUIRE_EQUAL(target.read(), temp_t(30.0));
    BOOST_REQUIRE_EQUAL(act.readValue(), temp_t(10.0));

    act.setValue(-10.0);
    BOOST_REQUIRE_EQUAL(reference.read(), temp_t(20.0));
    BOOST_REQUIRE_EQUAL(target.read(), temp_t(10.0));
    BOOST_REQUIRE_EQUAL(act.readValue(), temp_t(-10.0));
}

BOOST_AUTO_TEST_CASE(min_max){
    SetPointSimple target(20.0);
    SetPointSimple reference(20.0);

    ActuatorSetPoint act(&target, &reference, -10.0, 10.0);

    act.setValue(20.0);

    BOOST_REQUIRE_EQUAL(reference.read(), temp_t(20.0));
    BOOST_REQUIRE_EQUAL(target.read(), temp_t(30.0));
    BOOST_REQUIRE_EQUAL(act.readValue(), temp_t(10.0));

    act.setValue(-20.0);
    BOOST_REQUIRE_EQUAL(reference.read(), temp_t(20.0));
    BOOST_REQUIRE_EQUAL(target.read(), temp_t(10.0));
    BOOST_REQUIRE_EQUAL(act.readValue(), temp_t(-10.0));
}

BOOST_AUTO_TEST_SUITE_END()
