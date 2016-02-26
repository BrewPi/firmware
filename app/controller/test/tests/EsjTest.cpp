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
    "kind": "ActuatorPwm",
    "value": 0.0000,
    "period": 20,
    "minVal": 0.0000,
    "maxVal": 100.0000,
    "target": {
        "kind": "ActuatorMutexDriver",
        "mutexGroup": null,
        "target": {
            "kind": "ActuatorBool",
            "state": false
        }
    }
}
*/

    std::string valid = R"({"kind":"ActuatorPwm","value":0.0000,)"
                        R"("period":20,"minVal":0.0000,"maxVal":100.0000,)"
                        R"("target":{"kind":"ActuatorMutexDriver","mutexGroup":null,)"
                        R"("target":{"kind":"ActuatorBool","state":false}}})";
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
    "kind": "ActuatorPwm",
    "value": 0.0000,
    "period": 600,
    "minVal": 0.0000,
    "maxVal": 100.0000,
    "target": {
        "kind": "ActuatorMutexDriver",
        "mutexGroup": {
            "kind": "ActuatorMutexGroup",
            "deadTime": 0,
            "lastActiveTime": 0
        },
        "target": {
            "kind": "ActuatorTimeLimited",
            "minOnTime": 120,
            "minOffTime": 180,
            "maxOnTime": 65535,
            "active": false,
            "target": {
                "kind": "ActuatorBool",
                 "state": false
            }
        }
    }
}
*/


    std::string valid = R"({"kind":"ActuatorPwm","value":0.0000,)"
                        R"("period":600,"minVal":0.0000,"maxVal":100.0000,)"
                        R"("target":{"kind":"ActuatorMutexDriver","mutexGroup":)"
                        R"({"kind":"ActuatorMutexGroup","deadTime":0,"lastActiveTime":0},)"
                        R"("target":{"kind":"ActuatorTimeLimited","minOnTime":120,"minOffTime":180,"maxOnTime":65535,)"
                        R"("state":false,"target":{"kind":"ActuatorBool","state":false}}}})";


    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_setpoint) {
    SetPoint * sp1 = new SetPointSimple();
    SetPoint * sp2 = new SetPointConstant(20.0);
    SetPointSimple * sp3 = new SetPointSimple(25.0625);
    sp3->setName("test");

    std::string json = JSON::producer<SetPoint>::convert(sp1);
    std::string valid = R"({"kind":"SetPointSimple","name":"","value":null})";

    BOOST_CHECK_EQUAL(valid, json);

    json = JSON::producer<SetPoint>::convert(sp2);
    valid = R"({"kind":"SetPointConstant","value":20.0000})";

    BOOST_CHECK_EQUAL(valid, json);

    json = JSON::producer<SetPoint>::convert(sp3);
    valid = R"({"kind":"SetPointSimple","name":"test","value":25.0625})";

    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_setpointMinMax) {
    SetPointMinMax * sp1 = new SetPointMinMax();

    std::string json = JSON::producer<SetPointMinMax>::convert(sp1);
    std::string valid = R"({"kind":"SetPointMinMax","value":null,"min":-127.9922,"max":127.9961})";

    BOOST_CHECK_EQUAL(valid, json);

    sp1->write(20.0);
    sp1->setMin(-10.0);
    sp1->setMax(30.0);

    json = JSON::producer<SetPointMinMax>::convert(sp1);
    valid = R"({"kind":"SetPointMinMax","value":20.0000,"min":-10.0000,"max":30.0000})";

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
        "kind": "ActuatorSetPoint",
        "targetSetPoint": {
            "kind": "SetPointSimple",
            "name": "",
            "value": 25.0000
        },
        "targetSensor": {
            "kind": "TempSensorMock",
            "value": 20.0000,
            "connected": true
        },
        "referenceSetPoint": {
            "kind": "SetPointConstant",
            "value": 20.0000
        },
        "output": 5.0000,
        "achieved": 0.0000,
        "minimum": -10.0000,
        "maximum": 10.0000
    }
*/

    std::string valid = R"({"kind":"ActuatorSetPoint",)"
        R"("targetSetPoint":{"kind":"SetPointSimple","name":"","value":25.0000},)"
        R"("targetSensor":{"kind":"TempSensorMock","value":20.0000,"connected":true},)"
        R"("referenceSetPoint":{"kind":"SetPointConstant","value":20.0000},)"
        R"("output":5.0000,"achieved":0.0000,"minimum":-10.0000,"maximum":10.0000})";

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
    "kind": "Pid",
    "name":"",
    "enabled":true,
    "setPoint": {
        "kind": "SetPointSimple",
        "name":"",
        "value": 20.0000
    },
    "inputSensor": {
        "kind": "TempSensorMock",
        "value": 20.0000,
        "connected": true
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
        "kind": "ActuatorPwm",
        "value": 0.0000,
        "period": 4,
        "minVal": 0.0000,
        "maxVal": 100.0000,
        "target": {
            "kind": "ActuatorBool",
            "state": false
        }
    }
}
*/

    std::string valid = R"({"kind":"Pid","name":"","enabled":true,)"
        R"("setPoint":{"kind":"SetPointSimple","name":"","value":20.0000},)"
        R"("inputSensor":{"kind":"TempSensorMock","value":20.0000,"connected":true},)"
        R"("inputError":0.0000,"Kp":0.0000,"Ti":0,"Td":0,"p":0.0000,"i":0.0000,"d":0.0000,)"
        R"("actuatorIsNegative":false,"outputActuator":{"kind":"ActuatorPwm",)"
        R"("value":0.0000,"period":4,"minVal":0.0000,"maxVal":100.0000,)"
        R"("target":{"kind":"ActuatorBool","state":false}}})";

    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_TempSensor) {
    TempSensorBasic * s = new TempSensorMock(20.0);
    TempSensor * sensor = new TempSensor(s);
    sensor->setName("test");

    std::string json = JSON::producer<TempSensor>::convert(sensor);
    std::string valid = R"({"kind":"TempSensor","name":"test","sensor":{)"
        R"("kind":"TempSensorMock","value":20.0000,"connected":true}})";

    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_control) {

    Control * control = new Control();

    std::string json;

    json = JSON::producer<Control>::convert(control);

    BOOST_TEST_MESSAGE("Control JSON = " << json); // use --log_level=all to see in stdout
}

BOOST_AUTO_TEST_SUITE_END()

