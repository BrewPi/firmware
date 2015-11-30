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


#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time, to seed rand */

#include "ActuatorMocks.h"
#include "ActuatorPwm.h"
#include "ActuatorTimeLimited.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorMutexGroup.h"
#include <cstring>
#include "runner.h"
#include <iostream>
#include <fstream>

#define PRINT_TOGGLE_TIMES 0

temp_t randomIntervalTest(ActuatorPwm* act, ActuatorDigital * target, temp_t duty, int delayMax) {
    act->setValue(duty);
    ticks_millis_t lowToHighTime = ticks.millis();
    ticks_millis_t highToLowTime = ticks.millis();
    ticks_millis_t totalHighTime = 0;
    ticks_millis_t totalLowTime = 0;

    output << format("\n\n*** Results running 100 periods and random 0-%d ms update intervals,"
            "with duty cycle %u and period %u ***\n") % delayMax % duty % act->getPeriod();
#if PRINT_TOGGLE_TIMES
    output << "\n\nl->h timestamp  h->l timestamp       high time       low time\n";
#endif
    // run for 100 periods
    for (int i = 0; i < 100; i++) {
        do {
            highToLowTime = random_delay(delayMax);
            act->update();
        } while (target->isActive());
        ticks_millis_t highTime = highToLowTime - lowToHighTime;
        if (i > 0) { // skip first cycle in totals, it can be incomplete
            totalHighTime += highTime;
        }
#if PRINT_TOGGLE_TIMES
        output << format("_/  %10u \t \\_ %10u \t") % lowToHighTime % highToLowTime;
        output << format(" %10u ms \t") % highTime;
#endif
        do {
            lowToHighTime = random_delay(delayMax);
            act->update();
        } while (!target->isActive());
        ticks_millis_t lowTime = lowToHighTime - highToLowTime;
        if (i > 0) { // skip first cycle in totals, it can have old duty cycle
            totalLowTime += lowTime;
#if PRINT_TOGGLE_TIMES
            output << format("%10u ms \n") % lowTime;
#endif
        }
        else{
#if PRINT_TOGGLE_TIMES
            output << format("%10u ms (ignored)\n") % lowTime;
#endif
        }
        
    }
    double avgDuty = double(totalHighTime) / (totalHighTime + totalLowTime);
    temp_t avgDutyTemp = temp_t(round(avgDuty * double(act->max())));
    output << "total high time: " << totalHighTime << "\n"
           << "total low time: " << totalLowTime << "\n"
           << "avg duty: " << avgDutyTemp << "/" << act->max() << "\n";
    return avgDutyTemp;
}

BOOST_AUTO_TEST_SUITE(ActuatorPWM)

BOOST_AUTO_TEST_CASE( Test_ActuatorPWM_with_ValueActuator_as_driver) {
    ActuatorDigital * target = new ActuatorBool();
    ActuatorPwm * act = new ActuatorPwm(target,4);

    BOOST_CHECK(act->getValue() == temp_t(0.0)); // PWM value is initialized to 0

    // Test that PWM can be set and read
    act->setValue(50.0);
    BOOST_CHECK_EQUAL(act->getValue(), temp_t(50.0));
    act->setValue(100.0);
    BOOST_CHECK_EQUAL(act->getValue(), temp_t(100.0));
    act->setValue(0.0);
    BOOST_CHECK_EQUAL(act->getValue(), temp_t(0.0));

    act->setValue(110.0);
    BOOST_CHECK_EQUAL(act->getValue(), temp_t(100.0)); // max is 100

    act->setValue(-50.0);
    BOOST_CHECK_EQUAL(act->getValue(), temp_t(0.0)); // min is 0
}

BOOST_AUTO_TEST_CASE(test_ticks_millis_to_increment_every_call) {
    output << "Testing time:";
    ticks_millis_t start = ticks.millis();
    ticks_millis_t time;
    for (ticks_millis_t i = start + 1; i <= start + 10; i++) {
        time = ticks.millis();
        output << time << ' ';
        BOOST_REQUIRE_EQUAL(i, time);
    }
    output << ".\n";
}

