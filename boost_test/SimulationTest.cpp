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

#include "Pid.h"
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

        heaterPin = new ActuatorBool();
        heaterMutex = new ActuatorMutexDriver(heaterPin);
        heater = new ActuatorPwm(heaterMutex, 4); // period 4s

        coolerPin = new ActuatorBool();
        coolerTimeLimited = new ActuatorTimeLimited(coolerPin, 120, 180); // 2 min minOn time, 3 min minOff
        coolerMutex = new ActuatorMutexDriver(coolerTimeLimited);
        cooler = new ActuatorPwm(coolerMutex, 600); // period 10 min
        mutex = new ActuatorMutexGroup();

        beerSet = new SetPointSimple(20.0);
        fridgeSet = new SetPointSimple(20.0);

        heaterPid = new Pid();
        coolerPid = new Pid();
        beerToFridgePid = new Pid();

        fridgeSetPointActuator = new ActuatorSetPoint(fridgeSet, beerSet);

        heaterPid->setOutputActuator(heater);
        coolerPid->setOutputActuator(cooler);
        coolerPid->setActuatorIsNegative(true);
    }
    ~StaticSetup(){
        BOOST_TEST_MESSAGE( "tear down PID test fixture" );
        delete beerSensor;
        delete fridgeSensor;

        delete coolerPin;
        delete coolerTimeLimited;
        delete coolerMutex;
        delete cooler;

        delete heaterPin;
        delete heaterMutex;
        delete heater;

        delete fridgeSetPointActuator;

        delete mutex;

        delete heaterPid;
        delete coolerPid;
        delete beerToFridgePid;

        delete beerSet;
        delete fridgeSet;

    }

    TempSensorMock * beerSensor;
    TempSensorMock * fridgeSensor;

    ActuatorDigital * coolerPin;
    ActuatorDigital * coolerTimeLimited;
    ActuatorMutexDriver * coolerMutex;
    ActuatorRange * cooler;

    ActuatorDigital * heaterPin;
    ActuatorMutexDriver * heaterMutex;
    ActuatorRange * heater;

    ActuatorRange * fridgeSetPointActuator;

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
        envTemp = 18.0;

        beerCapacity = 4.2 * 1.0 * 20; // heat capacity water * density of water * 20L volume (in kJ per kelvin).
        airCapacity = 1.005 * 1.225 * 0.200; // 2 * heat capacity of dry air * density of air * 200L volume (in kJ per kelvin).
        // Moist air has only slightly higher heat capacity, 1.02 when saturated at 20C.
        wallCapacity = 5.0; // just a guess

        heaterPower = 0.1; // 100W, in kW.
        coolerPower = 0.1; // 100W, in kW. Assuming 200W at 50% efficiency

        airBeerTransfer= 0.01;
        wallAirTransfer= 0.02;
        envWallTransfer = 0.005; // losses to environment

    }
    virtual ~Simulation(){}

    void update(temp_t heaterValue, temp_t coolerValue){
        double beerTempNew = beerTemp;
        double airTempNew = airTemp;
        double wallTempNew = wallTemp;

        beerTempNew += (airTemp - beerTemp) * airBeerTransfer / beerCapacity;

        airTempNew += heaterPower * double(heaterValue) / (100.0 * airCapacity);
        airTempNew += (wallTemp - airTemp) * wallAirTransfer / airCapacity;
        airTempNew += (beerTemp - airTemp) * airBeerTransfer / airCapacity;

        wallTempNew -= coolerPower * double(coolerValue) / (100.0 * wallCapacity);
        wallTempNew += (envTemp - wallTemp) * envWallTransfer / wallCapacity;
        wallTempNew += (airTemp - wallTemp) * wallAirTransfer/ wallCapacity;

        airTemp = airTempNew;
        beerTemp = beerTempNew;
        wallTemp = wallTempNew;
    }

    double beerTemp;
    double airTemp;
    double wallTemp;
    double envTemp;

    double beerCapacity;
    double airCapacity;
    double wallCapacity;

    double heaterPower;
    double coolerPower;

    double airBeerTransfer;
    double wallAirTransfer;
    double envWallTransfer;
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
        heaterPid->setInputFilter(0);
        heaterPid->setDerivativeFilter(4);
        heaterPid->setConstants(100.0, 2400, 180);
    }

    void update(){
        sim.update(heater->getValue(), cooler->getValue());
        beerSensor->setTemp(sim.beerTemp);
        fridgeSensor->setTemp(sim.airTemp);
        heaterPid->update();
    }
};

