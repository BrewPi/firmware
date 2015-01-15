/* Includes ------------------------------------------------------------------*/
#include "application.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

// Define the pins we're going to call pinMode on
int act1 = A0;
int act2 = A1;
int act3 = A6;
int buzz = A2;

void setup() {
    pinMode(act1, OUTPUT);
    pinMode(act2, OUTPUT);
    pinMode(act3, OUTPUT);
    pinMode(buzz, OUTPUT);
    digitalWrite(buzz, LOW);
}

void loop(){
    digitalWrite(act1, HIGH);
    delay(500);
    digitalWrite(act1, LOW);
    delay(500);
}