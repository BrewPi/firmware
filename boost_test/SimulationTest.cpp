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

// yes this is hacky, but it allows us to some private variables without adding a lot of getters
#define protected public
#include "Pid.h"
#undef protected

#include "SetPoint.h"
#include <cstdio>
#include <math.h>
#include "TempSensorMock.h"
#include "ActuatorMocks.h"
#include "ActuatorInterfaces.h"
#include "ActuatorPwm.h"
#include "ActuatorTimeLimited.h"
#include "ActuatorSetPoint.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorMutexGroup.h"
#include "runner.h"
#include <iostream>
#include <fstream>

struct StaticSetup{
public:
    StaticSetup(){
        BOOST_TEST_MESSAGE( "setup PID test fixture" );

        beerSensor = new TempSensorMock(20.0);
        fridgeSensor = new TempSensorMock(20.0);

        heaterPin = std::make_shared<ActuatorBool>();
        heaterMutex = std::make_shared<ActuatorMutexDriver>(heaterPin);
        heater = std::make_shared<ActuatorPwm>(heaterMutex, 20); // period 20s, because update steps are 1 second

        coolerPin = std::make_shared<ActuatorBool>();
        coolerTimeLimited = std::make_shared<ActuatorTimeLimited>(coolerPin, 120, 180); // 2 min minOn time, 3 min minOff
        coolerMutex = std::make_shared<ActuatorMutexDriver>(coolerTimeLimited);
        cooler = std::make_shared<ActuatorPwm>(coolerMutex, 1200); // period 20 min
        mutex = new ActuatorMutexGroup();

        beerSet = new SetPointSimple(20.0);
        fridgeSet = new SetPointSimple(20.0);

        heaterPid = new Pid();
        coolerPid = new Pid();
        beerToFridgePid = new Pid();

        fridgeSetPointActuator = std::make_shared<ActuatorSetPoint>(fridgeSet, fridgeSensor, beerSet);

        heaterPid->setOutputActuator(heater);
        coolerPid->setOutputActuator(cooler);
        coolerPid->setActuatorIsNegative(true);
        beerToFridgePid->setOutputActuator(fridgeSetPointActuator);
    }
    ~StaticSetup(){
        BOOST_TEST_MESSAGE( "tear down PID test fixture" );
        delete beerSensor;
        delete fridgeSensor;
        delete mutex;

        delete heaterPid;
        delete coolerPid;
        delete beerToFridgePid;

        delete beerSet;
        delete fridgeSet;

    }

    TempSensorMock * beerSensor;
    TempSensorMock * fridgeSensor;

    std::shared_ptr<ActuatorDigital> coolerPin;
    std::shared_ptr<ActuatorDigital> coolerTimeLimited;
    std::shared_ptr<ActuatorMutexDriver> coolerMutex;
    std::shared_ptr<ActuatorRange> cooler;

    std::shared_ptr<ActuatorDigital> heaterPin;
    std::shared_ptr<ActuatorMutexDriver> heaterMutex;
    std::shared_ptr<ActuatorRange> heater;

    std::shared_ptr<ActuatorSetPoint> fridgeSetPointActuator;

    ActuatorMutexGroup * mutex;

    Pid * heaterPid;
    Pid * coolerPid;
    Pid * beerToFridgePid;

    SetPoint * beerSet;
    SetPoint * fridgeSet;
};

/* This class simulates a fridge is a simple way:
 * There are 3 heat capacities: the beer itself, the air in the fridge and the fridge walls.
 * The heater heats the air in the fridge directly.
 * The cooler cools the fridge walls, which in turn cool the fridge air.
 * This causes an extra delay when cooling and a potential source of overshoot
 */


