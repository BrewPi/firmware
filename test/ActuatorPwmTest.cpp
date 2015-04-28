#include "catch.hpp"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time, to seed rand */

#include "ActuatorPwm.h"
#include "Ticks.h"
#include <cstring>
// delay ms miliseconds and return current time afterwards

ticks_millis_t delay(int ms) {
    ticks_millis_t newTime;
    for (int i = 0; i < ms; i++) {
        newTime = ticks.millis();
    }
    return newTime;
}

// delay random nr of milliseconds with maximum max

ticks_millis_t random_delay(int max) {
    int ms = rand() % (max - 1);
    return delay(ms);
}

TEST_CASE("Test ActuatorPWM class with ValueActuator as driver", "[actuatorpwm]") {
    srand(time(NULL));
    Actuator * v = new ValueActuator();
    ActuatorPwm act(v, 0);
    // intToTemp is a macro to initialize temperatures in Celsius

    SECTION("PWM value is initialized to 0") {
        REQUIRE(act.getPwm() == 0);
    }

    SECTION("Test PWM can be set and read") {
        act.setPwm(100);
        REQUIRE(act.getPwm() == 100);
        act.setPwm(255);
        REQUIRE(act.getPwm() == 255);
        act.setPwm(0);
        REQUIRE(act.getPwm() == 0);
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
        act.setPwm(duty);
        ticks_millis_t lowToHighTime1 = ticks.millis();
        ticks_millis_t highToLowTime1 = ticks.millis();
        ticks_millis_t lowToHighTime2 = ticks.millis();
        while (!act.isActive()) {
            lowToHighTime1 = delay(1);
            act.updatePwm();
        }
        while (act.isActive()) {
            highToLowTime1 = delay(1);
            act.updatePwm();
        }
        while (!act.isActive()) {
            lowToHighTime2 = delay(1);
            act.updatePwm();
        }
        ticks_millis_t timeHigh = highToLowTime1 - lowToHighTime1;
        ticks_millis_t timeLow = lowToHighTime2 - highToLowTime1;
        ticks_millis_t actualDuty = timeHigh * 255 / (timeHigh + timeLow);
        printf("*** Timestamps testing one period with duty cycle %d and period %d***\n", duty, act.getPeriod());
        printf("lowToHigh1: %d \t highToLowTime1: %d \t lowToHigh2 %d \n", lowToHighTime1, highToLowTime1, lowToHighTime2);
        printf("time high: %d \t time low: %d \t actual duty cycle: %d\n", timeHigh, timeLow, actualDuty);
        REQUIRE(actualDuty == 100);
    }

    SECTION("Test that average duty cycle is correct, even with random update intervals"){
        const uint8_t duty = 100;
        act.setPwm(duty);
        ticks_millis_t lowToHighTime = ticks.millis();
        ticks_millis_t highToLowTime = ticks.millis();
        ticks_millis_t totalHighTime = 0;
        ticks_millis_t totalLowTime = 0;
        
        printf("*** Results running 100 periods and random update intervals,"
        "with duty cycle %u and period %u ***\n",
        duty, act.getPeriod());
        
        // wait for new period
        while (act.isActive()) {
            highToLowTime = delay(1);
            act.updatePwm();
        }
        while (!act.isActive()) {
            lowToHighTime = delay(1);
            act.updatePwm();
        }
        // run for 100 periods
        for (int i = 0; i < 100; i++) {
            act.updatePwm();
            while (act.isActive()) {
                highToLowTime = random_delay(200);
                act.updatePwm();
            }
            ticks_millis_t highTime = highToLowTime - lowToHighTime;
            totalHighTime += highTime;
            printf("high: %u ms \t", highTime);
            while (!act.isActive()) {
                lowToHighTime = random_delay(200);
                act.updatePwm();
            }
            ticks_millis_t lowTime = lowToHighTime - highToLowTime;
            totalLowTime += lowTime;
            printf("low: %u ms \n", lowTime);
        }
        double avgDuty = double(totalHighTime) / (totalHighTime + totalLowTime);
        int avgDutyInt = int(round(avgDuty*255));
        printf("total high time: %u \t total low time: %u \t avg duty: %0.3f = %u/255\n",
                totalHighTime, totalLowTime, avgDuty, avgDutyInt);
        REQUIRE(avgDutyInt == act.getPwm());
    }
}
