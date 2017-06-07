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
        hltHeaterMutex(hltHeaterPin),
        hltHeater(hltHeaterMutex, 4), // period 4s
        hltSet(20.0),
        mashSet(20.0),
        hlt(hltSensor, hltSet),
        mash(mashSensor, mashSet),
        hltOffsetActuator(hlt, mash),
        hltHeaterPid(hltHeaterPidInput, hltHeaterPidOutput),
        mashToHltPid(mashToHltPidInput, mashToHltPidOutput)

    {
        BOOST_TEST_MESSAGE( "setup mash test fixture" );
        hltHeaterPidInput.setLookup(PtrLookup(&hlt));
        mashToHltPidInput.setLookup(PtrLookup(&mash));
        hltHeaterPidOutput.setLookup(PtrLookup(&hltHeater));
        mashToHltPidOutput.setLookup(PtrLookup(&hltOffsetActuator));
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

    ProcessValueDelegate hltHeaterPidInput;
    ProcessValueDelegate hltHeaterPidOutput;
    Pid hltHeaterPid;

    ProcessValueDelegate mashToHltPidInput;
    ProcessValueDelegate mashToHltPidOutput;
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

        mashVolume = 18;
        hltVolume = 36;

        hltCapacity = 4.2 * 1.0 * hltVolume; // heat capacity water * density of water * 20L volume (in kJ per degree C).
        mashCapacity = 4.2 * 1.0 * mashVolume;

        hltHeaterPower = 3.5; // 3500W, in kW.

        coilTransfer = 0.8; // percentage of temperature difference picked up in HLT coil
        flowRate = 10.0/60; // 5 liter per minute, in L/s.
        kettleEnvTransfer = 0.01; // losses to environment
        mashToCoilLoss = 0.05; // losses between mash tun and coil
        coilToMashLoss = 0.03; // losses between mash tun and coil

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
            hltTempNew -= (coilOutTemp - coilInTemp) * flowRate / hltCapacity;
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
        hltHeaterPidInput.setLookup(PtrLookup(&mash));
        hltHeaterPid.setInputFilter(1);
        hltHeaterPid.setDerivativeFilter(1);
        hltHeaterPid.setConstants(50.0, 300, 120);
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
        hltHeaterPidInput.setLookup(PtrLookup(&hlt));
        hltHeaterPid.setInputFilter(1);
        hltHeaterPid.setDerivativeFilter(1);
        hltHeaterPid.setConstants(50.0, 300, 30);


        mashToHltPidInput.setLookup(PtrLookup(&mash));
        mashToHltPid.setInputFilter(2);
        mashToHltPid.setDerivativeFilter(2);
        mashToHltPid.setConstants(0.5, 300, 120);
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
                << hltHeaterPid.inputError << "," //error
                << mashSensor.read() << "," // mash  temp
                << hltSensor.read() << "," // hlt temp
                << sim.mashInTemp << "," // mash inflow temp
                << hltHeater.setting() << "," // heater output
                << hltHeater.value() << "," // actual heater output
                << hltHeaterPid.p << "," // proportional action
                << hltHeaterPid.i << "," // integral action
                << hltHeaterPid.d // derivative action
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
            "3#mash2hlt P, 3#mash2hlt I, 3#mash2hlt D, 3#mash2hlt PID, 3#mash2hlt realized output,"
            "5#heater pwm, 5# header realized pwm, 4#heater P, 4#heater I, 4#heater D"
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
                << mashToHltPid.inputError << "," // mash error
                << hltSet.read() << "," // hlt setpoint
                << hltSensor.read() << "," // hlt temp
                << hltHeaterPid.inputError << "," // hlt error

                << mashToHltPid.p << "," // proportional action
                << mashToHltPid.i << "," // integral action
                << mashToHltPid.d << "," // derivative action
                << mashToHltPid.p + mashToHltPid.i + mashToHltPid.d << "," // PID output
                << hltOffsetActuator.setting() << "," // Actually realized output

                << hltHeater.setting() << "," // actuator set output heater
                << hltHeater.value() << "," // actuator actual output heater
                << hltHeaterPid.p << "," // proportional action
                << hltHeaterPid.i << "," // integral action
                << hltHeaterPid.d  // derivative action
                << endl;
    }
    csv.close();
}

// Test HLT constant first, then switch to mash constant
BOOST_FIXTURE_TEST_CASE(Simulate_Switch_from_HTL_to_Cascaded_Control, SimMashCascaded)
{
    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#mash setpoint, 1#mash out sensor, 2#mash error, "
                "1#hlt setpoint, 1#hlt temp, 2#hlt error,"
                "3#mash2hlt P, 3#mash2hlt I, 3#mash2hlt D, 3#mash2hlt PID, 3#mash2hlt realized output,"
                "5#heater pwm, 5#heater achieved pwm, 4#heater P, 4#heater I, 4#heater D"
            << endl;

    // set PIDs for HLT constant mode (disable automatic HLT set point)
    mashToHltPid.disable(false);
    hltSet.write(70.0);
    sim.mashPumping = false;

    for(int t = 0; t < 10800; t++){
        if(t == 3600){
            // change to cascaded control (enable automatic HLT set point)
            mashSet.write(65.0);
            mashToHltPid.enable();
            sim.mashPumping = true;
        }
        update();

        csv     << mashSet.read() << "," // setpoint
                << mashSensor.read() << "," // mash temp
                << mashToHltPid.inputError << "," // mash error
                << hltSet.read() << "," // hlt setpoint
                << hltSensor.read() << "," // hlt temp
                << hltHeaterPid.inputError << "," // hlt error

                << mashToHltPid.p << "," // proportional action
                << mashToHltPid.i << "," // integral action
                << mashToHltPid.d << "," // derivative action
                << mashToHltPid.p + mashToHltPid.i + mashToHltPid.d << "," // PID output
                << hltOffsetActuator.setting() << "," // Actually realized output

                << hltHeater.setting() << "," // actuator output heater
                << hltHeater.value() << "," // actuator achieved output heater
                << hltHeaterPid.p << "," // proportional action
                << hltHeaterPid.i << "," // integral action
                << hltHeaterPid.d  // derivative action
                << endl;
    }
    csv.close();
}

BOOST_AUTO_TEST_SUITE_END()