struct Simulation{
    Simulation(){
        beerTemp = 20.0;
        airTemp = 20.0;
        wallTemp = 20.0;
        envTemp = 20.0;
        heaterTemp = 20.0;

        beerCapacity = 4.2 * 1.0 * 20; // heat capacity water * density of water * 20L volume (in kJ per kelvin).
        airCapacity = 1.005 * 1.225 * 0.200; // heat capacity of dry air * density of air * 200L volume (in kJ per kelvin).
        // Moist air has only slightly higher heat capacity, 1.02 when saturated at 20C.
        wallCapacity = 5.0; // just a guess
        heaterCapacity = 1.0; // also a guess, to simulate that heater first heats itself, then starts heating the air

        heaterPower = 0.1; // 100W, in kW.
        coolerPower = 0.1; // 100W, in kW. Assuming 200W at 50% efficiency

        airBeerTransfer= 1.0/300;
        wallAirTransfer= 1.0/300;
        heaterAirTransfer= 1.0/30;
        envWallTransfer = 0.001; // losses to environment

        heaterToBeer = 0.0; // ratio of heater transfered directly to beer instead of fridge air
        heaterToAir = 1.0 - heaterToBeer;

    }
    virtual ~Simulation(){}

    void update(bool heaterActive, bool coolerActive){
        double beerTempNew = beerTemp;
        double airTempNew = airTemp;
        double wallTempNew = wallTemp;
        double heaterTempNew = heaterTemp;

        beerTempNew += (airTemp - beerTemp) * airBeerTransfer / beerCapacity;

        if(heaterActive){
            heaterTempNew += heaterPower / heaterCapacity;
        }
        if(coolerActive){
            wallTempNew -= coolerPower / wallCapacity;
        }

        airTempNew += (heaterTemp - airTemp) * heaterAirTransfer / airCapacity;
        airTempNew += (wallTemp - airTemp) * wallAirTransfer / airCapacity;
        airTempNew += (beerTemp - airTemp) * airBeerTransfer / airCapacity;


        beerTempNew += (airTemp - beerTemp) * airBeerTransfer / beerCapacity;

        heaterTempNew += (airTemp - heaterTemp) * heaterAirTransfer / heaterCapacity;

        wallTempNew += (envTemp - wallTemp) * envWallTransfer / wallCapacity;
        wallTempNew += (airTemp - wallTemp) * wallAirTransfer/ wallCapacity;

        airTemp = airTempNew;
        beerTemp = beerTempNew;
        wallTemp = wallTempNew;
        heaterTemp = heaterTempNew;
    }

    double beerTemp;
    double airTemp;
    double wallTemp;
    double envTemp;
    double heaterTemp;

    double beerCapacity;
    double airCapacity;
    double wallCapacity;
    double heaterCapacity;

    double heaterPower;
    double coolerPower;

    double airBeerTransfer;
    double wallAirTransfer;
    double envWallTransfer;
    double heaterAirTransfer;

    double heaterToBeer;
    double heaterToAir;
};

/* Below are a few static setups that show how control can be set up.
 * The first 4 are simple: a single actuator, acting on beer or fridge temperature
 */

// Just a heater, acting on beer temperature directly
struct SimBeerHeater : public StaticSetup {
    Simulation sim;
    SimBeerHeater(){
        heaterPid->setInputSensor(beerSensor);
        heaterPid->setSetPoint(beerSet);
        heaterPid->setInputFilter(1);
        heaterPid->setDerivativeFilter(4);
        heaterPid->setConstants(60.0, 7200, 500);

        sim.envTemp = 16.0;
    }

    void update(){
        beerSensor->setTemp(sim.beerTemp);
        fridgeSensor->setTemp(sim.airTemp);
        heaterPid->update();
        heater->update();
        sim.update(heaterPin->isActive(), coolerPin->isActive());
        delay(1000); // simulate actual time passing for pin state and mutex group
    }
};

// Just a heater, acting on fridge temperature directly
struct SimFridgeHeater : public StaticSetup {
    Simulation sim;
    SimFridgeHeater(){
        heaterPid->setInputSensor(fridgeSensor);
        heaterPid->setSetPoint(fridgeSet);
        heaterPid->setInputFilter(1);
        heaterPid->setDerivativeFilter(4);
        heaterPid->setConstants(10.0, 600, 60);

        sim.envTemp = 16.0;
    }

    void update(){
        beerSensor->setTemp(sim.beerTemp);
        fridgeSensor->setTemp(sim.airTemp);
        heaterPid->update();
        heater->update();

        sim.update(heaterPin->isActive(), coolerPin->isActive());
        delay(1000); // simulate actual time passing for pin state and mutex group
    }
};


