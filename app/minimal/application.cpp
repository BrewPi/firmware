/* Includes ------------------------------------------------------------------*/
#include "application.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

// Define the pins we're going to call pinMode on
int act1 = A0;
int act2 = A1;
int act3 = A6;
int act4 = A7;
int buzz = A2;
const bool invertBuzzer = false;

void setup() {
    pinMode(act1, OUTPUT);
    pinMode(act2, OUTPUT);
    pinMode(act3, OUTPUT);
    pinMode(act4, OUTPUT);
    pinMode(buzz, OUTPUT);
    digitalWrite(buzz, !invertBuzzer);
    delay(200);
    digitalWrite(buzz, invertBuzzer);
}

void loop(){
    int actuators[4] = {act1, act2, act3, act4};
    for(int i=0;i<4;i++){
        digitalWrite(actuators[i], HIGH);
        delay(500);
        digitalWrite(actuators[i], LOW);
        delay(500);
    }
}