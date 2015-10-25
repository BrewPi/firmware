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

#include "runner.h"
#include "Nameable.h"

BOOST_AUTO_TEST_SUITE(NameableTest)

BOOST_AUTO_TEST_CASE(set_get) {
    Nameable obj;

    BOOST_CHECK_EQUAL(obj.getName(), "");

    obj.setName("Test");
    BOOST_CHECK_EQUAL(obj.getName(), "Test");
}

BOOST_AUTO_TEST_CASE(too_long_name_gets_truncated) {
    Nameable obj;

    obj.setName("TestTestTestTest");
    BOOST_CHECK_EQUAL(obj.getName(), "TestTestTestTes");
}

BOOST_AUTO_TEST_SUITE_END()