// Just a cooler, acting on beer temperature directly
struct SimBeerCooler : public StaticSetup {
    Simulation sim;
    SimBeerCooler(){
        coolerPid->setInputSensor(beerSensor);
        coolerPid->setSetPoint(beerSet);
        coolerPid->setInputFilter(2);
        coolerPid->setDerivativeFilter(5);
        coolerPid->setConstants(40.0, 7200, 1200);

        sim.envTemp = 24.0;
    }

    void update(){
        beerSensor->setTemp(sim.beerTemp);
        fridgeSensor->setTemp(sim.airTemp);
        coolerPid->update();
        cooler->update();

        sim.update(heaterPin->isActive(), coolerPin->isActive());
        delay(1000); // simulate actual time passing for pin state of cooler, which is time limited
    }
};

// Just a cooler, acting on fridge temperature directly
struct SimFridgeCooler : public StaticSetup {
    Simulation sim;
    SimFridgeCooler(){
        coolerPid->setInputSensor(fridgeSensor);
        coolerPid->setSetPoint(fridgeSet);
        coolerPid->setInputFilter(1);
        coolerPid->setDerivativeFilter(5);
        coolerPid->setConstants(10.0, 1800, 200);

        sim.envTemp = 24.0;
    }

    void update(){
        beerSensor->setTemp(sim.beerTemp);
        fridgeSensor->setTemp(sim.airTemp);
        coolerPid->update();
        cooler->update();

        sim.update(heaterPin->isActive(), coolerPin->isActive());
        delay(1000); // simulate actual time passing for pin state of cooler, which is time limited
    }
};

// A heater and a cooler, both acting on fridge temperature directly
struct SimFridgeHeaterCooler : public StaticSetup {
    Simulation sim;
    SimFridgeHeaterCooler(){
        coolerPid->setInputSensor(fridgeSensor);
        coolerPid->setSetPoint(fridgeSet);
        coolerPid->setInputFilter(1);
        coolerPid->setDerivativeFilter(4);
        coolerPid->setConstants(10.0, 1800, 200);

        heaterPid->setInputSensor(fridgeSensor);
        heaterPid->setSetPoint(fridgeSet);
        heaterPid->setInputFilter(1);
        heaterPid->setDerivativeFilter(4);
        heaterPid->setConstants(10.0, 1800, 60);

        coolerMutex->setMutex(mutex);
        heaterMutex->setMutex(mutex);
        mutex->setDeadTime(3600000); // 60 minutes
    }

    void update(){
        beerSensor->setTemp(sim.beerTemp);
        fridgeSensor->setTemp(sim.airTemp);
        heaterPid->update();
        coolerPid->update();
        cooler->update();
        heater->update();
        mutex->update();

        sim.update(heaterPin->isActive(), coolerPin->isActive());
        delay(1000); // simulate actual time passing for pin state of cooler, which is time limited
    }
};

// A heater and a cooler, both acting on beer temperature directly
struct SimBeerHeaterCooler : public StaticSetup {
    Simulation sim;
    SimBeerHeaterCooler(){
        coolerPid->setInputSensor(beerSensor);
        coolerPid->setSetPoint(beerSet);
        coolerPid->setInputFilter(1);
        coolerPid->setDerivativeFilter(4);
        coolerPid->setConstants(40.0, 7200, 1200);

        heaterPid->setInputSensor(beerSensor);
        heaterPid->setSetPoint(beerSet);
        heaterPid->setInputFilter(1);
        heaterPid->setDerivativeFilter(4);
        heaterPid->setConstants(60.0, 7200, 500);

        coolerMutex->setMutex(mutex);
        heaterMutex->setMutex(mutex);
        mutex->setDeadTime(3600000); // 60 minutes
    }

    void update(){
        beerSensor->setTemp(sim.beerTemp);
        fridgeSensor->setTemp(sim.airTemp);
        heaterPid->update();
        coolerPid->update();
        cooler->update();
        heater->update();
        mutex->update();

        sim.update(heaterPin->isActive(), coolerPin->isActive());
        delay(1000); // simulate actual time passing for pin state of cooler, which is time limited
    }
};