BOOST_AUTO_TEST_CASE(on_off_time_matches_duty_cycle_when_updating_every_ms) {
    srand(time(NULL));
    ActuatorDigital * target = new ActuatorBool();
    ActuatorPwm * act = new ActuatorPwm(target,4);

    temp_t duty = 50.0;
    act->setValue(duty);
    ticks_millis_t lowToHighTime1 = ticks.millis();
    ticks_millis_t highToLowTime1 = ticks.millis();
    ticks_millis_t lowToHighTime2 = ticks.millis();
    do {
        lowToHighTime1 = delay(1);
        act->update();
    } while (!target->isActive());
    do {
        highToLowTime1 = delay(1);
        act->update();
    } while (target->isActive());
    do {
        lowToHighTime2 = delay(1);
        act->update();
    } while (!target->isActive());

    ticks_millis_t timeHigh = highToLowTime1 - lowToHighTime1;
    ticks_millis_t timeLow = lowToHighTime2 - highToLowTime1;
    double actualDuty = (timeHigh * 100.0) / (timeHigh + timeLow); // rounded result
    output << "*** Timestamps testing one period with duty cycle " << duty << " and period " << act->getPeriod() << "***\n";
    output << "lowToHigh1: " << lowToHighTime1 << "\t"
           << "highToLow1: " << highToLowTime1 <<" \t lowToHigh2: " << lowToHighTime2 << "\n"
           << "time high: " << timeHigh << "\t"
           << "time low: " << timeLow << "\t"
           << "actual duty cycle: " << actualDuty;
    BOOST_CHECK_CLOSE(actualDuty, 50.0, 0.1);
}

BOOST_AUTO_TEST_CASE(average_duty_cycle_is_correct_with_random_update_intervals) {
    srand(time(NULL));
    ActuatorDigital * target = new ActuatorBool();
    ActuatorPwm * act = new ActuatorPwm(target,4);
    BOOST_CHECK_EQUAL(randomIntervalTest(act, target, 50.0, 500), temp_t(50.0));
    BOOST_CHECK_EQUAL(randomIntervalTest(act, target, 3.0, 500), temp_t(3.0));
    BOOST_CHECK_EQUAL(randomIntervalTest(act, target, 1.0, 500), temp_t(1.0));
    BOOST_CHECK_EQUAL(randomIntervalTest(act, target, 99.0, 500), temp_t(99.0));
}

BOOST_AUTO_TEST_CASE(average_duty_cycle_is_correct_with_long_period) {
    srand(time(NULL));
    ActuatorDigital * target = new ActuatorBool();
    ActuatorPwm * act = new ActuatorPwm(target,3600);
    BOOST_CHECK_EQUAL(randomIntervalTest(act, target, 50.0, 50000), temp_t(50.0));
    BOOST_CHECK_EQUAL(randomIntervalTest(act, target, 3.0, 50000), temp_t(3.0));
    BOOST_CHECK_EQUAL(randomIntervalTest(act, target, 1.0, 50000), temp_t(1.0));
    BOOST_CHECK_EQUAL(randomIntervalTest(act, target, 99.0, 50000), temp_t(99.0));
}



BOOST_AUTO_TEST_CASE(output_stays_low_with_value_0) {
    ActuatorDigital * target = new ActuatorBool();
    ActuatorPwm * act = new ActuatorPwm(target,4);

    act->setValue(0.0);
    // wait target to go low
    while (target->isActive()) {
        delay(1);
        act->update();
    }
    for (uint32_t i = 0; i < 10 * act->getPeriod() * 1000; i++) {
        delay(1);
        act->update();

        BOOST_REQUIRE_MESSAGE(!target->isActive(), "Actuator was high at i=" << i);
    }
}

BOOST_AUTO_TEST_CASE(on_big_positive_changes_go_high_immediately) {
    ActuatorDigital * vAct = new ActuatorBool();
    ActuatorDigital * limited = new ActuatorTimeLimited(vAct, 0, 0);
    ActuatorPwm * act = new ActuatorPwm(limited, 100); // period is 100 seconds

    act->setValue(30);
    for (uint32_t i = 0; i < 250 ; i++) { // 250 seconds
        delay(1000);
        act->update();
    }
    BOOST_CHECK(!vAct->isActive()); // actuator is inactive
    act->setValue(60.0);
    act->update();
    BOOST_CHECK(vAct->isActive()); // actuator turns on immediately

    ticks_millis_t highTime = ticks.millis();
    ticks_millis_t lowTime = ticks.millis();
    while(vAct->isActive()){
        delay(1000);
        act->update();
    }
    lowTime = ticks.millis();
    BOOST_CHECK_CLOSE(double(lowTime - highTime) / (1000*act->getPeriod()), 0.6, 2); // check that high period afterwards is correct
}


