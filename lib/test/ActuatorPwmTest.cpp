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

#include "ActuatorInterfaces.h"
#include "ActuatorMocks.h"
#include "ActuatorPwm.h"
#include "ActuatorTimeLimited.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorMutexGroup.h"
#include <cstring>
#include "runner.h"
#include <iostream>
#include <fstream>
#include <cmath> // for sin

#define PRINT_TOGGLE_TIMES 0

double randomIntervalTest(ActuatorPwm & act, ActuatorDigital & target, temp_t duty, int delayMax) {
    act.set(duty);
    ticks_millis_t lowToHighTime = ticks.millis();
    ticks_millis_t highToLowTime = ticks.millis();
    ticks_millis_t totalHighTime = 0;
    ticks_millis_t totalLowTime = 0;

    *output << format("\n\n*** Results running 100 periods and random 0-%d ms update intervals,"
            "with duty cycle %u and period %u ***\n") % delayMax % duty % act.getPeriod();
#if PRINT_TOGGLE_TIMES
    *output << "\n\nl.h timestamp  h->l timestamp       high time       low time\n";
#endif
    // run for 100 periods
    for (int i = 0; i < 100; i++) {
        do {
            highToLowTime = random_delay(delayMax);
            act.update();
        } while (target.getState() == ActuatorDigital::State::Active);
        ticks_millis_t highTime = highToLowTime - lowToHighTime;
        if (i > 0) { // skip first cycle in totals, it can be incomplete
            totalHighTime += highTime;
        }
#if PRINT_TOGGLE_TIMES
        *output << format("_/  %10u \t \\_ %10u \t") % lowToHighTime % highToLowTime;
        *output << format(" %10u ms \t") % highTime;
#endif
        do {
            lowToHighTime = random_delay(delayMax);
            act.update();
        } while (target.getState() == ActuatorDigital::State::Inactive);
        ticks_millis_t lowTime = lowToHighTime - highToLowTime;
        if (i > 0) { // skip first cycle in totals, it can have old duty cycle
            totalLowTime += lowTime;
#if PRINT_TOGGLE_TIMES
            *output << format("%10u ms \n") % lowTime;
#endif
        }
        else{
#if PRINT_TOGGLE_TIMES
            *output << format("%10u ms (ignored)\n") % lowTime;
#endif
        }
        
    }
    double avgDuty = double(totalHighTime) / (totalHighTime + totalLowTime) * double(act.max());
    *output << "total high time: " << totalHighTime << "\n"
           << "total low time: " << totalLowTime << "\n"
           << "avg duty: " << avgDuty << "/" << act.max() << "\n";
    return avgDuty;
}

BOOST_AUTO_TEST_SUITE(ActuatorPWM)

BOOST_AUTO_TEST_CASE( Test_ActuatorPWM_with_ValueActuator_as_driver) {
    auto target = ActuatorBool();
    auto act = ActuatorPwm(target,4);

    BOOST_CHECK(act.setting() == temp_t(0.0)); // PWM value is initialized to 0

    // Test that PWM can be set and read
    act.set(50.0);
    BOOST_CHECK_EQUAL(act.setting(), temp_t(50.0));
    act.set(100.0);
    BOOST_CHECK_EQUAL(act.setting(), temp_t(100.0));
    act.set(0.0);
    BOOST_CHECK_EQUAL(act.setting(), temp_t(0.0));

    act.set(110.0);
    BOOST_CHECK_EQUAL(act.setting(), temp_t(100.0)); // max is 100

    act.set(-50.0);
    BOOST_CHECK_EQUAL(act.setting(), temp_t(0.0)); // min is 0
}