// A heater and a cooler, both acting on fridge temperature directly
struct SimCascadedHeaterCooler : public StaticSetup {
    Simulation sim;
    SimCascadedHeaterCooler(){
        coolerPid->setInputSensor(fridgeSensor);
        coolerPid->setSetPoint(fridgeSet);
        coolerPid->setInputFilter(1);
        coolerPid->setDerivativeFilter(4);
        coolerPid->setConstants(10.0, 1800, 200);

        heaterPid->setInputSensor(fridgeSensor);
        heaterPid->setSetPoint(fridgeSet);
        heaterPid->setInputFilter(1);
        heaterPid->setDerivativeFilter(4);
        heaterPid->setConstants(10.0, 600, 60);


        beerToFridgePid->setInputSensor(beerSensor);
        beerToFridgePid->setSetPoint(beerSet);
        beerToFridgePid->setInputFilter(1);
        beerToFridgePid->setDerivativeFilter(4);
        beerToFridgePid->setConstants(2.0, 7200, 1200);
        fridgeSetPointActuator->setMin(-10.0);
        fridgeSetPointActuator->setMax(10.0);

        coolerMutex->setMutex(mutex);
        heaterMutex->setMutex(mutex);
        mutex->setDeadTime(3600000); // 60 minutes
    }

    void update(){
        beerSensor->setTemp(sim.beerTemp);
        fridgeSensor->setTemp(sim.airTemp);
        heaterPid->update();
        coolerPid->update();
        beerToFridgePid->update();
        cooler->update();
        heater->update();
        fridgeSetPointActuator->update();
        mutex->update();

        sim.update(heaterPin->isActive(), coolerPin->isActive());
        delay(1000); // simulate actual time passing for pin state of cooler, which is time limited
    }
};


BOOST_AUTO_TEST_SUITE(simulation_test)



// Test heating fridge air based on beer temperature (non-cascaded control)
BOOST_FIXTURE_TEST_CASE(Simulate_Air_Heater_Acts_On_Beer, SimBeerHeater)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#beer setPoint, 2#error, 1#beer sensor, 1#fridge air sensor, 1#fridge wall temp, "
            "3#heater pwm, 3#heater achieved pwm, 4#p, 4#i, 4#d" << endl;
    double SetPointDouble = 19;
    for(int t = 0; t < 60000; t++){
        if(t==1000){
            SetPointDouble = 21;
        }
        if(t==8000){
            SetPointDouble = 24;
        }
        beerSet->write(SetPointDouble);
        update();

        csv     << beerSet->read() << "," // setpoint
                << heaterPid->inputError << "," //error
                << beerSensor->read() << "," // beer temp
                << fridgeSensor->read() << "," // air temp
                << sim.wallTemp << "," // fridge wall temperature
                << heater->getValue() << "," // actuator output
                << heater->readValue() << "," // achieved  output
                << heaterPid->p << "," // proportional action
                << heaterPid->i << "," // integral action
                << heaterPid->d // derivative action
                << endl;
    }
    csv.close();
}

// Test heating fridge air based on fridge air temperature (non-cascaded control)
BOOST_FIXTURE_TEST_CASE(Simulate_Air_Heater_Acts_On_Fridge_Air, SimFridgeHeater)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#fridge setPoint, 2#error, 1#beer sensor, 1#fridge air sensor, 1#fridge wall temp, "
            "3#heater pwm, 3#heater achieved pwm, 4#p, 4#i, 4#d" << endl;
    double SetPointDouble = 19;
    for(int t = 0; t < 20000; t++){
        if(t==1000){
            SetPointDouble = 24;
        }
        if(t==8000){
            SetPointDouble = 28;
        }
        fridgeSet->write(SetPointDouble);
        update();

        csv     << fridgeSet->read() << "," // setpoint
                << heaterPid->inputError << "," //error
                << beerSensor->read() << "," // beer temp
                << fridgeSensor->read() << "," // air temp
                << sim.wallTemp << "," // fridge wall temperature
                << heater->getValue() << "," // actuator output
                << heater->readValue() << "," // achieved output
                << heaterPid->p << "," // proportional action
                << heaterPid->i << "," // integral action
                << heaterPid->d // derivative action
                << endl;
    }
    csv.close();
}

