#pragma SPARK_NO_PREPROCESSOR
#include "application.h"
/*
 * This example shows how to create a repeating timer using FreeRTOS
 * xTimerCreate() and xTimerStart() functions.
 *
 * A timer is created that calls the `timerCallback` function every second.
 * The callback function simply increments a counter and writes the current count to serial,
 * as well as blinking the D7 led.
 *
 * Note that when you use freertos API calls directly, the code will not be
 * portable to the Photon or Electron.
 *
 * We will be releasing high-level Wiring APIs for timer functionality
 * in the future which will be compatible across all platforms.
 */

/* Includes ------------------------------------------------------------------*/
#include "freertos4core/freertos4core.h"

TimerHandle_t timer;

void timerCallback(TimerHandle_t handle)
{
    static int count = 0;
    digitalWrite(D7, !digitalRead(D7));
    Serial.println(count++);
}

void createTimer()
{
    timer = xTimerCreate("timer", 500, true, NULL, timerCallback);
    xTimerStart(timer, 0);
}

/* This function is called once at start up ----------------------------------*/
void setup()
{
    Serial.begin(9600);
    pinMode(D7, OUTPUT);
    createTimer();
}

/* This function loops forever --------------------------------------------*/
void loop()
{
    delay(30000);
}
