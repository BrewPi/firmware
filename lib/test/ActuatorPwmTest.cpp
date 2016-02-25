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
#include <math.h> // for sin

#define PRINT_TOGGLE_TIMES 0

double randomIntervalTest(ActuatorPwm* act, ActuatorDigital * target, temp_t duty, int delayMax) {
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
    double avgDuty = double(totalHighTime) / (totalHighTime + totalLowTime) * double(act->max());
    output << "total high time: " << totalHighTime << "\n"
           << "total low time: " << totalLowTime << "\n"
           << "avg duty: " << avgDuty << "/" << act->max() << "\n";
    return avgDuty;
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
    BOOST_CHECK_CLOSE(actualDuty, 50.0, 0.5);
}

BOOST_AUTO_TEST_CASE(average_duty_cycle_is_correct_with_random_update_intervals) {
    srand(time(NULL));
    ActuatorDigital * target = new ActuatorBool();
    ActuatorPwm * act = new ActuatorPwm(target,4);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 50.0, 500), 50.0, 2); // between 49 and 51
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 3.0, 500), 3.0, 16.7); // between 2.5 and 3.5
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 1.0, 500), 1.0, 50); // between 0.5 and 1.5
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 99.0, 500), 99.0, 0.5); // between 98.5 and 99.5
}

BOOST_AUTO_TEST_CASE(average_duty_cycle_is_correct_with_long_period) {
    srand(time(NULL));
    ActuatorDigital * target = new ActuatorBool();
    ActuatorPwm * act = new ActuatorPwm(target,3600);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 50.0, 500), 50.0, 1);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 3.0, 500), 3.0, 16.7);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 1.0, 500), 1.0, 50);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 99.0, 500), 99.0, 0.5);
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

BOOST_AUTO_TEST_CASE(on_big_positive_changes_shortened_cycle_has_correct_value) {
    ActuatorDigital * vAct = new ActuatorBool();
    ActuatorDigital * limited = new ActuatorTimeLimited(vAct, 0, 0);
    ActuatorPwm * act = new ActuatorPwm(limited, 100); // period is 100 seconds

    act->setValue(short(30));
    ticks_millis_t start = ticks.millis();
    ticks_millis_t periodStart;
    while(ticks.millis() - start < 250000) { // 250 seconds
        bool oldState = vAct->isActive();
        act->update();
        if(!oldState && vAct->isActive()){ // low to high transition
            periodStart = ticks.millis();
        }
        delay(1000);
    }

    BOOST_CHECK(!vAct->isActive()); // actuator is inactive, ~50 seconds into 3rd cycle
    act->setValue(50.0);
    while (!vAct->isActive()) {
        delay(1000);
        act->update();
    }
    // cycle should be shortened to 60 seconds, 30 high + 30 low
    BOOST_CHECK_CLOSE(double(ticks.millis() - periodStart), 60000, 3);
    periodStart = ticks.millis();

    while(vAct->isActive()){
        delay(1000);
        act->update();
    }
    // next high time should be normal
    BOOST_CHECK_CLOSE(double(ticks.millis() - periodStart), 50000, 1); // actuator turned on for 50 seconds

    while(!vAct->isActive()){
        delay(1000);
        act->update();
    }
    // next low time should be normal
    BOOST_CHECK_CLOSE(double(ticks.millis() - periodStart), 100000, 1); // actuator turned on for 50 seconds
}


BOOST_AUTO_TEST_CASE(on_big_negative_changes_go_low_immediately) {
    ActuatorDigital * vAct = new ActuatorBool();
    ActuatorDigital * limited = new ActuatorTimeLimited(vAct, 0, 0);
    ActuatorPwm * act = new ActuatorPwm(limited, 100); // period is 100 seconds

    ticks_millis_t lastLowTimeBeforeChange = ticks.millis();
    act->setValue(60.0);
    for (uint32_t i = 0; i < 250 ; i++) { // 250 seconds
        delay(1000);
        act->update();
        if(!vAct->isActive()){
            lastLowTimeBeforeChange = ticks.millis();
        }
    }

    BOOST_CHECK(vAct->isActive()); // actuator is active
    act->setValue(30.0);
    act->update();
    BOOST_CHECK(!vAct->isActive()); // actuator turns off immediately

    ticks_millis_t highTolowTime = ticks.millis();
    ticks_millis_t highPeriod = highTolowTime - lastLowTimeBeforeChange;

    while(!vAct->isActive()){
        delay(100);
        act->update();
    }
    ticks_millis_t lowToHighTime = ticks.millis();
    ticks_millis_t lowPeriod = lowToHighTime - highTolowTime;
    // check that this cycle has normal duration
    BOOST_CHECK_CLOSE(double(highPeriod + lowPeriod), 100000, 2);

    // but overshooting the high value is compensated in high period of next cycle
    while(vAct->isActive()){
        delay(100);
        act->update();
    }
    ticks_millis_t reducedHighPeriod = ticks.millis() - lowToHighTime;

    BOOST_CHECK_CLOSE(double(highPeriod + reducedHighPeriod), 0.3*2*100000, 2);
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
    BOOST_CHECK_CLOSE(randomIntervalTest(act, vAct, 50.0, 500), 50.0, 1);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, vAct, 3.0, 500), 3.0, 16.7);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, vAct, 1.0, 500), 1.0, 50);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, vAct, 99.0, 500), 99.0, 0.5);
}


