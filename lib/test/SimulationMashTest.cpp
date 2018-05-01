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

#include "temperatureFormats.h"
#include "SetPoint.h"
#include <cstdio>
#include <math.h>
#include "TempSensorMock.h"
#include "ActuatorMocks.h"
#include "ActuatorInterfaces.h"
#include "ActuatorPwm.h"
#include "ActuatorTimeLimited.h"
#include "ActuatorOffset.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorMutexGroup.h"
#include "ProcessValueDelegate.h"
#include "SensorSetPointPair.h"
#include "runner.h"
#include <iostream>
#include <fstream>

struct MashStaticSetup{
public:
    MashStaticSetup() :
        mashSensor(20.0),
        hltSensor(20.0),
        hltHeaterPin(),
        mutex(),
        hltHeaterMutex(hltHeaterPin, mutex),
        hltHeater(hltHeaterMutex, 10), // period 10s, because we're skipping fastUpdate() here
        hltSet(20.0),
        mashSet(20.0),
        hlt(hltSensor, hltSet),
        mash(mashSensor, mashSet),
        hltOffsetActuator(hlt, mash),

        hltHeaterInputLookup(&hlt),
        hltHeaterOutLookup(&hltHeater),
        mashToHltInputLookup(&mash),
        mashToHltOutputLookup(&hltOffsetActuator),

        hltHeaterPidInput(hltHeaterInputLookup),
        hltHeaterPidOutput(hltHeaterOutLookup),
        mashToHltPidInput(mashToHltInputLookup),
        mashToHltPidOutput(mashToHltOutputLookup),

        hltHeaterPid(hltHeaterPidInput, hltHeaterPidOutput),
        mashToHltPid(mashToHltPidInput, mashToHltPidOutput)
    {
        BOOST_TEST_MESSAGE( "setup mash test fixture" );
    }
    ~MashStaticSetup(){
        BOOST_TEST_MESSAGE( "tear down mash test fixture" );
    }

    void update(){
        hltHeaterPid.update();
        mashToHltPid.update();

        hltOffsetActuator.update();
        hltHeater.update();
        mutex.update();
    }

    TempSensorMock mashSensor;
    TempSensorMock hltSensor;
    ActuatorBool hltHeaterPin;
    ActuatorMutexGroup mutex;
    ActuatorMutexDriver hltHeaterMutex;
    ActuatorPwm hltHeater;
    SetPointSimple hltSet;
    SetPointSimple mashSet;
    SensorSetPointPair hlt;
    SensorSetPointPair mash;
    ActuatorOffset hltOffsetActuator;

    PtrLookup hltHeaterInputLookup;
    PtrLookup hltHeaterOutLookup;
    PtrLookup mashToHltInputLookup;
    PtrLookup mashToHltOutputLookup;

    ProcessValueDelegate hltHeaterPidInput;
    ProcessValueDelegate hltHeaterPidOutput;
    ProcessValueDelegate mashToHltPidInput;
    ProcessValueDelegate mashToHltPidOutput;

    Pid hltHeaterPid;
    Pid mashToHltPid;
};

/* This class simulates a mashing process.
 * There are 3 heat capacities: the beer itself, the air in the fridge and the fridge walls.
 * The heater heats the air in the fridge directly.
 * The cooler cools the fridge walls, which in turn cool the fridge air.
 * This causes an extra delay when cooling and a potential source of overshoot
 */


struct MashSimulation{
    MashSimulation(){
        mashTemp = 60.0;
        hltTemp = 60.0;
        envTemp = 20.0;
        coilInTemp = 20;
        coilOutTemp = 20;
        mashInTemp = 20;

        // typical for a 40 liter batch and a 45cm kettle
        mashVolume = 28;
        hltVolume = 28;

        specificHeat = 4.2;
        hltCapacity = specificHeat * 1.0 * hltVolume; // heat capacity water * density of water * 20L volume (in kJ per degree C).
        mashCapacity = specificHeat * 1.0 * mashVolume;

        hltHeaterPower = 3.2; // 3200W, in kW.

        coilTransfer = 0.6; // ratio of temperature difference picked up in HLT coil
        flowRate = 8.0/60; // 8 liter per minute, in L/s.
        kettleEnvTransfer = 0.01; // 10W per degree
        mashToCoilLoss = 0.01; // losses between mash tun and coil
        coilToMashLoss = 0.01; // losses between mash tun and coil

        mashPumping = true;
    }

    virtual ~MashSimulation(){}