BOOST_AUTO_TEST_CASE(on_off_time_matches_duty_cycle_when_updating_every_ms) {
    srand(time(NULL));
    auto target = ActuatorBool();
    auto act = ActuatorPwm(target,4);

    temp_t duty = 50.0;
    act.set(duty);
    ticks_millis_t lowToHighTime1 = ticks.millis();
    ticks_millis_t highToLowTime1 = ticks.millis();
    ticks_millis_t lowToHighTime2 = ticks.millis();
    do {
        lowToHighTime1 = delay(1);
        act.update();
    } while (target.getState() == ActuatorDigital::State::Inactive);
    do {
        highToLowTime1 = delay(1);
        act.update();
    } while (target.getState() == ActuatorDigital::State::Active);
    do {
        lowToHighTime2 = delay(1);
        act.update();
    } while (target.getState() == ActuatorDigital::State::Inactive);

    ticks_millis_t timeHigh = highToLowTime1 - lowToHighTime1;
    ticks_millis_t timeLow = lowToHighTime2 - highToLowTime1;
    double actualDuty = (timeHigh * 100.0) / (timeHigh + timeLow); // rounded result
    *output << "*** Timestamps testing one period with duty cycle " << duty << " and period " << act.getPeriod() << "***\n";
    *output << "lowToHigh1: " << lowToHighTime1 << "\t"
            << "highToLow1: " << highToLowTime1 <<" \t lowToHigh2: " << lowToHighTime2 << "\n"
            << "time high: " << timeHigh << "\t"
            << "time low: " << timeLow << "\t"
            << "actual duty cycle: " << actualDuty;
    BOOST_CHECK_CLOSE(actualDuty, 50.0, 0.5);
}

BOOST_AUTO_TEST_CASE(average_duty_cycle_is_correct_with_random_update_intervals) {
    srand(time(NULL));
    auto target = ActuatorBool();
    auto act = ActuatorPwm(target,4);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 50.0, 500), 50.0, 2); // between 49 and 51
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 3.0, 500), 3.0, 16.7); // between 2.5 and 3.5
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 1.0, 500), 1.0, 50); // between 0.5 and 1.5
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 99.0, 500), 99.0, 0.5); // between 98.5 and 99.5
}

BOOST_AUTO_TEST_CASE(average_duty_cycle_is_correct_with_long_period) {
    srand(time(NULL));
    auto target = ActuatorBool();
    auto act = ActuatorPwm(target,3600);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 50.0, 500), 50.0, 1);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 3.0, 500), 3.0, 16.7);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 1.0, 500), 1.0, 50);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, target, 99.0, 500), 99.0, 0.5);
}



BOOST_AUTO_TEST_CASE(output_stays_low_with_value_0) {
    auto target = ActuatorBool();
    auto act = ActuatorPwm(target,4);

    act.set(0.0);
    // wait target to go low
	while (target.getState() == ActuatorDigital::State::Active) {
        delay(1);
        act.update();
    }
    for (uint32_t i = 0; i < 10 * act.getPeriod() * 1000; i++) {
        delay(1);
        act.update();

        BOOST_REQUIRE_MESSAGE(target.getState() == ActuatorDigital::State::Inactive, "Actuator was high at i=" << i);
    }
}

BOOST_AUTO_TEST_CASE(on_big_positive_changes_shortened_cycle_has_correct_value) {
    auto vAct = ActuatorBool();
    auto limited = ActuatorTimeLimited(vAct, 0, 0);
    auto act = ActuatorPwm(limited, 100); // period is 100 seconds

    act.set(short(30));
    ticks_millis_t start = ticks.millis();
    ticks_millis_t periodStart;
    while(ticks.millis() - start < 250000) { // 250 seconds
    	ActuatorDigital::State oldState = vAct.getState();
        act.update();
        ActuatorDigital::State newState = vAct.getState();
        if(oldState == ActuatorDigital::State::Inactive &&
        		newState == ActuatorDigital::State::Active){ // low to high transition
            periodStart = ticks.millis();
        }
        delay(1000);
    }

    BOOST_CHECK(vAct.getState() == ActuatorDigital::State::Inactive); // actuator is inactive, ~50 seconds into 3rd cycle
    act.set(50.0);
    while (vAct.getState()  == ActuatorDigital::State::Inactive) {
        delay(1000);
        act.update();
    }
    // cycle should be shortened to 60 seconds, 30 high + 30 low
    BOOST_CHECK_CLOSE(double(ticks.millis() - periodStart), 60000, 3);
    periodStart = ticks.millis();

    while(vAct.getState() == ActuatorDigital::State::Active){
        delay(1000);
        act.update();
    }
    // next high time should be normal
    BOOST_CHECK_CLOSE(double(ticks.millis() - periodStart), 50000, 1); // actuator turned on for 50 seconds

    while(vAct.getState() == ActuatorDigital::State::Inactive){
        delay(1000);
        act.update();
    }
    // next low time should be normal
    BOOST_CHECK_CLOSE(double(ticks.millis() - periodStart), 100000, 1); // actuator turned on for 50 seconds
}