BOOST_AUTO_TEST_CASE(when_switching_between_zero_and_low_value_average_is_correct){
    // test with minimum ON of 2 seconds, minimum off of 5 seconds and period 5 seconds
    ActuatorDigital * vAct = new ActuatorBool();
    ActuatorDigital * onOffAct = new ActuatorTimeLimited(vAct, 20, 50);
    ActuatorPwm * act = new ActuatorPwm(onOffAct, 100);

    ticks_seconds_t timeHigh = 0;
    ticks_seconds_t timeLow = 0;

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#value, 2a#pin" << endl;

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
    BOOST_CHECK_CLOSE(avgDuty, 2, 25); // value is between 1.5 and 2.5
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
    ActuatorPwm * act1 = new ActuatorPwm(mutexAct1, 10);

    ActuatorDigital * boolAct2 = new ActuatorBool();
    ActuatorMutexDriver * mutexAct2 = new ActuatorMutexDriver(boolAct2);
    ActuatorPwm * act2 = new ActuatorPwm(mutexAct2, 10);

    ActuatorMutexGroup * mutex = new ActuatorMutexGroup();
    mutex->setDeadTime(0);
    mutexAct1->setMutex(mutex);
    mutexAct2->setMutex(mutex);

    ticks_seconds_t timeHigh1 = 0;
    ticks_seconds_t timeLow1 = 0;
    ticks_seconds_t timeHigh2 = 0;
    ticks_seconds_t timeLow2 = 0;

    act1->setValue(20.0);
    act2->setValue(20.0);

    act1->update();
    act2->update();
    ticks_millis_t start = ticks.millis();

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
        csv << "1a#pin1, 1a#pin2" << endl;

    while(ticks.millis() - start <= 100000){ // run for 100 seconds
        act1->update();
        act2->update();
        mutex->update();
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
        csv     << boolAct1->isActive() << ","
                << boolAct2->isActive()
                << endl;
        delay(100);
    }

    double avgDuty1 = double(timeHigh1) * 100.0 / (timeHigh1 + timeLow1);
    BOOST_CHECK_CLOSE(avgDuty1, 20.0, 2); // small error possible due to test window influence

    double avgDuty2 = double(timeHigh2) * 100.0 / (timeHigh2 + timeLow2);
    BOOST_CHECK_CLOSE(avgDuty2, 20.0, 2); // small error possible due to test window influence
}

BOOST_AUTO_TEST_CASE(actual_value_returned_by_ActuatorPwm_readValue_is_correct){
    ActuatorDigital * boolAct = new ActuatorBool();
    ActuatorPwm * pwmAct = new ActuatorPwm(boolAct, 20);

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#set value, 1#read value, 2a#pin" << endl;

    pwmAct->setValue(20.0);
    ticks_millis_t start = ticks.millis();
    while(ticks.millis() - start < 200000){ // run for 200 seconds to dial in cycle time
        pwmAct->update();
        delay(100);
    }

    start = ticks.millis();

    int count = 0;
    double sum = 0.0;
    while(ticks.millis() - start < 500000){ // run for 500 seconds
        pwmAct->update();
        csv     << pwmAct->getValue() << ","
                << pwmAct->readValue() << ","
                << boolAct->isActive()
                << endl;
        count++;
        sum += double(pwmAct->readValue());
        delay(100);
    }
    double average = sum/count;
    BOOST_CHECK_CLOSE(average, 20.0, 1);
}


BOOST_AUTO_TEST_CASE(actual_value_returned_by_ActuatorPwm_readValue_is_correct_with_time_limited_actuator){
    ActuatorDigital * boolAct = new ActuatorBool();
    ActuatorDigital * timeLimitedAct = new ActuatorTimeLimited(boolAct, 2, 5);
    ActuatorPwm * pwmAct = new ActuatorPwm(timeLimitedAct, 20);

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#set value, 1#read value, 2a#pin" << endl;

    pwmAct->setValue(5.0); // set to a value with duty cycle lower than time limit

    ticks_millis_t start = ticks.millis();
    while(ticks.millis() - start < 100000){ // run for 100 seconds to dial in cycle time
        pwmAct->update();
        delay(100);
    }

    start = ticks.millis();

    int count = 0;
    double sum = 0.0;
    while(ticks.millis() - start < 1000000){ // run for 1000 seconds
        pwmAct->update();
        csv     << pwmAct->getValue() << ","
                << pwmAct->readValue() << ","
                << boolAct->isActive()
                << endl;
        count++;
        sum += double(pwmAct->readValue());
        delay(100);
    }
    double average = sum/count;
    BOOST_CHECK_CLOSE(average, 5.0, 10);
}


