#include "catch.hpp"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time, to seed rand */

#include "ActuatorPwm.h"
#include "ActuatorOnOff.h"
#include "Ticks.h"
#include <cstring>

#include "main.h"


uint8_t randomIntervalTest(ActuatorPwm* act, Actuator * target, uint8_t duty, int delayMax) {
    act->setPwm(duty);
    ticks_millis_t lowToHighTime = ticks.millis();
    ticks_millis_t highToLowTime = ticks.millis();
    ticks_millis_t totalHighTime = 0;
    ticks_millis_t totalLowTime = 0;

    printf("\n\n*** Results running 100 periods and random 0-%d ms update intervals,"
            "with duty cycle %u and period %u ***\n",
            delayMax, duty, act->getPeriod());

    printf("\n\nl->h timestamp  h->l timestamp       high time       low time\n");
    // run for 100 periods
    for (int i = 0; i < 100; i++) {
        do {
            highToLowTime = random_delay(delayMax);
            act->updatePwm();
        } while (target->isActive());
        ticks_millis_t highTime = highToLowTime - lowToHighTime;
        if (i > 0) { // skip first cycle in totals, it can be incomplete
            totalHighTime += highTime;
        }
        printf("_/  %10u \t \\_ %10u \t", lowToHighTime, highToLowTime);
        printf(" %10u ms \t", highTime);
        do {
            lowToHighTime = random_delay(delayMax);
            act->updatePwm();
        } while (!target->isActive());
        ticks_millis_t lowTime = lowToHighTime - highToLowTime;
        if (i > 0) { // skip first cycle in totals, it can have old duty cycle
            totalLowTime += lowTime;
            printf("%10u ms \n", lowTime);
        }
        else{
            printf("%10u ms (ignored)\n", lowTime);
        }
        
    }
    double avgDuty = double(totalHighTime) / (totalHighTime + totalLowTime);
    int avgDutyInt = int(round(avgDuty * 255));
    printf("total high time: %10u\ntotal low time:  %10u \navg duty: %7.3f = %u/255\n",
            totalHighTime, totalLowTime, avgDuty, avgDutyInt);
    return avgDutyInt;
}

TEST_CASE("Test ActuatorPWM class with ValueActuator as driver", "[actuatorpwm]") {
    srand(time(NULL));
    Actuator * target = new ValueActuator();
    ActuatorPwm * act = new ActuatorPwm(target,4);
    // intToTemp is a macro to initialize temperatures in Celsius

    SECTION("PWM value is initialized to 0") {
        REQUIRE(act->getPwm() == 0);
    }

    SECTION("Test PWM can be set and read") {
        act->setPwm(100);
        REQUIRE(act->getPwm() == 100);
        act->setPwm(255);
        REQUIRE(act->getPwm() == 255);
        act->setPwm(0);
        REQUIRE(act->getPwm() == 0);
    }

    SECTION("Test ticks.millis() to increment by 1 every call") {
        printf("Testing time:");
        ticks_millis_t start = ticks.millis();
        ticks_millis_t time;
        for (ticks_millis_t i = start + 1; i <= start + 10; i++) {
            time = ticks.millis();
            printf("% d", time);
            REQUIRE(i == time);
        }
        printf(".\n");
    }

    SECTION("Test that on and off time matches duty cycle when updating every ms") {
        const uint8_t duty = 100;
        act->setPwm(duty);
        ticks_millis_t lowToHighTime1 = ticks.millis();
        ticks_millis_t highToLowTime1 = ticks.millis();
        ticks_millis_t lowToHighTime2 = ticks.millis();
        do {
            lowToHighTime1 = delay(1);
            act->updatePwm();
        } while (!target->isActive());
        do {
            highToLowTime1 = delay(1);
            act->updatePwm();
        } while (target->isActive());
        do {
            lowToHighTime2 = delay(1);
            act->updatePwm();
        } while (!target->isActive());

        ticks_millis_t timeHigh = highToLowTime1 - lowToHighTime1;
        ticks_millis_t timeLow = lowToHighTime2 - highToLowTime1;
        ticks_millis_t actualDuty = (timeHigh * 255 + (timeHigh + timeLow)/2) / (timeHigh + timeLow); // rounded result
        printf("*** Timestamps testing one period with duty cycle %d and period %d***\n", duty, act->getPeriod());
        printf("lowToHigh1: %d \t highToLowTime1: %d \t lowToHigh2 %d \n", lowToHighTime1, highToLowTime1, lowToHighTime2);
        printf("time high: %d \t time low: %d \t actual duty cycle: %d\n", timeHigh, timeLow, actualDuty);
        REQUIRE(actualDuty == 100);
    }

    SECTION("Test that average duty cycle is correct, even with random update intervals") {
        REQUIRE((int) randomIntervalTest(act, target, 100, 500) == 100);
        REQUIRE((int) randomIntervalTest(act, target, 5, 500) == 5);
        REQUIRE((int) randomIntervalTest(act, target, 1, 500) == 1);
        REQUIRE((int) randomIntervalTest(act, target, 254, 500) == 254);
    }

    SECTION("Test output stays low with PWM value 0") {
        act->setPwm(0);
        // wait target to go low
        while (target->isActive()) {
            delay(1);
            act->updatePwm();
        }
        for (uint32_t i = 0; i < 10 * act->getPeriod(); i++) {
            delay(1);
            act->updatePwm();
            if (target->isActive()) {
                REQUIRE_FALSE(target->isActive());
            } // prevents many assertions
            // REQUIRE_FALSE(target->isActive());
        }
    }

    SECTION("Test output stays high with PWM value 255") {
        act->setPwm(255);
        // wait for target to go high
        while (!target->isActive()) {
            delay(1);
            act->updatePwm();
        }
        for (uint32_t i = 0; i < 10 * act->getPeriod(); i++) {
            delay(1);
            act->updatePwm();
            if (!target->isActive()) {
                REQUIRE(target->isActive());
            } // prevents many assertions
            // REQUIRE_TRUE(target->isActive());
        }
    }
}

