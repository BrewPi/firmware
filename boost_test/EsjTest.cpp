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
#include "ActuatorTimeLimited.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorPwm.h"
#include "json_writer.h"
#include "json_reader.h"

BOOST_AUTO_TEST_SUITE(EsjTest)

BOOST_AUTO_TEST_CASE(serialize_nested_actuators) {
    //ActuatorBool * actBool = new ActuatorBool();
    //ActuatorTimeLimited * actTl = new ActuatorTimeLimited(actBool, 10, 20);
    ActuatorBool * boolAct1 = new ActuatorBool();
    ActuatorMutexDriver * mutexAct1 = new ActuatorMutexDriver(boolAct1);
    ActuatorPwm * act1 = new ActuatorPwm (mutexAct1, 20);

    std::string json;

    json = JSON::producer<ActuatorPwm>::convert(act1);

    /* With some extra whitespace, the valid output looks like this:
{
    "class": "ActuatorPwm",
    "variables": {
        "value": 0,
        "period": 20000,
        "minVal": 0,
        "maxVal": 100,
        "target": {
            "class": "ActuatorMutexDriver",
            "variables": {
                "mutexGroup": null,
                "target": {
                    "class": "ActuatorBool",
                    "variables": {
                        "state": false
                    }
                }
            }
        }
    }
}
*/

    std::string valid = R"({"class":"ActuatorPwm","variables":{"value":0.0000,)"
                        R"("period":20000,"minVal":0.0000,"maxVal":100.0000,)"
                        R"("target":{"class":"ActuatorMutexDriver",)"
                        R"("variables":{"mutexGroup":null,)"
                        R"("target":{"class":"ActuatorBool",)"
                        R"("variables":{"state":false}}}}}})";
    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_nested_actuators2) {
    ActuatorDigital* coolerPin = new ActuatorBool();
    ActuatorDigital* coolerTimeLimited = new ActuatorTimeLimited(coolerPin, 120, 180); // 2 min minOn time, 3 min minOff
    ActuatorMutexGroup * mutex = new ActuatorMutexGroup();
    ActuatorDigital* coolerMutex = new ActuatorMutexDriver(coolerTimeLimited, mutex);
    ActuatorRange* cooler = new ActuatorPwm(coolerMutex, 600); // period 10 min


    std::string json;

    json = JSON::producer<ActuatorRange>::convert(cooler);

/* With some extra whitespace, the valid output looks like this:
{
    "class": "ActuatorPwm",
    "variables": {
        "value": 0,
        "period": 600000,
        "minVal": 0,
        "maxVal": 100,
        "target": {
            "class": "ActuatorMutexDriver",
            "variables": {
                "mutexGroup": {
                    "class": "ActuatorMutexGroup",
                    "variables": {
                        "deadTime": 0,
                        "lastActiveTime": 0
                    }
                },
                "target": {
                    "class": "ActuatorTimeLimited",
                    "variables": {
                        "minOnTime": 120,
                        "minOffTime": 180,
                        "maxOnTime": 65535,
                        "active": false,
                        "target": {
                            "class": "ActuatorBool",
                            "variables": {
                                "state": false
                            }
                        }
                    }
                }
            }
        }
    }
}
*/


    std::string valid = R"({"class":"ActuatorPwm","variables":{"value":0.0000,)"
                        R"("period":600000,"minVal":0.0000,"maxVal":100.0000,)"
                        R"("target":{"class":"ActuatorMutexDriver",)"
                        R"("variables":{"mutexGroup":{"class":"ActuatorMutexGroup",)"
                        R"("variables":{"deadTime":0,"lastActiveTime":0}},)"
                        R"("target":{"class":"ActuatorTimeLimited",)"
                        R"("variables":{"minOnTime":120,"minOffTime":180,"maxOnTime":65535,)"
                        R"("active":false,"target":{"class":"ActuatorBool",)"
                        R"("variables":{"state":false}}}}}}}})";


    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_SUITE_END()

