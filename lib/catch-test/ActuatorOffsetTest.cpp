#include <iostream>
#include "Catch.hpp"

#include "ActuatorOffset.h"
#include "defaultDevices.h"
#include "TempSensorMock.h"
#include "SensorSetPointPair.h"

SCENARIO("Setting of value", "[actuator]") {
    GIVEN( "A target and a reference" ) {
        TempSensorMock targetSensor(20.0);
        SetPointSimple targetSetPoint(20.0);
        TempSensorMock referenceSensor(19.0);
        SetPointSimple referenceSetPoint(20.0);

        SensorSetPointPair target(targetSensor, targetSetPoint);
        SensorSetPointPair reference(referenceSensor, referenceSetPoint);

        ActuatorOffset act(target, reference);

        WHEN("set to positive offset") {
		    act.set(10.0);
            THEN( "the target setpoint changes" ) {
                REQUIRE(targetSetPoint.read() == temp_t(30.0));
                REQUIRE(referenceSetPoint.read() == temp_t(20.0));
                // The setting will always be what is handed to this method. With the side-effect of adding the value to the targetSetPoint.
                // Will not assert on the setting more than once.
                REQUIRE(act.setting() == temp_t(10.0));
                REQUIRE(act.value() == temp_t(0.0));

                WHEN("target temperature reaches offset") {
                    targetSensor.setTemp(30.0);
                    THEN("offset value is reached") {
                        REQUIRE(act.value() == temp_t(10.0));
                    }
                }
            }
        }

        WHEN("set to negative offset") {
			act.set(-10.0);

            THEN( "the target setpoint changes" ) {
                REQUIRE(targetSetPoint.read() == temp_t(10.0));
                REQUIRE(referenceSetPoint.read() == temp_t(20.0));
                //std::cout <<  "x = " << act.value() << std::endl;
                REQUIRE(act.value() == temp_t(0.0));

                WHEN("target temperature reaches offset") {
                    targetSensor.setTemp(10.0);
                    THEN("offset value is reached") {
                        REQUIRE(act.value() == temp_t(-10.0));
                    }
                }
            }
        }

        WHEN("using the reference setpoint as the baseline") {
            referenceSetPoint.write(10.0);
            act.set(12.0);

            THEN("the values will be the target minus the refernece") {
                REQUIRE(act.value() == temp_t(10.0));
                REQUIRE(targetSetPoint.read() == temp_t(22.0));
            }
            
            WHEN("the reference setpoint becomes invalid") {                
                referenceSetPoint.write(temp_t::invalid());
                act.set(12.0);

                THEN("the target setpoint will be invalid") {
                    REQUIRE(act.value() == temp_t::invalid());
                    REQUIRE(targetSetPoint.read() == temp_t::invalid());
                }
            }
        }

        WHEN("using the reference sensor as the baseline") {
            act.setReferenceSettingOrValue(true);
            referenceSensor.setTemp(15.0);
            act.set(12.0);

            THEN("the values change accordingly") {
                REQUIRE(act.value() == temp_t(5.0));
                REQUIRE(targetSetPoint.read() == temp_t(27.0));
            }

            WHEN("the reference sensor becomes invalid") {            
                referenceSensor.setConnected(false);
                act.set(12.0);

                THEN("the target setpoint will be invalid") {
                    REQUIRE(act.value() == temp_t::invalid());
                    REQUIRE(targetSetPoint.read() == temp_t::invalid());
                }
            }
        }
    }
}

SCENARIO("Min max", "[actuator]") {
    TempSensorMock targetSensor(20.0);
    SetPointSimple targetSetPoint(20.0);
    TempSensorMock referenceSensor(20.0);
    SetPointSimple referenceSetPoint(20.0);

    SensorSetPointPair target(targetSensor, targetSetPoint);
    SensorSetPointPair reference(referenceSensor, referenceSetPoint);

    ActuatorOffset act(target, reference, -10.0, 10.0);

    WHEN("the setting is within the range") {
        act.set(5.0);
        targetSensor.setTemp(30.0);

        THEN("the setting is un-altered") {
            REQUIRE(act.setting() == temp_t(5.0));
            REQUIRE(referenceSetPoint.read() == temp_t(20.0));
            REQUIRE(targetSetPoint.read() == temp_t(25.0));
        }
    }

    WHEN("the setting is above the maximum") {
        act.set(20.0);
        targetSensor.setTemp(30.0);

        THEN("the setting is pinned to the maximum") {
            REQUIRE(act.setting() == temp_t(10.0));
            REQUIRE(referenceSetPoint.read() == temp_t(20.0));
            REQUIRE(targetSetPoint.read() == temp_t(30.0));
        }
    }

    WHEN("the setting goes below the minimum") {
        act.set(-20.0);
        targetSensor.setTemp(10.0);

        THEN("the settings are pinned to the minimum") {
            REQUIRE(act.setting() == temp_t(-10.0));
            REQUIRE(referenceSetPoint.read() == temp_t(20.0));
            REQUIRE(targetSetPoint.read() == temp_t(10.0));
        }
    }
}

SCENARIO("Target sensor", "[actuator]") {
    TempSensorMock targetSensor(20.0);
    SetPointSimple targetSetPoint(20.0);
    TempSensorMock referenceSensor(20.0);
    SetPointSimple referenceSetPoint(20.0);

    SensorSetPointPair target(targetSensor, targetSetPoint);
    SensorSetPointPair reference(referenceSensor, referenceSetPoint);

    ActuatorOffset act(target, reference);

    WHEN("the target sensor becomes invalid") {
        act.set(20.0);
        targetSensor.setConnected(false);
        act.update();

        THEN("the actuator also turns invalid") {
            REQUIRE(referenceSetPoint.read() == temp_t(20.0));
            REQUIRE(targetSetPoint.read() == temp_t(40.0));
            REQUIRE(act.value() == temp_t::invalid());
        }
    }
}