TEST_CASE("Test ActuatorPWM class with min/max time limited On/Off actuator as driver", "[actuatorpwmonoff]") {
    // test with minimum ON of 2 seconds, minimum off of 5 seconds and period 5 seconds

    srand(time(NULL));
    Actuator * vAct = new ValueActuator();
    Actuator * onOffAct = new ActuatorOnOff(vAct, 2, 5);
    ActuatorPwm * act = new ActuatorPwm(onOffAct, 10);

    SECTION("Test that average duty cycle is correct, even with minimum times") {
        REQUIRE((int) randomIntervalTest(act, vAct, 100, 500) == 100);
        REQUIRE((int) randomIntervalTest(act, vAct, 5, 500) == 5);
        REQUIRE((int) randomIntervalTest(act, vAct, 1, 500) == 1);
//        REQUIRE((int) randomIntervalTest(act, vAct, 254, 500) == 254); // 254 not attainable due to minimum OFF time
    }
}

TEST_CASE("Ramping PWM up faster than period gives correct average", "[actuatorpwmonoff]") {
    Actuator * vAct = new ValueActuator();
    ActuatorPwm * act = new ActuatorPwm(vAct, 20);
    ticks_seconds_t timeHigh = 0;
    ticks_seconds_t timeLow = 0;
    int actualDuty;

    for(int ramps = 0; ramps <100; ramps++){ // enough ramps to not be affected by time window
        for(int i = 100; i <= 200; i++){
            act->setPwm(i);
            for(int j = 0; j < 100; j++){ // 10 seconds total
                delay(100);
                act->updatePwm();
                if(vAct->isActive()){
                    timeHigh++;
                }
                else{
                    timeLow++;
                }
            }
        }
    }
    actualDuty = (timeHigh * 255 + (timeHigh + timeLow)/2) / (timeHigh + timeLow); // rounded result
    CHECK(actualDuty == 150);
}

TEST_CASE("Ramping PWM down faster than period gives correct average", "[actuatorpwmonoff]") {
    Actuator * vAct = new ValueActuator();
    ActuatorPwm * act = new ActuatorPwm(vAct, 20);
    ticks_seconds_t timeHigh = 0;
    ticks_seconds_t timeLow = 0;
    int actualDuty;

    for(int ramps = 0; ramps <100; ramps++){ // enough ramps to not be affected by time window
        for(int i = 200; i >= 100; i--){
            act->setPwm(i);
            for(int j = 0; j < 100; j++){ // 10 seconds total
                act->updatePwm();
                delay(100);
                if(vAct->isActive()){
                    timeHigh++;
                }
                else{
                    timeLow++;
                }
            }
        }
    }
    actualDuty = (timeHigh * 255 + (timeHigh + timeLow)/2) / (timeHigh + timeLow); // rounded result
    CHECK(actualDuty == 150);
}