BOOST_AUTO_TEST_CASE(on_big_negative_changes_go_low_immediately) {
    auto vAct = ActuatorBool();
    auto limited = ActuatorTimeLimited(vAct, 0, 0);
    auto act = ActuatorPwm(limited, 100); // period is 100 seconds

    ticks_millis_t lastLowTimeBeforeChange = ticks.millis();
    act.set(60.0);
    for (uint32_t i = 0; i < 250 ; i++) { // 250 seconds
        delay(1000);
        act.update();
        if(vAct.getState() == ActuatorDigital::State::Inactive){
            lastLowTimeBeforeChange = ticks.millis();
        }
    }

    BOOST_CHECK(vAct.getState() == ActuatorDigital::State::Active); // actuator is active
    act.set(30.0);
    act.update();
    BOOST_CHECK(vAct.getState() == ActuatorDigital::State::Inactive); // actuator turns off immediately

    ticks_millis_t highTolowTime = ticks.millis();
    ticks_millis_t highPeriod = highTolowTime - lastLowTimeBeforeChange;

    while(vAct.getState() == ActuatorDigital::State::Inactive){
        delay(100);
        act.update();
    }
    ticks_millis_t lowToHighTime = ticks.millis();
    ticks_millis_t lowPeriod = lowToHighTime - highTolowTime;
    // check that this cycle has normal duration
    BOOST_CHECK_CLOSE(double(highPeriod + lowPeriod), 100000, 2);

    // but overshooting the high value is compensated in high period of next cycle
    while(vAct.getState() == ActuatorDigital::State::Active){
        delay(100);
        act.update();
    }
    ticks_millis_t reducedHighPeriod = ticks.millis() - lowToHighTime;

    BOOST_CHECK_CLOSE(double(highPeriod + reducedHighPeriod), 0.3*2*100000, 2);
}


BOOST_AUTO_TEST_CASE(output_stays_high_with_value_100) {
    auto target = ActuatorBool();
    auto act = ActuatorPwm(target,4);

    act.set(100.0);
    // wait for target to go high
    while (target.getState() == ActuatorDigital::State::Inactive) {
        delay(1);
        act.update();
    }
    for (uint32_t i = 0; i < 10 * act.getPeriod() * 1000; i++) {
        delay(1);
        act.update();
        BOOST_REQUIRE_MESSAGE(target.getState() == ActuatorDigital::State::Active, "Actuator was low at i=" << i);
    }
}


BOOST_AUTO_TEST_CASE(ActuatorPWM_with_min_max_time_limited_OnOffActuator_as_driver){
    // test with minimum ON of 2 seconds, minimum off of 5 seconds and period 10 seconds

    srand(time(NULL));
    auto vAct = ActuatorBool();
    auto onOffAct = ActuatorTimeLimited(vAct, 2, 5);
    auto act = ActuatorPwm(onOffAct, 10);

    // Test that average duty cycle is correct, even with minimum times enforced in the actuator
    BOOST_CHECK_CLOSE(randomIntervalTest(act, vAct, 50.0, 500), 50.0, 1);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, vAct, 3.0, 500), 3.0, 16.7);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, vAct, 1.0, 500), 1.0, 50);
    BOOST_CHECK_CLOSE(randomIntervalTest(act, vAct, 99.0, 500), 99.0, 0.5);
}