// Test cooling fridge air (via wall) based on beer temperature (non-cascaded control)
BOOST_FIXTURE_TEST_CASE(Simulate_Air_Cooler_Acts_On_Beer, SimBeerCooler)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#setPoint, 2#error, 1#beer sensor, 1#fridge air sensor, 1#fridge wall temp, "
            "3#cooler pwm, 3#cooler achieved pwm, 4#p, 4#i, 4#d, 5a#cooler pin" << endl;
    double SetPointDouble = 21;
    for(int t = 0; t < 30000; t++){
        if(t==1000){
            SetPointDouble = 19;
        }
        if(t==15000){
            SetPointDouble = 18.5;
        }
        beerSet->write(SetPointDouble);
        update();

        csv     << beerSet->read() << "," // setpoint
                << coolerPid->inputError << "," //error
                << beerSensor->read() << "," // beer temp
                << fridgeSensor->read() << "," // air temp
                << sim.wallTemp << "," // fridge wall temperature
                << cooler->getValue() << "," // actuator output
                << cooler->readValue() << "," // achieved output
                << coolerPid->p << "," // proportional action
                << coolerPid->i << "," // integral action
                << coolerPid->d << "," // derivative action
                << coolerPin->isActive() // actual cooler pin state
                << endl;
    }
    csv.close();
}

// Test cooling fridge air (via wall) based on fridge air temperature (non-cascaded control)
BOOST_FIXTURE_TEST_CASE(Simulate_Air_Cooler_Acts_On_Fridge_Air, SimFridgeCooler)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#setPoint, 2#error, 1#beer sensor, 1#fridge air sensor, 1#fridge wall temp, "
            "3#cooler pwm, 3#cooler achieved pwm, 4#p, 4#i, 4#d, 5a#cooler pin" << endl;
    double SetPointDouble = 21;
    for(int t = 0; t < 20000; t++){
        if(t==1000){
            SetPointDouble = 19;
        }
        if(t==8000){
            SetPointDouble = 15;
        }
        fridgeSet->write(SetPointDouble);
        update();

        csv     << fridgeSet->read() << "," // setpoint
                << coolerPid->inputError << "," //error
                << beerSensor->read() << "," // beer temp
                << fridgeSensor->read() << "," // air temp
                << sim.wallTemp << "," // fridge wall temperature
                << cooler->getValue() << "," // actuator output
                << cooler->readValue() << "," // achieved output
                << coolerPid->p << "," // proportional action
                << coolerPid->i << "," // integral action
                << coolerPid->d << "," // derivative action
                << coolerPin->isActive() // actual cooler pin state
                << endl;
    }
    csv.close();
}

// Test heating and cooling fridge air based on fridge air temperature (non-cascaded control)
BOOST_FIXTURE_TEST_CASE(Simulate_Air_Heater_And_Cooler_Acting_On_Fridge_Air, SimFridgeHeaterCooler)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#beer setPoint, 2#error, 1#beer sensor, 1#fridge air sensor, 1#fridge wall temp, "
                "4#cooler pwm, 4#cooler achieved pwm, 3#cooler P, 3#cooler I, 3#cooler D, "
                "4#heater pwm, 4#heater achieved pwm, 3#heater P, 3#heater I, 3#heater D, "
                "5a#cooler pin, 5a#heater pin" << endl;
    double SetPointDouble = 20;
    for(int t = 0; t < 40000; t++){
        if(t==1000){
            SetPointDouble = 19;
        }
        if(t > 8000 && t < 16000){
            SetPointDouble -= 0.0005; // ramp down slowly
        }

        if(t > 20000 && t < 28000){
            SetPointDouble -= 0.0005; // ramp up slowly
        }

        fridgeSet->write(SetPointDouble);

        update();

        BOOST_CHECK_MESSAGE(! (heaterPin->isActive() && coolerPin->isActive()), "at " << t); // pins are not active at the same time

        csv     << fridgeSet->read() << "," // setpoint
                << coolerPid->inputError << "," //error
                << beerSensor->read() << "," // beer temp
                << fridgeSensor->read() << "," // air temp
                << sim.wallTemp << "," // fridge wall temperature
                << cooler->getValue() << "," // actuator output
                << cooler->readValue() << "," // achieved output
                << coolerPid->p << "," // proportional action
                << coolerPid->i << "," // integral action
                << coolerPid->d << "," // derivative action
                << heater->getValue() << "," // actuator output
                << heater->readValue() << "," // achieved output
                << heaterPid->p << "," // proportional action
                << heaterPid->i << "," // integral action
                << heaterPid->d << "," // derivative action
                << coolerPin->isActive() << "," // actual cooler pin state
                << heaterPin->isActive() // actual cooler pin state
                << endl;
    }
    csv.close();
}