    void update(temp_t heaterValue){
        double mashTempNew = mashTemp;
        double hltTempNew = hltTemp;

        if(mashPumping){
            coilInTemp = mashTemp - (mashTemp - envTemp) * mashToCoilLoss;
            coilOutTemp = coilInTemp + (hltTemp - coilInTemp) * coilTransfer;
            mashInTemp = coilOutTemp - (coilOutTemp - envTemp) * coilToMashLoss;

            // coil transfer
            mashTempNew = (mashTemp * (mashVolume - flowRate) + mashInTemp*flowRate) / mashVolume;
            hltTempNew -= (coilOutTemp - coilInTemp) * flowRate / hltVolume;
        }

        // heater
        hltTempNew += hltHeaterPower * double(heaterValue) / (100.0 * hltCapacity);

        // environment loss
        mashTempNew -= (mashTemp - envTemp) * kettleEnvTransfer / mashCapacity;
        hltTempNew -= (hltTemp - envTemp) * kettleEnvTransfer / hltCapacity;

        hltTemp = hltTempNew;
        mashTemp = mashTempNew;
    }

    double mashVolume;
    double hltVolume;

    double mashTemp;
    double hltTemp;
    double envTemp;
    double coilInTemp;
    double coilOutTemp;
    double mashInTemp;

    double hltCapacity;
    double mashCapacity;
    double specificHeat;

    double hltHeaterPower;

    double coilTransfer;
    double flowRate;
    double kettleEnvTransfer;
    double mashToCoilLoss;
    double coilToMashLoss;
    bool mashPumping;
};

/* Below are a few static setups that show how control can be set up.
 * The first 4 are simple: a single actuator, acting on beer or fridge temperature
 */

// Heater acts on mash temp directly
struct SimMashDirect : public MashStaticSetup {
    MashSimulation sim;
    SimMashDirect(){
        hltHeaterInputLookup.setPtr(&mash);
        hltHeaterPid.setInputFiltering(1);
        hltHeaterPid.setDerivativeFiltering(3);
        hltHeaterPid.setConstants(50.0, 300, 60);
    }

    void update(){
        MashStaticSetup::update();
        mashSensor.setTemp(sim.mashTemp);
        hltSensor.setTemp(sim.hltTemp);
        sim.update(hltHeater.setting());
        delay(1000);
    }
};


// A heater and a cooler, both acting on fridge temperature directly
struct SimMashCascaded : public MashStaticSetup {
    MashSimulation sim;
    SimMashCascaded(){
        hltHeaterInputLookup.setPtr(&hlt);
        hltHeaterPid.setInputFiltering(1);
        hltHeaterPid.setDerivativeFiltering(2);
        hltHeaterPid.setConstants(50.0, 300, 30);


        mashToHltInputLookup.setPtr(&mash);
        mashToHltPid.setInputFiltering(2);
        mashToHltPid.setDerivativeFiltering(2);
        mashToHltPid.setConstants(1, 180, 0);
        hltOffsetActuator.setMin(-5.0);
        hltOffsetActuator.setMax(5.0);
    }

    void update(){
        MashStaticSetup::update();
        mashSensor.setTemp(sim.mashTemp);
        hltSensor.setTemp(sim.hltTemp);
        sim.update(hltHeater.setting());
        delay(1000); // simulate actual time passing for pin state of cooler, which is time limited
    }
};


BOOST_AUTO_TEST_SUITE( mash_simulation_test)

// Test heating HLT based on mash out temperature (non-cascaded control)
BOOST_FIXTURE_TEST_CASE(Simulate_HLT_Heater_Acts_On_MashTemp, SimMashDirect)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1# mash setPoint, 2#error, 1#mash out sensor, 1#hlt sensor, 1#mash in temp, 3#heater pwm, 3# heater realized pwm, 4#p, 4#i, 4#d" << endl;
    double SetPointDouble = 68;
    for(int t = 0; t < 7200; t++){

        if(t > 2600 && t < 3200){
            SetPointDouble += (5.0/600); // ramp up slowly, 5 degrees in 10 minutes
        }

        if(t > 4600 && t < 6400){
            SetPointDouble += (5.0/1200); // ramp up slowly, 5 degrees in 20 minutes
        }

        mashSet.write(SetPointDouble);
        update();

        csv     << mashSet.read() << "," // setpoint
                << hltHeaterPid.state.error << "," //error
                << mashSensor.read() << "," // mash  temp
                << hltSensor.read() << "," // hlt temp
                << sim.mashInTemp << "," // mash inflow temp
                << hltHeater.setting() << "," // heater output
                << hltHeater.value() << "," // actual heater output
                << hltHeaterPid.state.p << "," // proportional action
                << hltHeaterPid.state.i << "," // integral action
                << hltHeaterPid.state.d // derivative action
                << endl;
    }
    csv.close();
}


