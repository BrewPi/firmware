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
#include "ActuatorSetPoint.h"
#include "TempSensorMock.h"
#include "TempSensor.h"
#include "Pid.h"
#include "SetPoint.h"
#include "Control.h"
#include "json_writer.h"

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
        "value": 0.0000,
        "period": 20,
        "minVal": 0.0000,
        "maxVal": 100.0000,
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
                        R"("period":20,"minVal":0.0000,"maxVal":100.0000,)"
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
        "value": 0.0000,
        "period": 600,
        "minVal": 0.0000,
        "maxVal": 100.0000,
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
                        R"("period":600,"minVal":0.0000,"maxVal":100.0000,)"
                        R"("target":{"class":"ActuatorMutexDriver",)"
                        R"("variables":{"mutexGroup":{"class":"ActuatorMutexGroup",)"
                        R"("variables":{"deadTime":0,"lastActiveTime":0}},)"
                        R"("target":{"class":"ActuatorTimeLimited",)"
                        R"("variables":{"minOnTime":120,"minOffTime":180,"maxOnTime":65535,)"
                        R"("active":false,"target":{"class":"ActuatorBool",)"
                        R"("variables":{"state":false}}}}}}}})";


    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_setpoint) {
    SetPoint * sp1 = new SetPointSimple();
    SetPoint * sp2 = new SetPointConstant(20.0);


    std::string json = JSON::producer<SetPoint>::convert(sp1);
    std::string valid = R"({"class":"SetPointSimple","variables":{"value":null}})";

    BOOST_CHECK_EQUAL(valid, json);

    json = JSON::producer<SetPoint>::convert(sp2);
    valid = R"({"class":"SetPointConstant","variables":{"value":20.0000}})";

    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_ActuatorSetPoint) {
    SetPoint * sp1 = new SetPointSimple();
    SetPoint * sp2 = new SetPointConstant(20.0);
    TempSensorBasic * sens1 = new TempSensorMock(20.0);
    ActuatorRange * act = new ActuatorSetPoint(sp1, sens1, sp2, -10.0, 10.0);
    act->setValue(5.0); // should set sp1 to sp2 + 5.0 = 25.0;

    std::string json = JSON::producer<ActuatorRange>::convert(act);

/* With some extra whitespace, the valid output looks like this:
    {
        "class": "ActuatorSetPoint",
        "variables": {
            "targetSetPoint": {
                "class": "SetPointSimple",
                "variables": {
                    "value": 25.0000
                }
            },
            "targetSensor": {
                "class": "TempSensorMock",
                "variables": {
                    "value": 20.0000,
                    "connected": true
                }
            },
            "referenceSetPoint": {
                "class": "SetPointConstant",
                "variables": {
                    "value": 20.0000
                }
            },
            "minimum": -10.0000,
            "maximum": 10.0000
        }
    }
*/

    std::string valid = R"({"class":"ActuatorSetPoint","variables":{)"
        R"("targetSetPoint":{"class":"SetPointSimple","variables":{"value":25.0000}},)"
        R"("targetSensor":{"class":"TempSensorMock","variables":{"value":20.0000,"connected":true}},)"
        R"("referenceSetPoint":{"class":"SetPointConstant","variables":{)"
        R"("value":20.0000}},"minimum":-10.0000,"maximum":10.0000}})";

    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_Pid) {
    TempSensorBasic * sensor = new TempSensorMock(20.0);
    ActuatorDigital * boolAct = new ActuatorBool();
    ActuatorRange * pwmAct = new ActuatorPwm(boolAct,4);
    SetPoint * sp = new SetPointSimple(20.0);
    Pid * pid = new Pid(sensor, pwmAct, sp);

    std::string json = JSON::producer<Pid>::convert(pid);

/* With some extra whitespace, the valid output looks like this:
{
    "class": "Pid",
    "variables": {
        "name":"",
        "enabled":true,
        "setPoint": {
            "class": "SetPointSimple",
            "variables": {
                "value": 20.0000
            }
        },
        "inputSensor": {
            "class": "TempSensorMock",
            "variables": {
                "value": 20.0000,
                "connected": true
            }
        },
        "inputError": 0,
        "Kp": 0.0000,
        "Ti": 0,
        "Td": 0,
        "p": 0.0000,
        "i": 0.0000,
        "d": 0.0000,
        "actuatorIsNegative": false,
        "outputActuator": {
            "class": "ActuatorPwm",
            "variables": {
                "value": 0.0000,
                "period": 4,
                "minVal": 0.0000,
                "maxVal": 100.0000,
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

    std::string valid = R"({"class":"Pid","variables":{"name":"","enabled":true,)"
        R"("setPoint":{"class":"SetPointSimple","variables":{"value":20.0000}},)"
        R"("inputSensor":{"class":"TempSensorMock","variables":{"value":20.0000,"connected":true}},)"
        R"("inputError":0.0000,"Kp":0.0000,"Ti":0,"Td":0,"p":0.0000,"i":0.0000,"d":0.0000,)"
        R"("actuatorIsNegative":false,"outputActuator":{"class":"ActuatorPwm",)"
        R"("variables":{"value":0.0000,"period":4,"minVal":0.0000,"maxVal":100.0000,)"
        R"("target":{"class":"ActuatorBool","variables":{"state":false}}}}}})";

    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_TempSensor) {
    TempSensorBasic * s = new TempSensorMock(20.0);
    TempSensor * sensor = new TempSensor(s, "test");

    std::string json = JSON::producer<TempSensor>::convert(sensor);
    std::string valid = R"({"class":"TempSensor","variables":{"name":"test","sensor":{)"
        R"("class":"TempSensorMock","variables":{"value":20.0000,"connected":true}}}})";

    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_control) {

    Control * control = new Control();

    std::string json;

    json = JSON::producer<Control>::convert(control);

    BOOST_TEST_MESSAGE("Control JSON = " << json); // use --log_level=all to see in stdout
}

BOOST_AUTO_TEST_SUITE_END()