BOOST_AUTO_TEST_CASE(when_switching_between_zero_and_low_value_average_is_correct){
    // test with minimum ON of 2 seconds, minimum off of 5 seconds and period 5 seconds
    auto vAct = ActuatorBool();
    auto onOffAct = ActuatorTimeLimited(vAct, 20, 50);
    auto act = ActuatorPwm(onOffAct, 100);

    ticks_seconds_t timeHigh = 0;
    ticks_seconds_t timeLow = 0;

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#value, 2a#pin" << endl;

    for(int cycles = 0; cycles < 100; cycles++){
        if(cycles %2 == 0){
            act.set(4.0); // under minimum ON time of 20, doesn't trigger skipping ahead
        }
        else{
            act.set(0.0);
        }
        for(int i = 0; i < 180; i++){ // 180 seconds, not full periods on purpose
            delay(1000);
            act.update();
            if(vAct.getState() == ActuatorDigital::State::Active){
                timeHigh++;
            }
            else{
                timeLow++;
            }
            csv     << act.setting() << "," // setpoint
                    << vAct.getState() // actual cooler pin state
                    << endl;
        }
    }

    csv.close();

    double avgDuty = double(timeHigh) * 100.0 / (timeHigh + timeLow);
    BOOST_CHECK_CLOSE(avgDuty, 2, 25); // value is between 1.5 and 2.5
}