// Just a heater, acting on fridge temperature directly
struct SimFridgeHeater : public StaticSetup {
    Simulation sim;
    SimFridgeHeater(){
        heaterPid->setInputSensor(fridgeSensor);
        heaterPid->setSetPoint(fridgeSet);
        heaterPid->setInputFilter(0);
        heaterPid->setDerivativeFilter(2);
        heaterPid->setConstants(5.0, 120, 0);
    }

    void update(){
        sim.update(heater->getValue(), cooler->getValue());
        beerSensor->setTemp(sim.beerTemp);
        fridgeSensor->setTemp(sim.airTemp);
        heaterPid->update();
    }
};


// Just a cooler, acting on beer temperature directly
struct SimBeerCooler : public StaticSetup {
    Simulation sim;
    SimBeerCooler(){
        coolerPid->setInputSensor(beerSensor);
        coolerPid->setSetPoint(beerSet);
        coolerPid->setInputFilter(4);
        coolerPid->setDerivativeFilter(4);
        coolerPid->setConstants(50.0, 7200, 800);
    }

    void update(){
        beerSensor->setTemp(sim.beerTemp);
        fridgeSensor->setTemp(sim.airTemp);
        coolerPid->update();
        cooler->update();
        // because cooler is time limited, get actual output from pin
        if(coolerPin->isActive()){
            sim.update(heater->getValue(), 100.0);
        } else{
            sim.update(heater->getValue(), 0.0);
        }
        delay(1000); // simulate actual time passing for pin state of cooler, which is time limited
    }
};

// Just a cooler, acting on fridge temperature directly
struct SimFridgeCooler : public StaticSetup {
    Simulation sim;
    SimFridgeCooler(){
        coolerPid->setInputSensor(fridgeSensor);
        coolerPid->setSetPoint(fridgeSet);
        coolerPid->setInputFilter(0);
        coolerPid->setDerivativeFilter(4);
        coolerPid->setConstants(3.0, 600, 60);
    }

    void update(){
        beerSensor->setTemp(sim.beerTemp);
        fridgeSensor->setTemp(sim.airTemp);
        coolerPid->update();
        cooler->update();

        // because cooler is time limited, get actual output from pin
        if(coolerPin->isActive()){
            sim.update(heater->getValue(), 100.0);
        } else{
            sim.update(heater->getValue(), 0.0);
        }
        delay(1000); // simulate actual time passing for pin state of cooler, which is time limited
    }
};

// A heater and a cooler, both acting on fridge temperature directly
struct SimFridgeHeaterCooler : public StaticSetup {
    Simulation sim;
    SimFridgeHeaterCooler(){
        coolerPid->setInputSensor(fridgeSensor);
        coolerPid->setSetPoint(fridgeSet);
        coolerPid->setInputFilter(0);
        coolerPid->setDerivativeFilter(4);
        coolerPid->setConstants(3.0, 600, 60);

        heaterPid->setInputSensor(fridgeSensor);
        heaterPid->setSetPoint(fridgeSet);
        heaterPid->setInputFilter(0);
        heaterPid->setDerivativeFilter(4);
        heaterPid->setConstants(5.0, 120, 0);

        coolerMutex->setMutex(mutex);
        heaterMutex->setMutex(mutex);
        mutex->setDeadTime(1800000); // 30 minutes
    }

    void update(){
        beerSensor->setTemp(sim.beerTemp);
        fridgeSensor->setTemp(sim.airTemp);
        heaterPid->update();
        coolerPid->update();
        cooler->update();
        heater->update();

        // because cooler is time limited, get actual output from pin
        if(coolerPin->isActive()){
            sim.update(heater->getValue(), 100.0);
        } else{
            sim.update(heater->getValue(), 0.0);
        }
        delay(1000); // simulate actual time passing for pin state of cooler, which is time limited
    }
};

// A heater and a cooler, both acting on fridge temperature directly
struct SimBeerHeaterCooler : public StaticSetup {
    Simulation sim;
    SimBeerHeaterCooler(){
        coolerPid->setInputSensor(beerSensor);
        coolerPid->setSetPoint(beerSet);
        coolerPid->setInputFilter(4);
        coolerPid->setDerivativeFilter(4);
        coolerPid->setConstants(50.0, 7200, 800);

        heaterPid->setInputSensor(beerSensor);
        heaterPid->setSetPoint(beerSet);
        heaterPid->setInputFilter(4);
        heaterPid->setDerivativeFilter(4);
        heaterPid->setConstants(100.0, 2400, 180);

        coolerMutex->setMutex(mutex);
        heaterMutex->setMutex(mutex);
        mutex->setDeadTime(1800000); // 30 minutes
    }

