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


#include "TempSensorFallback.h"
#include "TempSensorMock.h"
#include "runner.h"
#include <boost/test/unit_test.hpp>
#include <math.h>
#include <boost/test/output_test_stream.hpp>

BOOST_AUTO_TEST_SUITE(TempSensorFallbackTest)

/**
 * Fixture that sets up sensors like they would be used in BrewPi.
 * The fridge sensor will be used by a PID, but when it is unavailable, the PID will
 * take its input directly from the beer sensor
 */
struct FallbackFixture{
public:
    FallbackFixture(){
        beerSensor = new TempSensorMock(initialBeerTemp);
        fridgeSensor = new TempSensorMock(initialFridgeTemp);
        fridgeSensorWithFallback = new TempSensorFallback(fridgeSensor, beerSensor);
    }
    ~FallbackFixture(){
        BOOST_TEST_MESSAGE( "tear down PID test fixture" );
        delete beerSensor;
        delete fridgeSensor;
        delete fridgeSensorWithFallback;
    }

    TempSensorMock * beerSensor;
    TempSensorMock * fridgeSensor;
    TempSensorFallback * fridgeSensorWithFallback;
    temp_t const initialFridgeTemp = 20.0;
    temp_t const initialBeerTemp = 25.0;
};


BOOST_AUTO_TEST_CASE (fallback_sensor_without_arguments_defaults_to_default_sensor){
    TempSensorInterface * s = new TempSensorFallback();
    temp_t t = s->read();

    BOOST_CHECK_EQUAL(t, temp_t::invalid());
}


BOOST_FIXTURE_TEST_CASE (fallback_sensor_returns_main_sensor_value, FallbackFixture){
    temp_t t = fridgeSensorWithFallback->read();
    BOOST_CHECK_EQUAL(t, temp_t(initialFridgeTemp));

    fridgeSensorWithFallback->update();
    t = fridgeSensorWithFallback->read();
    BOOST_CHECK_EQUAL(t, temp_t(initialFridgeTemp)); // still on main sensor
}


BOOST_FIXTURE_TEST_CASE (fallback_sensor_returns_backup_when_main_is_disconnected, FallbackFixture){
    temp_t t = fridgeSensorWithFallback->read();
    BOOST_CHECK_EQUAL(t, temp_t(initialFridgeTemp));
    BOOST_CHECK(fridgeSensorWithFallback->activeSensor() == fridgeSensor);

    fridgeSensor->setConnected(false);
    t = fridgeSensorWithFallback->read();
    BOOST_CHECK_EQUAL(t, temp_t::invalid()); // still pointing at fridge temp, but now invalid
    BOOST_CHECK(fridgeSensorWithFallback->activeSensor() == fridgeSensor);

    fridgeSensorWithFallback->update();
    t = fridgeSensorWithFallback->read();
    BOOST_CHECK_EQUAL(t, temp_t(initialBeerTemp)); // but not after update
    BOOST_CHECK(fridgeSensorWithFallback->activeSensor() == beerSensor);
}


BOOST_FIXTURE_TEST_CASE (fallback_sensor_is_connected_when_main_or_backup_is_connected, FallbackFixture){
    fridgeSensor->setConnected(true);
    beerSensor->setConnected(true);
    fridgeSensorWithFallback->update();
    BOOST_CHECK(fridgeSensorWithFallback->isConnected());

    fridgeSensor->setConnected(true);
    beerSensor->setConnected(false);
    fridgeSensorWithFallback->update();
    BOOST_CHECK(fridgeSensorWithFallback->isConnected());

    fridgeSensor->setConnected(false);
    beerSensor->setConnected(true);
    fridgeSensorWithFallback->update();
    BOOST_CHECK(fridgeSensorWithFallback->isConnected());

    fridgeSensor->setConnected(false);
    beerSensor->setConnected(false);
    fridgeSensorWithFallback->update();
    BOOST_CHECK(!fridgeSensorWithFallback->isConnected());
}


BOOST_FIXTURE_TEST_CASE (fallback_sensor_inits_active_sensor, FallbackFixture){
    fridgeSensor->setConnected(false); // init of fridge sensor will return false now
    bool successful_init = fridgeSensorWithFallback->init(); // will return false
    BOOST_CHECK(!successful_init);

    fridgeSensorWithFallback->update(); // this should switch to fallback sensor
    successful_init = fridgeSensorWithFallback->init(); // will return true, because now beer sensor inits successfully

    beerSensor->setConnected(false); // init of beer sensor will return false now too
    successful_init = fridgeSensorWithFallback->init(); // will return false, because now beer sensor init fails
    BOOST_CHECK(!successful_init);
}

BOOST_FIXTURE_TEST_CASE (fallback_sensor_log_messages, FallbackFixture){
    using boost::test_tools::output_test_stream;

    output_test_stream test_stream;
    output = &test_stream; // redirect logger output to test stream
    // last argument to test stream functions sets to flush the stream after the call when true

    fridgeSensorWithFallback->update();
    BOOST_CHECK(test_stream.is_empty());

    fridgeSensor->setConnected(false);
    fridgeSensorWithFallback->update();
    BOOST_CHECK(!test_stream.is_empty(false)); // update has generated a log message
    BOOST_CHECK(test_stream.is_equal("LOG MESSAGE: {W: 4, V: []}\n", true));

    fridgeSensorWithFallback->update();
    BOOST_CHECK(test_stream.is_empty(false)); // no more messages are generated

    fridgeSensor->setConnected(true);
    fridgeSensorWithFallback->update();
    BOOST_CHECK(!test_stream.is_empty(false)); // when sensor comes back, a log message is generated
    BOOST_CHECK(test_stream.is_equal("LOG MESSAGE: {I: 21, V: []}\n", true));

    fridgeSensor->setConnected(true);
    fridgeSensorWithFallback->update();
    BOOST_CHECK(test_stream.is_empty(false)); // but only once

    beerSensor->setConnected(false);
    fridgeSensorWithFallback->update();
    BOOST_CHECK(test_stream.is_empty(false)); // disconnecting the backup sensor does not result in a message

    fridgeSensor->setConnected(false);
    fridgeSensorWithFallback->update();
    BOOST_CHECK(test_stream.is_empty(false)); // When backup is not connected, it is not used for fallback. No messages.

    beerSensor->setConnected(true);
    fridgeSensorWithFallback->update();
    BOOST_CHECK(!test_stream.is_empty(false)); // But when it becomes available, while the main sensor is still unavailable, it is used
    BOOST_CHECK(test_stream.is_equal("LOG MESSAGE: {W: 4, V: []}\n", true)); // and the correct message is logged

    output = &cout; // don't forget to set back to cout, because test stream will go out of scope
}

BOOST_AUTO_TEST_SUITE_END()
