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
    TempSensorBasic * s = new TempSensorFallback();
    temp_t t = s->read();

    BOOST_CHECK_EQUAL(t, temp_t::invalid());
}


BOOST_FIXTURE_TEST_CASE (fallback_sensor_returns_main_sensor_value, FallbackFixture){
    temp_t t = fridgeSensorWithFallback->read();
    BOOST_CHECK_EQUAL(t, temp_t(initialFridgeTemp));
}


BOOST_FIXTURE_TEST_CASE (fallback_sensor_returns_backup_when_main_is_invalid, FallbackFixture){
    temp_t t = fridgeSensorWithFallback->read();
    BOOST_CHECK_EQUAL(t, temp_t(initialFridgeTemp));

    fridgeSensor->setConnected(false);

    t = fridgeSensorWithFallback->read();

    BOOST_CHECK_EQUAL(t, temp_t(initialBeerTemp));
}


BOOST_FIXTURE_TEST_CASE (fallback_sensor_is_connected_when_main_or_backup_is_connected, FallbackFixture){
    fridgeSensor->setConnected(true);
    beerSensor->setConnected(true);
    BOOST_CHECK(fridgeSensorWithFallback->isConnected());

    fridgeSensor->setConnected(true);
    beerSensor->setConnected(false);
    BOOST_CHECK(fridgeSensorWithFallback->isConnected());

    fridgeSensor->setConnected(false);
    beerSensor->setConnected(true);
    BOOST_CHECK(fridgeSensorWithFallback->isConnected());

    fridgeSensor->setConnected(false);
    beerSensor->setConnected(false);
    BOOST_CHECK(!fridgeSensorWithFallback->isConnected());
}


BOOST_FIXTURE_TEST_CASE (fallback_sensor_init_falls_back_on_backup_sensor, FallbackFixture){
    fridgeSensor->setConnected(false); // init of fridge sensor will return false
    bool successful_init = fridgeSensorWithFallback->init(); // will return true, because backup inits successfully
    BOOST_CHECK(successful_init);

    beerSensor->setConnected(false); // init of beer sensor will return false now too
    successful_init = fridgeSensorWithFallback->init(); // will return false, because now both sensors are invalid
    BOOST_CHECK(!successful_init);
}

BOOST_AUTO_TEST_SUITE_END()