BOOST_AUTO_TEST_CASE(on_big_negative_changes_go_low_immediately) {
    ActuatorDigital * vAct = new ActuatorBool();
    ActuatorDigital * limited = new ActuatorTimeLimited(vAct, 0, 0);
    ActuatorPwm * act = new ActuatorPwm(limited, 100); // period is 100 seconds

    act->setValue(60.0);
    for (uint32_t i = 0; i < 250 ; i++) { // 250 seconds
        delay(1000);
        act->update();
    }

    BOOST_CHECK(vAct->isActive()); // actuator is active
    act->setValue(30.0);
    act->update();
    BOOST_CHECK(!vAct->isActive()); // actuator turns off immediately

    ticks_millis_t lowTime = ticks.millis();
    ticks_millis_t highTime = ticks.millis();

    while(!vAct->isActive()){
        delay(1000);
        act->update();
    }
    highTime = ticks.millis();
    BOOST_CHECK_CLOSE(double(highTime - lowTime) / (1000*act->getPeriod()), 0.7, 2); // check that low period afterwards is correct
}


BOOST_AUTO_TEST_CASE(output_stays_high_with_value_100) {
    ActuatorDigital * target = new ActuatorBool();
    ActuatorPwm * act = new ActuatorPwm(target,4);

    act->setValue(100.0);
    // wait for target to go high
    while (!target->isActive()) {
        delay(1);
        act->update();
    }
    for (uint32_t i = 0; i < 10 * act->getPeriod() * 1000; i++) {
        delay(1);
        act->update();
        BOOST_REQUIRE_MESSAGE(target->isActive(), "Actuator was low at i=" << i);
    }
}


BOOST_AUTO_TEST_CASE(ActuatorPWM_with_min_max_time_limited_OnOffActuator_as_driver){
    // test with minimum ON of 2 seconds, minimum off of 5 seconds and period 10 seconds

    srand(time(NULL));
    ActuatorDigital * vAct = new ActuatorBool();
    ActuatorDigital * onOffAct = new ActuatorTimeLimited(vAct, 2, 5);
    ActuatorPwm * act = new ActuatorPwm(onOffAct, 10);

    // Test that average duty cycle is correct, even with minimum times enforced in the actuator
    BOOST_CHECK_EQUAL(randomIntervalTest(act, vAct, 50.0, 500), temp_t(50.0));
    BOOST_CHECK_EQUAL(randomIntervalTest(act, vAct, 3.0, 500), temp_t(3.0));
    BOOST_CHECK_EQUAL(randomIntervalTest(act, vAct, 1.0, 500), temp_t(1.0));
    BOOST_CHECK_EQUAL(randomIntervalTest(act, vAct, 99.0, 500), temp_t(99.0)); // 99 not attainable due to minimum OFF time
}


BOOST_AUTO_TEST_CASE(when_switching_between_zero_and_low_value_average_is_correct){
    // test with minimum ON of 2 seconds, minimum off of 5 seconds and period 5 seconds
    ActuatorDigital * vAct = new ActuatorBool();
    ActuatorDigital * onOffAct = new ActuatorTimeLimited(vAct, 20, 50);
    ActuatorPwm * act = new ActuatorPwm(onOffAct, 100);

    ticks_seconds_t timeHigh = 0;
    ticks_seconds_t timeLow = 0;

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "value, pin" << endl;

    for(int cycles = 0; cycles < 100; cycles++){
        if(cycles %2 == 0){
            act->setValue(4.0); // under minimum ON time of 20, doesn't trigger skipping ahead
        }
        else{
            act->setValue(0.0);
        }
        for(int i = 0; i < 180; i++){ // 180 seconds, not full periods on purpose
            delay(1000);
            act->update();
            if(vAct->isActive()){
                timeHigh++;
            }
            else{
                timeLow++;
            }
            csv     << act->getValue() << "," // setpoint
                    << vAct->isActive() // actual cooler pin state
                    << endl;
        }
    }

    csv.close();

    double avgDuty = double(timeHigh) * 100.0 / (timeHigh + timeLow);
    BOOST_CHECK_CLOSE(avgDuty, 2, 5); // actuator will do about 20 pulses, so 5% error margin
}


