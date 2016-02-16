/*
* Copyright 2016 BrewPi/Elco Jacobs.
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
#include <boost/algorithm/string/erase.hpp>

#include "runner.h"
#include <string>

#include "../app/controller/SmartPtrContainerDemo.h"
#include "json_writer.h"

BOOST_AUTO_TEST_SUITE(SmartPtrContainerTest)

BOOST_AUTO_TEST_CASE(serialize_test_objects) {

    SmartPtrContainer<TestTarget> target(new TestTarget(2));
    SmartPtrContainer<TestDriver> driver(new TestDriver(target));

    std::string json;
    json = JSON::producer<SmartPtrContainer<TestDriver>>::convert(driver);

    // Valid output looks like this with whitespace:
    std::string valid =
    R"({                             )"
    R"(    "kind": "TestDriver",     )"
    R"(    "id": 1,                  )"
    R"(    "a": 1,                   )"
    R"(    "target": {               )"
    R"(        "kind": "TestTarget", )"
    R"(        "id": 0,              )"
    R"(        "b": 2                )"
    R"(    },                        )"
    R"(    "ab": 2                   )"
    R"(}                             )";

    erase_all(valid, " "); // remove spaces from valid string

    BOOST_CHECK_EQUAL(valid, json);
}
BOOST_AUTO_TEST_SUITE_END()

