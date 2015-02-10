/* Includes ------------------------------------------------------------------*/
#include "application.h"

extern "C" {
#include "d4d.h"
}

SYSTEM_MODE(SEMI_AUTOMATIC);

// Define the pins we're going to call pinMode on
int act1 = A0;
int act2 = A1;
int act3 = A6;
int buzz = A2;

D4D_EXTERN_SCREEN(screen_helloworld);

void setup() {
    pinMode(act1, OUTPUT);
    pinMode(act2, OUTPUT);
    pinMode(act3, OUTPUT);
    pinMode(buzz, OUTPUT);
    digitalWrite(buzz, LOW);

    Serial.begin(57600);
    
    if (!D4D_Init(&screen_helloworld)) {
        // D4D initialization failed
        Serial.println("eGUI/D4D initialization failed");
        return;
    }

    D4D_SetOrientation(D4D_ORIENT_LANDSCAPE);
}

void loop() {
    D4D_Poll();
    delay(10);
}