BOOST_AUTO_TEST_CASE(slowly_changing_pwm_value_reads_back_as_correct_value){
    ActuatorDigital * boolAct = new ActuatorBool();
    ActuatorPwm * pwmAct = new ActuatorPwm(boolAct, 20);

    pwmAct->setValue(0.0);
    ticks_millis_t start = ticks.millis();

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
            csv << "1#set value, 1#read value, 2a#pin" << endl;

    double pwmValue = 50;

    while(ticks.millis() - start < 3000000){ // run for 3000 seconds
        // fluctuate with a period of 1000 seconds around 50 with amplitude 60, so some clipping will occur
        pwmValue = 50.0 - 60.0 * cos(3.14159265*2 * (ticks.millis() - start)/1000000); // starts at zero
        pwmAct->setValue(pwmValue);

        if(pwmValue < 0.1){
            pwmAct->setValue(pwmValue);
        }

        pwmAct->update();
        delay(100);
        csv     << pwmAct->getValue() << ","
                << pwmAct->readValue() << ","
                << boolAct->isActive()
                << endl;
        // maximum from one cylce to the next is maximum derivative * pwm period = 60*2*pi/1000 * 20 = 7.5398
        BOOST_CHECK_LE(abs(double(pwmAct->getValue() - pwmAct->readValue())), 7.5398); // read back value stays within 5% of set value
    }
}


BOOST_AUTO_TEST_CASE(fluctuating_pwm_value_gives_correct_average_with_time_limited_actuator){
    ActuatorDigital * boolAct = new ActuatorBool();
    ActuatorDigital * timeLimitedAct = new ActuatorTimeLimited(boolAct, 2, 5);
    ActuatorPwm * pwmAct = new ActuatorPwm(timeLimitedAct, 20);

    pwmAct->setValue(5.0); // set to a value with duty cycle lower than time limit
    ticks_millis_t start = ticks.millis();

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
            csv << "1#set value, 1#read value, 2a#pin" << endl;

    double pwmValue = 50;

    int count = 0;
    double sum = 0.0;
    double timeHigh = 0.0;
    double timeLow = 0.0;
    ticks_millis_t loopTime = ticks.millis();
    while(ticks.millis() - start < 1000000){ // run for 1000 seconds
        // fluctuate with a period of 200 seconds around 50 with amplitude 50
        pwmValue = 50.0 - 50.0 * cos(3.14159265*2 * (ticks.millis()-start)/200000);
        pwmAct->setValue(pwmValue);

        pwmAct->update();
        delay(100);
        csv     << pwmAct->getValue() << ","
                << pwmAct->readValue() << ","
                << boolAct->isActive()
                << endl;
        count++;
        sum += double(pwmAct->readValue());
        ticks_millis_t prevLoopTime = loopTime;
        loopTime = delay(200);
        if(boolAct->isActive()){
            timeHigh += loopTime - prevLoopTime;
        }
        else{
            timeLow += loopTime - prevLoopTime;
        }
    }
    double readAverage = sum/count;
    double actualDuty = (timeHigh * 100.0) / (timeHigh + timeLow); // rounded result
    BOOST_CHECK_CLOSE(actualDuty, 50.0, 10); // setpoint is fluctuating very fast given time limits. Error of just 5 is good enough.
    BOOST_CHECK_CLOSE(readAverage, actualDuty, 5);

}


BOOST_AUTO_TEST_CASE(decreasing_pwm_value_after_long_high_time_and_mutex_wait){
    ActuatorMutexGroup * mutex = new ActuatorMutexGroup();
    mutex->setDeadTime(100000);

    // actuator that prevents other actuator from going high
    ActuatorDigital * blocker = new ActuatorBool();
    ActuatorMutexDriver * blockerMutex = new ActuatorMutexDriver(blocker, mutex);


    ActuatorDigital * boolAct = new ActuatorBool();
    ActuatorMutexDriver * mutexAct = new ActuatorMutexDriver(boolAct, mutex);
    ActuatorPwm * pwmAct = new ActuatorPwm(mutexAct, 20);

    ticks_millis_t start = ticks.millis();

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#set value, 1#read value, 2a#pin" << endl;

    // trigger dead time of mutex
    blockerMutex->setActive(true);
    mutex->update();
    BOOST_CHECK(blocker->isActive());
    blockerMutex->setActive(false);
    BOOST_CHECK_EQUAL(mutex->getWaitTime(), 100000);


    double pwmValue = 100;

    while(ticks.millis() - start < 1500000){ // run for 1500 seconds
        if(ticks.millis() - start < 100000){
            BOOST_REQUIRE(!boolAct->isActive()); // mutex group dead time keeps actuator low
        }

        pwmAct->setValue(pwmValue);
        mutex->update();
        pwmAct->update();

        if(ticks.millis() - start > 200000){ // start decreasing after 200 s
            pwmValue -= 0.01; // decrease slowly, with 0.1 degree per second
            // maximum difference between history based value and setpoint is 4
            BOOST_CHECK_LE(abs(double(pwmAct->getValue() - pwmAct->readValue())), 4);
        }

        delay(100);
        csv     << pwmAct->getValue() << ","
                << pwmAct->readValue() << ","
                << boolAct->isActive()
                << endl;
    }
}

BOOST_AUTO_TEST_SUITE_END()