BOOST_AUTO_TEST_CASE(ramping_PWM_up_faster_than_period_gives_correct_average){
    auto vAct = ActuatorBool();
    auto act = ActuatorPwm(vAct, 20);
    ticks_seconds_t timeHigh = 0;
    ticks_seconds_t timeLow = 0;

    for(int ramps = 0; ramps <100; ramps++){ // enough ramps to not be affected by time window
        for(temp_t v = temp_t(40.0); v <= temp_t(60.0); v = v + temp_t(0.25)){
            act.set(v);
            for(int j = 0; j < 100; j++){ // 10 seconds total
                delay(100);
                act.update();
                if(vAct.getState() == ActuatorDigital::State::Active){
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
    auto vAct = ActuatorBool();
    auto act = ActuatorPwm(vAct, 20);
    ticks_seconds_t timeHigh = 0;
    ticks_seconds_t timeLow = 0;

    for(int ramps = 0; ramps <100; ramps++){ // enough ramps to not be affected by time window
        for(temp_t v = temp_t(60.0); v >= temp_t(40.0); v = v - temp_t(0.25)){
          act.set(v);
          for(int j = 0; j < 100; j++){ // 10 seconds total
              delay(100);
              act.update();
              if(vAct.getState() == ActuatorDigital::State::Active){
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

BOOST_AUTO_TEST_CASE(two_mutex_PWM_actuators_can_overlap_with_equal_duty){
    auto mutex = ActuatorMutexGroup();
    auto boolAct1 = ActuatorBool();
    auto mutexAct1 = ActuatorMutexDriver(boolAct1, mutex);
    auto act1 = ActuatorPwm(mutexAct1, 10);

    auto boolAct2 = ActuatorBool();
    auto mutexAct2 = ActuatorMutexDriver(boolAct2, mutex);
    auto act2 = ActuatorPwm(mutexAct2, 10);

    mutex.setDeadTime(0);

    ticks_seconds_t timeHigh1 = 0;
    ticks_seconds_t timeLow1 = 0;
    ticks_seconds_t timeHigh2 = 0;
    ticks_seconds_t timeLow2 = 0;

    act1.set(20.0);
    act2.set(20.0);

    act1.update();
    act2.update();
    ticks_millis_t start = ticks.millis();

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
        csv << "1a#pin1, 1a#pin2" << endl;

    while(ticks.millis() - start <= 100000){ // run for 100 seconds
        act1.update();
        act2.update();
        mutex.update();
        if(boolAct1.getState() == ActuatorDigital::State::Active){
            timeHigh1++;
        }
        else{
            timeLow1++;
        }
        if(boolAct2.getState() == ActuatorDigital::State::Active){
            timeHigh2++;
        }
        else{
            timeLow2++;
        }
        BOOST_REQUIRE(!(boolAct1.getState() == ActuatorDigital::State::Active &&
        		boolAct2.getState() == ActuatorDigital::State::Active)); // not active at the same time
        csv     << boolAct1.getState() << ","
                << boolAct2.getState()
                << endl;
        delay(100);
    }

    double avgDuty1 = double(timeHigh1) * 100.0 / (timeHigh1 + timeLow1);
    BOOST_CHECK_CLOSE(avgDuty1, 20.0, 2); // small error possible due to test window influence

    double avgDuty2 = double(timeHigh2) * 100.0 / (timeHigh2 + timeLow2);
    BOOST_CHECK_CLOSE(avgDuty2, 20.0, 2); // small error possible due to test window influence
}

BOOST_AUTO_TEST_CASE(two_mutex_PWM_actuators_can_overlap_with_different_duty){
    auto mutex = ActuatorMutexGroup();
    auto boolAct1 = ActuatorBool();
    auto mutexAct1 = ActuatorMutexDriver(boolAct1, mutex);
    auto act1 = ActuatorPwm(mutexAct1, 10);

    auto boolAct2 = ActuatorBool();
    auto mutexAct2 = ActuatorMutexDriver(boolAct2, mutex);
    auto act2 = ActuatorPwm(mutexAct2, 10);

    mutex.setDeadTime(0);

    ticks_seconds_t timeHigh1 = 0;
    ticks_seconds_t timeLow1 = 0;
    ticks_seconds_t timeHigh2 = 0;
    ticks_seconds_t timeLow2 = 0;

    act1.set(60.0);
    act2.set(20.0);

    act1.update();
    act2.update();
    ticks_millis_t start = ticks.millis();

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
        csv << "1a#pin1, 1a#pin2" << endl;

    while(ticks.millis() - start <= 100000){ // run for 100 seconds
        act1.update();
        act2.update();
        mutex.update();
        if(boolAct1.getState() == ActuatorDigital::State::Active){
            timeHigh1++;
        }
        else{
            timeLow1++;
        }
        if(boolAct2.getState() == ActuatorDigital::State::Active){
            timeHigh2++;
        }
        else{
            timeLow2++;
        }
        BOOST_REQUIRE(!(boolAct1.getState() == ActuatorDigital::State::Active &&
                		boolAct2.getState() == ActuatorDigital::State::Active)); // not active at the same time
        csv     << boolAct1.getState() << ","
                << boolAct2.getState()
                << endl;
        delay(100);
    }

    double avgDuty1 = double(timeHigh1) * 100.0 / (timeHigh1 + timeLow1);
    BOOST_CHECK_CLOSE(avgDuty1, 60.0, 2); // small error possible due to test window influence

    double avgDuty2 = double(timeHigh2) * 100.0 / (timeHigh2 + timeLow2);
    BOOST_CHECK_CLOSE(avgDuty2, 20.0, 2); // small error possible due to test window influence
}

BOOST_AUTO_TEST_CASE(mutex_actuator_which_cannot_go_active_cannot_block_other_actuator){
    auto mutex = ActuatorMutexGroup();
    auto boolAct1 = ActuatorBool();
    auto mutexAct1 = ActuatorMutexDriver(boolAct1, mutex);
    auto act1 = ActuatorPwm(mutexAct1, 10);

    auto boolAct2 = ActuatorNop(); // actuator which can never go active
    auto mutexAct2 = ActuatorMutexDriver(boolAct2, mutex);
    auto act2 = ActuatorPwm(mutexAct2, 10);

    mutex.setDeadTime(0);

    ticks_seconds_t timeHigh1 = 0;
    ticks_seconds_t timeLow1 = 0;
    ticks_seconds_t timeHigh2 = 0;
    ticks_seconds_t timeLow2 = 0;

    act1.set(20.0);
    act2.set(40.0);  // <-- act2 will have higher priority due to higher duty cycle

    act1.update();
    act2.update();
    ticks_millis_t start = ticks.millis();

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
        csv << "1a#pin1, 1a#pin2" << endl;

    while(ticks.millis() - start <= 100000){ // run for 100 seconds
        act1.update();
        act2.update();
        mutex.update();
        if(boolAct1.getState() == ActuatorDigital::State::Active){
            timeHigh1++;
        }
        else{
            timeLow1++;
        }
        if(boolAct2.getState() == ActuatorDigital::State::Active){
            timeHigh2++;
        }
        else{
            timeLow2++;
        }
        BOOST_REQUIRE(!(boolAct1.getState() == ActuatorDigital::State::Active &&
			boolAct2.getState() == ActuatorDigital::State::Active)); // not active at the same time
        csv     << boolAct1.getState() << ","
                << boolAct2.getState()
                << endl;
        delay(100);
    }

    double avgDuty1 = double(timeHigh1) * 100.0 / (timeHigh1 + timeLow1);
    BOOST_CHECK_CLOSE(avgDuty1, 20.0, 2); // small error possible due to test window influence

    double avgDuty2 = double(timeHigh2) * 100.0 / (timeHigh2 + timeLow2);
    BOOST_CHECK_CLOSE(avgDuty2, 0.0, 2); // Nop actuator cannot go active
}

BOOST_AUTO_TEST_CASE(actual_value_returned_by_ActuatorPwm_readValue_is_correct){
    auto boolAct = ActuatorBool();
    auto pwmAct = ActuatorPwm(boolAct, 20);

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#set value, 1#read value, 2a#pin" << endl;

    pwmAct.set(20.0);
    ticks_millis_t start = ticks.millis();
    while(ticks.millis() - start < 200000){ // run for 200 seconds to dial in cycle time
        pwmAct.update();
        delay(100);
    }

    start = ticks.millis();

    int count = 0;
    double sum = 0.0;
    while(ticks.millis() - start < 500000){ // run for 500 seconds
        pwmAct.update();
        csv     << pwmAct.setting() << ","
                << pwmAct.value() << ","
                << boolAct.getState()
                << endl;
        count++;
        sum += double(pwmAct.value());
        delay(100);
    }
    double average = sum/count;
    BOOST_CHECK_CLOSE(average, 20.0, 1);
}


BOOST_AUTO_TEST_CASE(actual_value_returned_by_ActuatorPwm_readValue_is_correct_with_time_limited_actuator){
    auto boolAct = ActuatorBool();
    auto timeLimitedAct = ActuatorTimeLimited(boolAct, 2, 5);
    auto pwmAct = ActuatorPwm(timeLimitedAct, 20);

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#set value, 1#read value, 2a#pin" << endl;

    pwmAct.set(5.0); // set to a value with duty cycle lower than time limit

    ticks_millis_t start = ticks.millis();
    while(ticks.millis() - start < 100000){ // run for 100 seconds to dial in cycle time
        pwmAct.update();
        delay(100);
    }

    start = ticks.millis();

    int count = 0;
    double sum = 0.0;
    while(ticks.millis() - start < 1000000){ // run for 1000 seconds
        pwmAct.update();
        csv     << pwmAct.setting() << ","
                << pwmAct.value() << ","
                << boolAct.getState()
                << endl;
        count++;
        sum += double(pwmAct.value());
        delay(100);
    }
    double average = sum/count;
    BOOST_CHECK_CLOSE(average, 5.0, 10);
}


BOOST_AUTO_TEST_CASE(slowly_changing_pwm_value_reads_back_as_correct_value){
    auto boolAct = ActuatorBool();
    auto pwmAct = ActuatorPwm(boolAct, 20);

    pwmAct.set(0.0);
    ticks_millis_t start = ticks.millis();

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
            csv << "1#set value, 1#read value, 2a#pin" << endl;

    double pwmValue = 50;

    while(ticks.millis() - start < 3000000){ // run for 3000 seconds
        // fluctuate with a period of 1000 seconds around 50 with amplitude 60, so some clipping will occur
        pwmValue = 50.0 - 60.0 * cos(3.14159265*2 * (ticks.millis() - start)/1000000); // starts at zero
        pwmAct.set(pwmValue);

        if(pwmValue < 0.1){
            pwmAct.set(pwmValue);
        }

        pwmAct.update();
        delay(100);
        csv     << pwmAct.setting() << ","
                << pwmAct.value() << ","
                << boolAct.getState()
                << endl;
        // maximum from one cylce to the next is maximum derivative * pwm period = 60*2*pi/1000 * 20 = 7.5398
        BOOST_REQUIRE_LE(abs(double(pwmAct.setting() - pwmAct.value())), 7.5398); // read back value stays within 5% of set value
    }
}


BOOST_AUTO_TEST_CASE(fluctuating_pwm_value_gives_correct_average_with_time_limited_actuator){
    auto boolAct = ActuatorBool();
    auto timeLimitedAct = ActuatorTimeLimited(boolAct, 2, 5);
    auto pwmAct = ActuatorPwm(timeLimitedAct, 20);

    pwmAct.set(5.0); // set to a value with duty cycle lower than time limit
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
        pwmAct.set(pwmValue);

        pwmAct.update();
        delay(100);
        csv     << pwmAct.setting() << ","
                << pwmAct.value() << ","
                << boolAct.getState()
                << endl;
        count++;
        sum += double(pwmAct.value());
        ticks_millis_t prevLoopTime = loopTime;
        loopTime = delay(200);
        if(boolAct.getState() == ActuatorDigital::State::Active){
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
    auto mutex = ActuatorMutexGroup();
    mutex.setDeadTime(100000);

    // actuator that prevents other actuator from going high
    auto blocker = ActuatorBool();
    auto blockerMutex = ActuatorMutexDriver(blocker, mutex);


    auto boolAct = ActuatorBool();
    auto mutexAct = ActuatorMutexDriver(boolAct, mutex);
    auto pwmAct = ActuatorPwm(mutexAct, 20);

    ticks_millis_t start = ticks.millis();

    ofstream csv("./test_results/" + boost_test_name() + ".csv");
    csv << "1#set value, 1#read value, 2a#pin" << endl;

    // trigger dead time of mutex
    blockerMutex.setState(ActuatorDigital::State::Active);
    mutex.update();
    BOOST_CHECK(blocker.getState() == ActuatorDigital::State::Active);
    blockerMutex.setState(ActuatorDigital::State::Inactive);
    BOOST_CHECK_EQUAL(mutex.getWaitTime(), 100000u);


    double pwmValue = 100;

    while(ticks.millis() - start < 1500000){ // run for 1500 seconds
        if(ticks.millis() - start < 100000){
            BOOST_REQUIRE(boolAct.getState() == ActuatorDigital::State::Inactive); // mutex group dead time keeps actuator low
        }

        pwmAct.set(pwmValue);
        mutex.update();
        pwmAct.update();

        if(ticks.millis() - start > 200000){ // start decreasing after 200 s
            pwmValue -= 0.01; // decrease slowly, with 0.1 degree per second
            // maximum difference between history based value and setpoint is 4
            BOOST_REQUIRE_LE(abs(double(pwmAct.setting() - pwmAct.value())), 4);
        }

        delay(100);
        csv     << pwmAct.setting() << ","
                << pwmAct.value() << ","
                << boolAct.getState()
                << endl;
    }
}

BOOST_AUTO_TEST_SUITE_END()