// Test heating HLT based on HLT setpoint, but HLT setpoint set by mash out PID (cascaded control)
BOOST_FIXTURE_TEST_CASE(Simulate_Mash_Cascaded_Control, SimMashCascaded)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#mash setpoint, 1#mash out sensor, 2#mash error, "
            "1#hlt setpoint, 1#hlt temp, 2#hlt error,"
            "3#mash2hlt P, 3#mash2hlt I, 3#mash2hlt D, 3#mash2hlt PID, 3#mash2hlt desired output, 3#mash2hlt realized output,"
            "5#heater pwm, 5# heater realized pwm, 4#heater P, 4#heater I, 4#heater D"
            << endl;
    double SetPointDouble = 68;
    for(int t = 0; t < 10800; t++){

        if(t > 3600 && t < 4200){
            SetPointDouble += (5.0/600); // ramp up slowly, 5 degrees in 10 minutes
        }

        mashSet.write(SetPointDouble);
        update();

        csv     << mashSet.read() << "," // setpoint
                << mashSensor.read() << "," // mash temp
                << mashToHltPid.state.error << "," // mash error
                << hltSet.read() << "," // hlt setpoint
                << hltSensor.read() << "," // hlt temp
                << hltHeaterPid.state.error << "," // hlt error

                << mashToHltPid.state.p << "," // proportional action
                << mashToHltPid.state.i << "," // integral action
                << mashToHltPid.state.d << "," // derivative action
                << mashToHltPid.state.p + mashToHltPid.state.i + mashToHltPid.state.d << "," // PID output
                << hltOffsetActuator.setting() << "," // Desired output
                << hltOffsetActuator.value() << "," // Actually realized output

                << hltHeater.setting() << "," // actuator set output heater
                << hltHeater.value() << "," // actuator actual output heater
                << hltHeaterPid.state.p << "," // proportional action
                << hltHeaterPid.state.i << "," // integral action
                << hltHeaterPid.state.d  // derivative action
                << endl;
    }
    csv.close();
}

// Test HLT constant first, then switch to mash constant
BOOST_FIXTURE_TEST_CASE(Simulate_Switch_from_HTL_to_Cascaded_Control_And_Step, SimMashCascaded)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#mash setpoint, 1#mash out sensor, 2#mash error, "
                "1#hlt setpoint, 1#hlt temp, 2#hlt error,"
                "3#mash2hlt P, 3#mash2hlt I, 3#mash2hlt D, 3#mash2hlt PID, 3#mash2hlt desired output, 3#mash2hlt realized output,"
                "5#heater pwm, 5#heater achieved pwm, 4#heater P, 4#heater I, 4#heater D"
            << endl;

    // set PIDs for HLT constant mode (disable automatic HLT set point)
    mashToHltPid.disable(false);
    hltSet.write(70.0);
    mashSet.write(60.0); // not used, but by having a value close to the other values, it won't mess up the plot scaling
    sim.mashPumping = false;

    for(int t = 0; t < 7200; t++){
        if(t == 600){
            // change to cascaded control (enable automatic HLT set point)
            mashSet.write(65.0);
            mashToHltPid.enable();
            sim.mashPumping = true;
        }
        if(t == 3600){
            // change to cascaded control (enable automatic HLT set point)
            mashSet.write(75.0);
        }
        if(t == 1800){
            // sensor value is at setpoint after 10 minutes
            BOOST_CHECK_CLOSE(double(mashSet.read()), double(mashSensor.read()),0.3);
        }
        if(t == 7000){
            // sensor kept exactly at setpoint after second step
            BOOST_CHECK_CLOSE(double(mashSet.read()), double(mashSensor.read()),0.1);
        }
        update();

        csv     << mashSet.read() << "," // setpoint
                << mashSensor.read() << "," // mash temp
                << mashToHltPid.state.error << "," // mash error
                << hltSet.read() << "," // hlt setpoint
                << hltSensor.read() << "," // hlt temp
                << hltHeaterPid.state.error << "," // hlt error

                << mashToHltPid.state.p << "," // proportional action
                << mashToHltPid.state.i << "," // integral action
                << mashToHltPid.state.d << "," // derivative action
                << mashToHltPid.state.p + mashToHltPid.state.i + mashToHltPid.state.d << "," // PID output
                << hltOffsetActuator.setting() << "," // Desired output
                << hltOffsetActuator.value() << "," // Actually realized output

                << hltHeater.setting() << "," // actuator output heater
                << hltHeater.value() << "," // actuator achieved output heater
                << hltHeaterPid.state.p << "," // proportional action
                << hltHeaterPid.state.i << "," // integral action
                << hltHeaterPid.state.d  // derivative action
                << endl;
    }
    csv.close();
}