    void update(){
        beerSensor->setTemp(sim.beerTemp);
        fridgeSensor->setTemp(sim.airTemp);
        heaterPid->update();
        coolerPid->update();
        cooler->update();
        heater->update();

        // because cooler is time limited, get actual output from pin
        if(coolerPin->isActive()){
            sim.update(heater->getValue(), 100.0);
        } else{
            sim.update(heater->getValue(), 0.0);
        }
        delay(1000); // simulate actual time passing for pin state of cooler, which is time limited
    }
};


BOOST_AUTO_TEST_SUITE( simulation_test)



// Test heating fridge air based on beer temperature (non-cascaded control)
BOOST_FIXTURE_TEST_CASE(Simulate_Air_Heater_Acts_On_Beer, SimBeerHeater)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "setPoint, error, beer sensor, fridge air sensor, fridge wall temp, heater pwm, p, i, d" << endl;
    double SetPointDouble = 19;
    for(int t = 0; t < 20000; t++){
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
    csv << "setPoint, error, beer sensor, fridge air sensor, fridge wall temp, heater pwm, p, i, d" << endl;
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
    csv << "setPoint, error, beer sensor, fridge air sensor, fridge wall temp, cooler pwm, p, i, d, cooler pin" << endl;
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
        if(t==7000){
            update();
        }

        csv     << beerSet->read() << "," // setpoint
                << coolerPid->inputError << "," //error
                << beerSensor->read() << "," // beer temp
                << fridgeSensor->read() << "," // air temp
                << sim.wallTemp << "," // fridge wall temperature
                << cooler->getValue() << "," // actuator output
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
    csv << "setPoint, error, beer sensor, fridge air sensor, fridge wall temp, cooler pwm, p, i, d, cooler pin" << endl;
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
    csv << "setPoint, error, beer sensor, fridge air sensor, fridge wall temp, "
            "cooler pwm, cooler P, cooler I, cooler D, "
            "heater pwm, heater P, heater I, heater D, "
            "heater pin, cooler pin" << endl;
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
                << coolerPid->p << "," // proportional action
                << coolerPid->i << "," // integral action
                << coolerPid->d << "," // derivative action
                << heater->getValue() << "," // actuator output
                << heaterPid->p << "," // proportional action
                << heaterPid->i << "," // integral action
                << heaterPid->d << "," // derivative action
                << heaterPin->isActive() << "," // actual cooler pin state
                << coolerPin->isActive() // actual cooler pin state
                << endl;
    }
    csv.close();
}

// Test heating and cooling fridge air based on fridge air temperature (non-cascaded control)
BOOST_FIXTURE_TEST_CASE(Simulate_Air_Heater_And_Cooler_Acting_On_Beer, SimFridgeHeaterCooler)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "setPoint, error, beer sensor, fridge air sensor, fridge wall temp, "
            "cooler pwm, cooler P, cooler I, cooler D, "
            "heater pwm, heater P, heater I, heater D, "
            "heater pin, cooler pin" << endl;
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

        if((heaterPin->isActive() && coolerPin->isActive()) ){
            update();
        }

        BOOST_CHECK( !(heaterPin->isActive() && coolerPin->isActive()) ); // pins are not active at the same time

        csv     << beerSet->read() << "," // setpoint
                << coolerPid->inputError << "," //error
                << beerSensor->read() << "," // beer temp
                << fridgeSensor->read() << "," // air temp
                << sim.wallTemp << "," // fridge wall temperature
                << cooler->getValue() << "," // actuator output
                << coolerPid->p << "," // proportional action
                << coolerPid->i << "," // integral action
                << coolerPid->d << "," // derivative action
                << heater->getValue() << "," // actuator output
                << heaterPid->p << "," // proportional action
                << heaterPid->i << "," // integral action
                << heaterPid->d << "," // derivative action
                << heaterPin->isActive() << "," // actual cooler pin state
                << coolerPin->isActive() // actual cooler pin state
                << endl;
    }
    csv.close();
}



BOOST_AUTO_TEST_SUITE_END()