BOOST_AUTO_TEST_CASE(ramping_PWM_up_faster_than_period_gives_correct_average){
    ActuatorDigital * vAct = new ActuatorBool();
    ActuatorPwm * act = new ActuatorPwm(vAct, 20);
    ticks_seconds_t timeHigh = 0;
    ticks_seconds_t timeLow = 0;

    for(int ramps = 0; ramps <100; ramps++){ // enough ramps to not be affected by time window
        for(temp_t v = temp_t(40.0); v <= temp_t(60.0); v = v + temp_t(0.25)){
            act->setValue(v);
            for(int j = 0; j < 100; j++){ // 10 seconds total
                delay(100);
                act->update();
                if(vAct->isActive()){
                    timeHigh++;
                }
                else{
                    timeLow++;
                }
            }
        }
    }

    double avgDuty = double(timeHigh) * 100.0 / (timeHigh + timeLow);
    BOOST_CHECK_CLOSE(avgDuty, 50.0, 2);
}


BOOST_AUTO_TEST_CASE(ramping_PWM_down_faster_than_period_gives_correct_average){
    ActuatorDigital * vAct = new ActuatorBool();
    ActuatorPwm * act = new ActuatorPwm(vAct, 20);
    ticks_seconds_t timeHigh = 0;
    ticks_seconds_t timeLow = 0;

    for(int ramps = 0; ramps <100; ramps++){ // enough ramps to not be affected by time window
        for(temp_t v = temp_t(60.0); v >= temp_t(40.0); v = v - temp_t(0.25)){
          act->setValue(v);
          for(int j = 0; j < 100; j++){ // 10 seconds total
              delay(100);
              act->update();
              if(vAct->isActive()){
                  timeHigh++;
              }
              else{
                  timeLow++;
              }
          }
        }
    }

    double avgDuty = double(timeHigh) * 100.0 / (timeHigh + timeLow);
    BOOST_CHECK_CLOSE(avgDuty, 50.0, 2);
}

BOOST_AUTO_TEST_CASE(two_mutex_PWM_actuators_can_overlap){
    ActuatorDigital * boolAct1 = new ActuatorBool();
    ActuatorMutexDriver * mutexAct1 = new ActuatorMutexDriver(boolAct1);
    ActuatorPwm * act1 = new ActuatorPwm(mutexAct1, 20);

    ActuatorDigital * boolAct2 = new ActuatorBool();
    ActuatorMutexDriver * mutexAct2 = new ActuatorMutexDriver(boolAct2);
    ActuatorPwm * act2 = new ActuatorPwm(mutexAct2, 20);

    ActuatorMutexGroup * mutex = new ActuatorMutexGroup();
    mutexAct1->setMutex(mutex);
    mutexAct2->setMutex(mutex);

    ticks_seconds_t timeHigh1 = 0;
    ticks_seconds_t timeLow1 = 0;
    ticks_seconds_t timeHigh2 = 0;
    ticks_seconds_t timeLow2 = 0;

    act1->setValue(20.0);
    act2->setValue(20.0);

    ticks_millis_t start = ticks.millis();

    while(ticks.millis() - start < 1000000){ // run for 1000 seconds
        act1->update();
        act2->update();
        delay(100);
        if(boolAct1->isActive()){
            timeHigh1++;
        }
        else{
            timeLow1++;
        }
        if(boolAct2->isActive()){
            timeHigh2++;
        }
        else{
            timeLow2++;
        }
        BOOST_REQUIRE(!(boolAct1->isActive() && boolAct2->isActive())); // actuators cannot be active at the same time
    }

    double avgDuty1 = double(timeHigh1) * 100.0 / (timeHigh1 + timeLow1);
    BOOST_CHECK_CLOSE(avgDuty1, 20.0, 1);

    double avgDuty2 = double(timeHigh2) * 100.0 / (timeHigh2 + timeLow2);
    BOOST_CHECK_CLOSE(avgDuty2, 20.0, 1);
}