// Test HLT constant first, then switch to mash constant
BOOST_FIXTURE_TEST_CASE(Simulate_Switch_from_HTL_to_Cascaded_Control_When_Already_Too_High, SimMashCascaded)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#mash setpoint, 1#mash out sensor, 2#mash error, "
                "1#hlt setpoint, 1#hlt temp, 2#hlt error,"
                "3#mash2hlt P, 3#mash2hlt I, 3#mash2hlt D, 3#mash2hlt PID, 3#mash2hlt desired output, 3#mash2hlt realized output,"
                "5#heater pwm, 5#heater achieved pwm, 4#heater P, 4#heater I, 4#heater D"
            << endl;

    // set PIDs for HLT constant mode (disable automatic HLT set point)
    mashToHltPid.disable(false);
    hltSet.write(70.0);
    mashSet.write(68.0); // not used, but by having a value close to the other values, it won't mess up the plot scaling
    sim.hltTemp = 70.5;
    sim.mashTemp = 68.2;

    sim.mashPumping = true;

    for(int t = 0; t < 7200; t++){
        if(t == 100){
            // change to cascaded control (enable automatic HLT set point)
            mashSet.write(68.0);
            mashToHltPid.enable();
            sim.mashPumping = true;
        }
        if(t == 3600){
            mashSet.write(78.0);
        }

        update();

        csv     << mashSet.read() << "," // setpoint
                << mashSensor.read() << "," // mash temp
                << mashToHltPid.state.error << "," // mash error
                << hltSet.read() << "," // hlt setpoint
                << hltSensor.read() << "," // hlt temp
                << hltHeaterPid.state.error << "," // hlt error

                << mashToHltPid.state.p << "," // proportional action
                << mashToHltPid.state.i << "," // integral action
                << mashToHltPid.state.d << "," // derivative action
                << mashToHltPid.state.p + mashToHltPid.state.i + mashToHltPid.state.d << "," // PID output
                << hltOffsetActuator.setting() << "," // Desired output
                << hltOffsetActuator.value() << "," // Actually realized output

                << hltHeater.setting() << "," // actuator output heater
                << hltHeater.value() << "," // actuator achieved output heater
                << hltHeaterPid.state.p << "," // proportional action
                << hltHeaterPid.state.i << "," // integral action
                << hltHeaterPid.state.d  // derivative action
                << endl;
    }
    csv.close();
}

BOOST_FIXTURE_TEST_CASE(Simulate_very_small_proportonial_gain, SimMashCascaded)
{
    // this test is to guard that even with a too small Kp, the integrator will still eventually make the PID reach the setpoint
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#mash setpoint, 1#mash out sensor, 2#mash error, "
                "1#hlt setpoint, 1#hlt temp, 2#hlt error,"
                "3#mash2hlt P, 3#mash2hlt I, 3#mash2hlt D, 3#mash2hlt PID, 3#mash2hlt desired output, 3#mash2hlt realized output,"
                "5#heater pwm, 5#heater achieved pwm, 4#heater P, 4#heater I, 4#heater D"
            << endl;

    mashSet.write(66.0);
    mashToHltPid.setConstants(0.2, 120, 0);


    for(int t = 0; t < 3600; t++){
        update();
        csv     << mashSet.read() << "," // setpoint
                << mashSensor.read() << "," // mash temp
                << mashToHltPid.state.error << "," // mash error
                << hltSet.read() << "," // hlt setpoint
                << hltSensor.read() << "," // hlt temp
                << hltHeaterPid.state.error << "," // hlt error

                << mashToHltPid.state.p << "," // proportional action
                << mashToHltPid.state.i << "," // integral action
                << mashToHltPid.state.d << "," // derivative action
                << mashToHltPid.state.p + mashToHltPid.state.i + mashToHltPid.state.d << "," // PID output
                << hltOffsetActuator.setting() << "," // Desired output
                << hltOffsetActuator.value() << "," // Actually realized output

                << hltHeater.setting() << "," // actuator output heater
                << hltHeater.value() << "," // actuator achieved output heater
                << hltHeaterPid.state.p << "," // proportional action
                << hltHeaterPid.state.i << "," // integral action
                << hltHeaterPid.state.d  // derivative action
                << endl;
    }
    update();
    BOOST_CHECK_CLOSE(double(mashSet.read()), double(mashSensor.read()),0.1);
    csv.close();
}



BOOST_AUTO_TEST_SUITE_END()
