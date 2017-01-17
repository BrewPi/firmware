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
#include <boost/algorithm/string/erase.hpp>

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
    ActuatorMutexDriver * mutexAct1 = new ActuatorMutexDriver(PtrLookup(boolAct1));
    ActuatorPwm * act1 = new ActuatorPwm (mutexAct1, 20);

    std::string json = JSON::producer<ActuatorPwm>::convert(act1);

    // With some extra whitespace, the valid output looks like this:
    std::string valid = \
    R"({                                         )"
    R"(    "kind": "ActuatorPwm",                )"
    R"(    "value": 0.0000,                      )"
    R"(    "period": 20,                         )"
    R"(    "minVal": 0.0000,                     )"
    R"(    "maxVal": 100.0000,                   )"
    R"(    "target": {                           )"
    R"(        "kind": "ActuatorMutexDriver",    )"
    R"(        "mutexGroup": null,               )"
    R"(        "target": {                       )"
    R"(            "kind": "ActuatorBool",       )"
    R"(            "state": false                )"
    R"(        }                                 )"
    R"(    }                                     )"
    R"(}                                         )";

    erase_all(valid, " "); // remove spaces from valid string

    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_nested_actuators2) {
    ActuatorDigitalInterface* coolerPin = new ActuatorBool();
    ActuatorDigitalInterface* coolerTimeLimited = new ActuatorTimeLimited(coolerPin, 120, 180); // 2 min minOn time, 3 min minOff
    ActuatorMutexGroup * mutex = new ActuatorMutexGroup();
    ActuatorDigitalInterface* coolerMutex = new ActuatorMutexDriver(PtrLookup(coolerTimeLimited, mutex));
    ActuatorRangeInterface* cooler = new ActuatorPwm(coolerMutex, 600); // period 10 min


    std::string json = JSON::producer<ActuatorRangeInterface>::convert(cooler);

    // With some extra whitespace, the valid output looks like this:
    std::string valid = \
    R"({                                             )"
    R"(    "kind": "ActuatorPwm",                    )"
    R"(    "value": 0.0000,                          )"
    R"(    "period": 600,                            )"
    R"(    "minVal": 0.0000,                         )"
    R"(    "maxVal": 100.0000,                       )"
    R"(    "target": {                               )"
    R"(        "kind": "ActuatorMutexDriver",        )"
    R"(        "mutexGroup": {                       )"
    R"(            "kind": "ActuatorMutexGroup",     )"
    R"(            "deadTime": 0,                    )"
    R"(            "waitTime": 0                     )"
    R"(        },                                    )"
    R"(        "target": {                           )"
    R"(            "kind": "ActuatorTimeLimited",    )"
    R"(            "minOnTime": 120,                 )"
    R"(            "minOffTime": 180,                )"
    R"(            "maxOnTime": 65535,               )"
    R"(            "state": false,                   )"
    R"(            "target": {                       )"
    R"(                "kind": "ActuatorBool",       )"
    R"(                 "state": false               )"
    R"(            }                                 )"
    R"(        }                                     )"
    R"(    }                                         )"
    R"(}                                             )";

    erase_all(valid, " "); // remove spaces from valid string

    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_setpoint) {
    SetPointInterface * sp1 = new SetPointSimple();
    SetPointInterface * sp2 = new SetPointConstant(20.0);
    SetPointSimple * sp3 = new SetPointSimple(25.0625);
    sp3->setName("test");

    std::string json = JSON::producer<SetPointInterface>::convert(sp1);
    std::string valid = R"({"kind":"SetPointSimple","name":"","value":null})";

    BOOST_CHECK_EQUAL(valid, json);

    json = JSON::producer<SetPointInterface>::convert(sp2);
    valid = R"({"kind":"SetPointConstant","value":20.0000})";

    BOOST_CHECK_EQUAL(valid, json);

    json = JSON::producer<SetPointInterface>::convert(sp3);
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
    SetPointInterface * sp1 = new SetPointSimple();
    SetPointInterface * sp2 = new SetPointConstant(20.0);
    TempSensorInterface * sens1 = new TempSensorMock(20.0);
    ActuatorRangeInterface * act = new ActuatorSetPoint(sp1, sens1, sp2, -10.0, 10.0);
    act->setValue(5.0); // should set sp1 to sp2 + 5.0 = 25.0;

    std::string json = JSON::producer<ActuatorRangeInterface>::convert(act);

    // With some extra whitespace, the valid output looks like this:
    std::string valid = \
    R"({                                      )"
    R"(    "kind": "ActuatorSetPoint",        )"
    R"(    "targetSetPoint": {                )"
    R"(        "kind": "SetPointSimple",      )"
    R"(        "name": "",                    )"
    R"(        "value": 25.0000               )"
    R"(    },                                 )"
    R"(    "targetSensor": {                  )"
    R"(        "kind": "TempSensorMock",      )"
    R"(        "value": 20.0000,              )"
    R"(        "connected": true              )"
    R"(    },                                 )"
    R"(    "referenceSetPoint": {             )"
    R"(        "kind": "SetPointConstant",    )"
    R"(        "value": 20.0000               )"
    R"(    },                                 )"
    R"(    "output": 5.0000,                  )"
    R"(    "achieved": 0.0000,                )"
    R"(    "minimum": -10.0000,               )"
    R"(    "maximum": 10.0000                 )"
    R"(}                                      )";

    erase_all(valid, " "); // remove spaces from valid string

    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_Pid) {
    TempSensorInterface * sensor = new TempSensorMock(20.0);
    ActuatorDigitalInterface * boolAct = new ActuatorBool();
    ActuatorRangeInterface * pwmAct = new ActuatorPwm(boolAct,4);
    SetPointInterface * sp = new SetPointSimple(20.0);
    Pid * pid = new Pid(PtrLookup(sensor), PtrLookup(pwmAct), PtrLookup(sp));

    std::string json = JSON::producer<Pid>::convert(pid);

    // With some extra whitespace, the valid output looks like this:
    std::string valid = \
    R"({                                      )"
    R"(    "kind": "Pid",                     )"
    R"(    "name":"",                         )"
    R"(    "enabled":true,                    )"
    R"(    "setPoint": {                      )"
    R"(        "kind": "SetPointSimple",      )"
    R"(        "name":"",                     )"
    R"(        "value": 20.0000               )"
    R"(    },                                 )"
    R"(    "input": {                         )"
    R"(        "kind": "TempSensorMock",      )"
    R"(        "value": 20.0000,              )"
    R"(        "connected": true              )"
    R"(    },                                 )"
    R"(    "inputError": 0.0000,              )"
    R"(    "Kp": 0.0000,                      )"
    R"(    "Ti": 0,                           )"
    R"(    "Td": 0,                           )"
    R"(    "p": 0.0000,                       )"
    R"(    "i": 0.0000,                       )"
    R"(    "d": 0.0000,                       )"
    R"(    "actuatorIsNegative": false,       )"
    R"(    "output": {                        )"
    R"(        "kind": "ActuatorPwm",         )"
    R"(        "value": 0.0000,               )"
    R"(        "period": 4,                   )"
    R"(        "minVal": 0.0000,              )"
    R"(        "maxVal": 100.0000,            )"
    R"(        "target": {                    )"
    R"(            "kind": "ActuatorBool",    )"
    R"(            "state": false             )"
    R"(        }                              )"
    R"(    }                                  )"
    R"(}                                      )";

    erase_all(valid, " "); // remove spaces from valid string

    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_TempSensor) {
    TempSensorInterface * s = new TempSensorMock(20.0);
    TempSensor * sensor = new TempSensor(s);
    sensor->setName("test");

    std::string json = JSON::producer<TempSensor>::convert(sensor);
    std::string valid = R"({"kind":"TempSensor","name":"test","sensor":{)"
        R"("kind":"TempSensorMock","value":20.0000,"connected":true}})";

    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_CASE(serialize_TempSensorFallback) {
    TempSensorMock * mainMock = new TempSensorMock(20.0);
    TempSensor * main = new TempSensor(mainMock);
    main->setName("main");

    TempSensorMock * backupMock = new TempSensorMock(21.0);
    TempSensor * backup = new TempSensor(backupMock);
    backup->setName("backup");

    TempSensorFallback * sensor = new TempSensorFallback(main, backup);


    std::string json = JSON::producer<TempSensorFallback>::convert(sensor);

    // Valid output looks like this with whitespace:
    std::string valid = \
    R"({                                      )"
    R"(    "kind": "TempSensorFallback",      )"
    R"(    "onBackupSensor": false,           )"
    R"(    "sensor": {                        )"
    R"(        "kind": "TempSensor",          )"
    R"(        "name": "main",                )"
    R"(        "sensor": {                    )"
    R"(            "kind": "TempSensorMock",  )"
    R"(            "value": 20.0000,          )"
    R"(            "connected": true          )"
    R"(        }                              )"
    R"(    }                                  )"
    R"(}                                      )";


    // now disconnect the main sensor
    mainMock->setConnected(false);
    sensor->update();

    // and check output again
    json = JSON::producer<TempSensorFallback>::convert(sensor);

    // Valid output looks like this with whitespace:
    valid = \
    R"({                                      )"
    R"(    "kind": "TempSensorFallback",      )"
    R"(    "onBackupSensor": true,            )"
    R"(    "sensor": {                        )"
    R"(        "kind": "TempSensor",          )"
    R"(        "name": "backup",              )"
    R"(        "sensor": {                    )"
    R"(            "kind": "TempSensorMock",  )"
    R"(            "value": 21.0000,          )"
    R"(            "connected": true          )"
    R"(        }                              )"
    R"(    }                                  )"
    R"(}                                      )";

    erase_all(valid, " "); // remove spaces from valid string

    BOOST_CHECK_EQUAL(valid, json);
}


BOOST_AUTO_TEST_CASE(serialize_control) {
    ticks.reset();
    Control * control = new Control();
    control->update();

    std::string json;

    json = JSON::producer<Control>::convert(control);

    std::string valid = \
    R"({                                                     )"
    R"( "kind": "Control",                                   )"
    R"( "pids": [{                                           )"
    R"(     "kind": "Pid",                                   )"
    R"(     "name": "heater1",                               )"
    R"(     "enabled": true,                                 )"
    R"(     "setPoint": {                                    )"
    R"(         "kind": "SetPointSimple",                    )"
    R"(         "name": "fridgeset",                         )"
    R"(         "value": null                                )"
    R"(     },                                               )"
    R"(     "input": {                                       )"
    R"(         "kind": "TempSensorFallback",                )"
    R"(         "onBackupSensor": false,                     )"
    R"(         "sensor": {                                  )"
    R"(             "kind": "TempSensor",                    )"
    R"(             "name": "fridge",                        )"
    R"(             "sensor": {                              )"
    R"(                 "kind": "TempSensorDisconnected",    )"
    R"(                 "value": null,                       )"
    R"(                 "connected": false                   )"
    R"(             }                                        )"
    R"(         }                                            )"
    R"(     },                                               )"
    R"(     "inputError": null,                              )"
    R"(     "Kp": 0.0000,                                    )"
    R"(     "Ti": 0,                                         )"
    R"(     "Td": 0,                                         )"
    R"(     "p": 0.0000,                                     )"
    R"(     "i": 0.0000,                                     )"
    R"(     "d": 0.0000,                                     )"
    R"(     "actuatorIsNegative": false,                     )"
    R"(     "output": {                                      )"
    R"(         "kind": "ActuatorPwm",                       )"
    R"(         "value": 0.0000,                             )"
    R"(         "period": 4,                                 )"
    R"(         "minVal": 0.0000,                            )"
    R"(         "maxVal": 100.0000,                          )"
    R"(         "target": {                                  )"
    R"(             "kind": "ActuatorMutexDriver",           )"
    R"(             "mutexGroup": {                          )"
    R"(                 "kind": "ActuatorMutexGroup",        )"
    R"(                 "deadTime": 1800000,                 )"
    R"(                 "waitTime": 0                        )"
    R"(             },                                       )"
    R"(             "target": {                              )"
    R"(                 "kind": "ActuatorNop",               )"
    R"(                 "state": false                       )"
    R"(             }                                        )"
    R"(         }                                            )"
    R"(     }                                                )"
    R"( }, {                                                 )"
    R"(     "kind": "Pid",                                   )"
    R"(     "name": "heater2",                               )"
    R"(     "enabled": true,                                 )"
    R"(     "setPoint": {                                    )"
    R"(         "kind": "SetPointSimple",                    )"
    R"(         "name": "beer2set",                          )"
    R"(         "value": null                                )"
    R"(     },                                               )"
    R"(     "input": {                                       )"
    R"(         "kind": "TempSensor",                        )"
    R"(         "name": "beer2",                             )"
    R"(         "sensor": {                                  )"
    R"(             "kind": "TempSensorDisconnected",        )"
    R"(             "value": null,                           )"
    R"(             "connected": false                       )"
    R"(         }                                            )"
    R"(     },                                               )"
    R"(     "inputError": null,                              )"
    R"(     "Kp": 0.0000,                                    )"
    R"(     "Ti": 0,                                         )"
    R"(     "Td": 0,                                         )"
    R"(     "p": 0.0000,                                     )"
    R"(     "i": 0.0000,                                     )"
    R"(     "d": 0.0000,                                     )"
    R"(     "actuatorIsNegative": false,                     )"
    R"(     "output": {                                      )"
    R"(         "kind": "ActuatorPwm",                       )"
    R"(         "value": 0.0000,                             )"
    R"(         "period": 4,                                 )"
    R"(         "minVal": 0.0000,                            )"
    R"(         "maxVal": 100.0000,                          )"
    R"(         "target": {                                  )"
    R"(             "kind": "ActuatorMutexDriver",           )"
    R"(             "mutexGroup": {                          )"
    R"(                 "kind": "ActuatorMutexGroup",        )"
    R"(                 "deadTime": 1800000,                 )"
    R"(                 "waitTime": 0                        )"
    R"(             },                                       )"
    R"(             "target": {                              )"
    R"(                 "kind": "ActuatorNop",               )"
    R"(                 "state": false                       )"
    R"(             }                                        )"
    R"(         }                                            )"
    R"(     }                                                )"
    R"( }, {                                                 )"
    R"(     "kind": "Pid",                                   )"
    R"(     "name": "cooler",                                )"
    R"(     "enabled": true,                                 )"
    R"(     "setPoint": {                                    )"
    R"(         "kind": "SetPointSimple",                    )"
    R"(         "name": "fridgeset",                         )"
    R"(         "value": null                                )"
    R"(     },                                               )"
    R"(     "input": {                                       )"
    R"(         "kind": "TempSensorFallback",                )"
    R"(         "onBackupSensor": false,                     )"
    R"(         "sensor": {                                  )"
    R"(             "kind": "TempSensor",                    )"
    R"(             "name": "fridge",                        )"
    R"(             "sensor": {                              )"
    R"(                 "kind": "TempSensorDisconnected",    )"
    R"(                 "value": null,                       )"
    R"(                 "connected": false                   )"
    R"(             }                                        )"
    R"(         }                                            )"
    R"(     },                                               )"
    R"(     "inputError": null,                              )"
    R"(     "Kp": 0.0000,                                    )"
    R"(     "Ti": 0,                                         )"
    R"(     "Td": 0,                                         )"
    R"(     "p": 0.0000,                                     )"
    R"(     "i": 0.0000,                                     )"
    R"(     "d": 0.0000,                                     )"
    R"(     "actuatorIsNegative": true,                      )"
    R"(     "output": {                                      )"
    R"(         "kind": "ActuatorPwm",                       )"
    R"(         "value": 0.0000,                             )"
    R"(         "period": 1200,                              )"
    R"(         "minVal": 0.0000,                            )"
    R"(         "maxVal": 100.0000,                          )"
    R"(         "target": {                                  )"
    R"(             "kind": "ActuatorMutexDriver",           )"
    R"(             "mutexGroup": {                          )"
    R"(                 "kind": "ActuatorMutexGroup",        )"
    R"(                 "deadTime": 1800000,                 )"
    R"(                 "waitTime": 0                        )"
    R"(             },                                       )"
    R"(             "target": {                              )"
    R"(                 "kind": "ActuatorTimeLimited",       )"
    R"(                 "minOnTime": 120,                    )"
    R"(                 "minOffTime": 180,                   )"
    R"(                 "maxOnTime": 65535,                  )"
    R"(                 "state": false,                      )"
    R"(                 "target": {                          )"
    R"(                     "kind": "ActuatorNop",           )"
    R"(                     "state": false                   )"
    R"(                 }                                    )"
    R"(             }                                        )"
    R"(         }                                            )"
    R"(     }                                                )"
    R"( }, {                                                 )"
    R"(     "kind": "Pid",                                   )"
    R"(     "name": "beer2fridge",                           )"
    R"(     "enabled": true,                                 )"
    R"(     "setPoint": {                                    )"
    R"(         "kind": "SetPointSimple",                    )"
    R"(         "name": "beer1set",                          )"
    R"(         "value": null                                )"
    R"(     },                                               )"
    R"(     "input": {                                       )"
    R"(         "kind": "TempSensor",                        )"
    R"(         "name": "beer1",                             )"
    R"(         "sensor": {                                  )"
    R"(             "kind": "TempSensorDisconnected",        )"
    R"(             "value": null,                           )"
    R"(             "connected": false                       )"
    R"(         }                                            )"
    R"(     },                                               )"
    R"(     "inputError": null,                              )"
    R"(     "Kp": 0.0000,                                    )"
    R"(     "Ti": 0,                                         )"
    R"(     "Td": 0,                                         )"
    R"(     "p": 0.0000,                                     )"
    R"(     "i": 0.0000,                                     )"
    R"(     "d": 0.0000,                                     )"
    R"(     "actuatorIsNegative": false,                     )"
    R"(     "output": {                                      )"
    R"(         "kind": "ActuatorSetPoint",                  )"
    R"(         "targetSetPoint": {                          )"
    R"(             "kind": "SetPointSimple",                )"
    R"(             "name": "fridgeset",                     )"
    R"(             "value": null                            )"
    R"(         },                                           )"
    R"(         "targetSensor": {                            )"
    R"(             "kind": "TempSensor",                    )"
    R"(             "name": "fridge",                        )"
    R"(             "sensor": {                              )"
    R"(                 "kind": "TempSensorDisconnected",    )"
    R"(                 "value": null,                       )"
    R"(                 "connected": false                   )"
    R"(             }                                        )"
    R"(         },                                           )"
    R"(         "referenceSetPoint": {                       )"
    R"(             "kind": "SetPointSimple",                )"
    R"(             "name": "beer1set",                      )"
    R"(             "value": null                            )"
    R"(         },                                           )"
    R"(         "output": 0.0000,                            )"
    R"(         "achieved": null,                            )"
    R"(         "minimum": -10.0000,                         )"
    R"(         "maximum": 10.0000                           )"
    R"(     }                                                )"
    R"( }]                                                   )"
    R"(}                                                     )";

    erase_all(valid, " "); // remove spaces from valid string

    BOOST_CHECK_EQUAL(valid, json);
}

BOOST_AUTO_TEST_SUITE_END()

