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

#include "ActuatorPwm.h"
#include "ActuatorTimeLimited.h"
#include <cstring>
#include "runner.h"


temp_t randomIntervalTest(ActuatorPwm* act, ActuatorDigital * target, temp_t duty, int delayMax) {
    act->setValue(duty);
    ticks_millis_t lowToHighTime = ticks.millis();
    ticks_millis_t highToLowTime = ticks.millis();
    ticks_millis_t totalHighTime = 0;
    ticks_millis_t totalLowTime = 0;

    output << format("\n\n*** Results running 100 periods and random 0-%d ms update intervals,"
            "with duty cycle %u and period %u ***\n") % delayMax % duty % act->getPeriod();

    output << "\n\nl->h timestamp  h->l timestamp       high time       low time\n";

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
        output << format("_/  %10u \t \\_ %10u \t") % lowToHighTime % highToLowTime;
        output << format(" %10u ms \t") % highTime;
        do {
            lowToHighTime = random_delay(delayMax);
            act->update();
        } while (!target->isActive());
        ticks_millis_t lowTime = lowToHighTime - highToLowTime;
        if (i > 0) { // skip first cycle in totals, it can have old duty cycle
            totalLowTime += lowTime;
            output << format("%10u ms \n") % lowTime;
        }
        else{
            output << format("%10u ms (ignored)\n") % lowTime;
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
    output << "lowToHigh1:" << lowToHighTime1 << "\t"
           << "highToLow1:" << highToLowTime1 <<" \t lowToHigh2" << lowToHighTime2 << "\n"
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



BOOST_AUTO_TEST_CASE(output_stays_low_with_value_0) {
    ActuatorDigital * target = new ActuatorBool();
    ActuatorPwm * act = new ActuatorPwm(target,4);

    act->setValue(0.0);
    // wait target to go low
    while (target->isActive()) {
        delay(1);
        act->update();
    }
    for (uint32_t i = 0; i < 10 * act->getPeriod(); i++) {
        delay(1);
        act->update();

        BOOST_REQUIRE_MESSAGE(!target->isActive(), "Actuator was high at i=" << i);
    }
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
    for (uint32_t i = 0; i < 10 * act->getPeriod(); i++) {
        delay(1);
        act->update();
        BOOST_REQUIRE_MESSAGE(target->isActive(), "Actuator was low at i=" << i);
    }
}


BOOST_AUTO_TEST_CASE(ActuatorPWM_with_min_max_time_limited_OnOffActuator_as_driver){
    // test with minimum ON of 2 seconds, minimum off of 5 seconds and period 5 seconds

    srand(time(NULL));
    ActuatorDigital * vAct = new ActuatorBool();
    ActuatorDigital * onOffAct = new ActuatorTimeLimited(vAct, 2, 5);
    ActuatorPwm * act = new ActuatorPwm(onOffAct, 10);

    // Test that average duty cycle is correct, even with minimum times enforced in the actuator
    BOOST_CHECK_EQUAL(randomIntervalTest(act, vAct, 50.0, 500), temp_t(50.0));
    BOOST_CHECK_EQUAL(randomIntervalTest(act, vAct, 3.0, 500), temp_t(3.0));
    BOOST_CHECK_EQUAL(randomIntervalTest(act, vAct, 1.0, 500), temp_t(1.0));
    // BOOST_CHECK_EQUAL(randomIntervalTest(act, vAct, 99.0, 500), temp(99.0)); // 99 not attainable due to minimum OFF time
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
    BOOST_CHECK_CLOSE(avgDuty, 50.0, 0.5);
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
    BOOST_CHECK_CLOSE(avgDuty, 50.0, 0.5);
}

BOOST_AUTO_TEST_SUITE_END()