BOOST_AUTO_TEST_CASE(install_and_uninstall_final_actuator){
    ActuatorMutexGroup * mutex = new ActuatorMutexGroup();
    mutex->setDeadTime(1000);
    ActuatorDigital * coolerPin = new ActuatorBool();
    ActuatorTimeLimited * coolerTimeLimited = new ActuatorTimeLimited(coolerPin, 120, 180); // 2 min minOn time, 3 min minOff
    ActuatorMutexDriver * coolerMutex = new ActuatorMutexDriver(coolerTimeLimited, mutex);
    ActuatorPwm * cooler = new ActuatorPwm(coolerMutex, 10); // period 10 min

    ActuatorDigital * heaterPin = new ActuatorBool();
    ActuatorMutexDriver * heaterMutex = new ActuatorMutexDriver(heaterPin, mutex);
    ActuatorPwm * heater = new ActuatorPwm(heaterMutex, 4); // period 4s

    BOOST_CHECK_EQUAL(coolerTimeLimited->getTarget(), coolerPin);

    // make sure actuator is added to the mutex group

    cooler->setValue(50);
    heater->setValue(50);
    ticks_millis_t start = ticks.millis();
    while(ticks.millis() - start < 100000){ // run for 100 seconds
        cooler->update();
        heater->update();
        mutex->update();
    }

    BOOST_CHECK(cooler->unInstallActuatorFinalTarget()); // returns true on successful uninstall
    BOOST_CHECK(heater->unInstallActuatorFinalTarget()); // returns true on successful uninstall
    BOOST_CHECK_EQUAL(cooler->getTarget(), coolerMutex); // unchanged
    BOOST_CHECK_EQUAL(coolerMutex->getTarget(), coolerTimeLimited); // unchanged
    BOOST_CHECK_EQUAL(coolerTimeLimited->getTarget(), defaultActuator()); // replaced by default actuator
    BOOST_CHECK_EQUAL(heater->getTarget(), heaterMutex); // unchanged
    BOOST_CHECK_EQUAL(heaterMutex->getTarget(), defaultActuator()); // replaced by default actuator

    // mutex group should not crash due to old references to uninstalled actuators
    start = ticks.millis();
    while(ticks.millis() - start < 100000){ // run for 100 seconds
        cooler->update();
        heater->update();
        mutex->update();
    }

    BOOST_CHECK(!cooler->unInstallActuatorFinalTarget()); // returns false, when target is already default actuator
    BOOST_CHECK(!heater->unInstallActuatorFinalTarget()); // returns false, when target is already default actuator

    coolerPin = new ActuatorBool(); // uninstall deleted the previous instance, need to recreate!
    heaterPin = new ActuatorBool(); // uninstall deleted the previous instance, need to recreate!

    BOOST_CHECK(cooler->installActuatorFinalTarget(coolerPin)); // returns true on successful install
    BOOST_CHECK(heater->installActuatorFinalTarget(heaterPin)); // returns true on successful install
    BOOST_CHECK_EQUAL(cooler->getTarget(), coolerMutex); // unchanged
    BOOST_CHECK_EQUAL(coolerMutex->getTarget(), coolerTimeLimited); // unchanged
    BOOST_CHECK_EQUAL(coolerTimeLimited->getTarget(), coolerPin); // replaced by default coolerPin again
    BOOST_CHECK_EQUAL(heater->getTarget(), heaterMutex); // unchanged
    BOOST_CHECK_EQUAL(heaterMutex->getTarget(), heaterPin); // replaced by default coolerPin again

    // mutex group should not crash due to old references to uninstalled actuators
    start = ticks.millis();
    while(ticks.millis() - start < 100000){ // run for 100 seconds
        cooler->update();
        heater->update();
        mutex->update();
    }

    BOOST_CHECK(!cooler->installActuatorFinalTarget(coolerPin)); // returns false when actuator was already installed
    BOOST_CHECK(!heater->installActuatorFinalTarget(heaterPin)); // returns false when actuator was already installed
}

BOOST_AUTO_TEST_SUITE_END()
