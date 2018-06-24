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
#include <string>

#include "ActuatorInterfaces.h"
#include "ActuatorMocks.h"

BOOST_AUTO_TEST_SUITE(ActuatorMocksTest)

BOOST_AUTO_TEST_CASE(ActuatorBoolTest) {
    ActuatorBool * act = new ActuatorBool();

    BOOST_CHECK(act->getState() == ActuatorDigital::State::Inactive); // defaults to inactive

    act->setState(ActuatorDigital::State::Active);
    BOOST_CHECK(act->getState() == ActuatorDigital::State::Active); // can be set to active

    act->setState(ActuatorDigital::State::Inactive);
    BOOST_CHECK(act->getState() == ActuatorDigital::State::Inactive); // can be set to inactive
}


BOOST_AUTO_TEST_SUITE_END()

