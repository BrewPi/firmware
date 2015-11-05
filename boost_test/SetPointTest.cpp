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


#include "SetPoint.h"
#include "defaultDevices.h"
#include "runner.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(SetPointTest)

BOOST_AUTO_TEST_CASE(init_write_read){
    SetPointSimple sp;

    BOOST_REQUIRE_EQUAL(sp.read(), temp_t::disabled()); // default value is 'disabled'
    sp.write(temp_t(21.1));
    BOOST_REQUIRE_EQUAL(sp.read(), temp_t(21.1));
}

BOOST_AUTO_TEST_CASE(default_setpoint_is_immutable){
    SetPoint * sp = defaultSetPoint();

    BOOST_REQUIRE_EQUAL(sp->read(), temp_t::invalid()); // value is 'disabled'
    sp->write(temp_t(21.1));
    BOOST_REQUIRE_EQUAL(sp->read(), temp_t::invalid()); // value is still 'disabled'
}

BOOST_AUTO_TEST_SUITE_END()