// Test heating and cooling fridge air based on fridge air temperature (non-cascaded control)
BOOST_FIXTURE_TEST_CASE(Simulate_Air_Heater_And_Cooler_Acting_On_Beer, SimBeerHeaterCooler)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#beer setPoint, 2#error, 1#beer sensor, 1#fridge air sensor, 1#fridge wall temp, "
            "4#cooler pwm, 4#cooler achieved pwm, 3#cooler P, 3#cooler I, 3#cooler D, "
            "4#heater pwm, 4#heater achieved pwm, 3#heater P, 3#heater I, 3#heater D, "
            "5a#cooler pin, 5a#heater pin" << endl;
    double SetPointDouble = 20;
    for(int t = 0; t < 40000; t++){
        if(t==1000){
            SetPointDouble = 19;
        }
        if(t > 8000 && t < 16000){
            SetPointDouble -= 0.0001; // ramp down slowly
        }

        if(t > 20000 && t < 28000){
            SetPointDouble -= 0.0001; // ramp up slowly
        }

        beerSet->write(SetPointDouble);
        update();

        BOOST_CHECK( !(heaterPin->isActive() && coolerPin->isActive()) ); // pins are not active at the same time

        csv     << beerSet->read() << "," // setpoint
                << coolerPid->inputError << "," //error
                << beerSensor->read() << "," // beer temp
                << fridgeSensor->read() << "," // air temp
                << sim.wallTemp << "," // fridge wall temperature
                << cooler->getValue() << "," // actuator output
                << cooler->readValue() << "," // achieved output
                << coolerPid->p << "," // proportional action
                << coolerPid->i << "," // integral action
                << coolerPid->d << "," // derivative action
                << heater->getValue() << "," // actuator output
                << heater->readValue() << "," // achieved output
                << heaterPid->p << "," // proportional action
                << heaterPid->i << "," // integral action
                << heaterPid->d << "," // derivative action
                << coolerPin->isActive() << "," // actual cooler pin state
                << heaterPin->isActive() // actual cooler pin state
                << endl;
    }
    csv.close();
}

// Test heating and cooling fridge air based on fridge air temperature (non-cascaded control)
BOOST_FIXTURE_TEST_CASE(Simulate_Cascaded_Control, SimCascadedHeaterCooler)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#beer setpoint, 1#beer sensor, 2#beer error, "
           "3#b2f P, 3#b2f I, 3#b2f D, 3#b2f PID, 3#b2f actual,"
           "4#fridge setpoint, 4#fridge air sensor, 4#fridge wall temp, "
           "7#cooler pwm, 7# cooler achieved pwm, 5#cooler P, 5#cooler I, 5#cooler D, "
           "7#heater pwm, 7# heater achieved pwm, 6#heater P, 6#heater I, 6#heater D, "
           "8a#cooler pin, 8a#heater pin" << endl;
    double SetPointDouble = 20;
    for(int t = 0; t < 50000; t++){
        if(t==1000){
            SetPointDouble = 19;
        }

        if(t > 8000 && t < 16000){
            SetPointDouble -= 0.0001; // ramp down slowly
        }

        if(t > 20000 && t < 28000){
            SetPointDouble -= 0.0001; // ramp up slowly
        }

        beerSet->write(SetPointDouble);
        update();

        BOOST_CHECK( !(heaterPin->isActive() && coolerPin->isActive()) ); // pins are not active at the same time

        csv     << beerSet->read() << "," // setpoint
                << beerSensor->read() << "," // beer temp
                << beerToFridgePid->inputError << "," // beer error

                << beerToFridgePid->p << "," // proportional action
                << beerToFridgePid->i << "," // integral action
                << beerToFridgePid->d << "," // derivative action
                << beerToFridgePid->p + beerToFridgePid->i + beerToFridgePid->d << "," // PID output
                << fridgeSetPointActuator->getValue() << "," // beer-fridge actual difference

                << fridgeSet->read() << "," // fridge setpoint
                << fridgeSensor->read() << "," // air temp
                << sim.wallTemp << "," // fridge wall temperature

                << cooler->getValue() << "," // actuator output
                << cooler->readValue() << "," // achieved output
                << coolerPid->p << "," // proportional action
                << coolerPid->i << "," // integral action
                << coolerPid->d << "," // derivative action

                << heater->getValue() << "," // actuator output
                << heater->readValue() << "," // achieved output
                << heaterPid->p << "," // proportional action
                << heaterPid->i << "," // integral action
                << heaterPid->d << "," // derivative action

                << coolerPin->isActive() << "," // actual cooler pin state
                << heaterPin->isActive() // actual cooler pin state
                << endl;
    }
    csv.close();
}


// Test heating and cooling fridge air based on just a glass of water as beer (cascaded control)
BOOST_FIXTURE_TEST_CASE(Simulate_Cascaded_Cool_Small_Volume, SimCascadedHeaterCooler)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#beer setpoint, 1#beer sensor, 2#beer error, "
           "3#b2f P, 3#b2f I, 3#b2f D, 3#b2f PID, 3#b2f actual,"
           "4#fridge setpoint, 4#fridge air sensor, 4#fridge wall temp, "
           "7#cooler pwm, 7# cooler achieved pwm, 5#cooler P, 5#cooler I, 5#cooler D, "
           "7#heater pwm, 7# heater achieved pwm, 6#heater P, 6#heater I, 6#heater D, "
           "8a#cooler pin, 8a#heater pin" << endl;

    sim.beerCapacity = 4.2 * 1.0 * 0.2; // heat capacity water * density of water * 0.2L volume (in kJ per kelvin).
    sim.airBeerTransfer = 0.001;

    beerToFridgePid->setConstants(0.5, 1800, 180);

    for(int t = 0; t < 10000; t++){
        if(t==2000){
            beerSet->write(5.0);
        }

        if(t==6000){
            beerSet->write(5.0);
        }

        update();

        BOOST_CHECK( !(heaterPin->isActive() && coolerPin->isActive()) ); // pins are not active at the same time

        csv     << beerSet->read() << "," // setpoint
                << beerSensor->read() << "," // beer temp
                << beerToFridgePid->inputError << "," // beer error

                << beerToFridgePid->p << "," // proportional action
                << beerToFridgePid->i << "," // integral action
                << beerToFridgePid->d << "," // derivative action
                << beerToFridgePid->p + beerToFridgePid->i + beerToFridgePid->d << "," // PID output
                << fridgeSetPointActuator->getValue() << "," // beer-fridge actual difference

                << fridgeSet->read() << "," // fridge setpoint
                << fridgeSensor->read() << "," // air temp
                << sim.wallTemp << "," // fridge wall temperature

                << cooler->getValue() << "," // actuator output
                << cooler->readValue() << "," // achieved output
                << coolerPid->p << "," // proportional action
                << coolerPid->i << "," // integral action
                << coolerPid->d << "," // derivative action

                << heater->getValue() << "," // actuator output
                << heater->readValue() << "," // achieved output
                << heaterPid->p << "," // proportional action
                << heaterPid->i << "," // integral action
                << heaterPid->d << "," // derivative action

                << coolerPin->isActive() << "," // actual cooler pin state
                << heaterPin->isActive() // actual cooler pin state
                << endl;
    }
    csv.close();
}

BOOST_AUTO_TEST_SUITE_END()
