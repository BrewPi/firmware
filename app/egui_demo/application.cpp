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
int act4 = A7;
int buzz = A2;

extern "C" {
D4D_EXTERN_SCREEN(screen_entry);
}

void setup() {
    pinMode(act1, OUTPUT);
    pinMode(act2, OUTPUT);
    pinMode(act3, OUTPUT);
    pinMode(act4, OUTPUT);
    pinMode(buzz, OUTPUT);
    
    // beep, beep
    digitalWrite(buzz, LOW);
    delay(50);
    digitalWrite(buzz, HIGH);
    delay(50);
    digitalWrite(buzz, LOW);
    delay(50);
    digitalWrite(buzz, HIGH);
    
    Serial.begin(57600);

    if (!D4D_Init(&screen_entry)) {
        // D4D initialization failed
        Serial.println("eGUI/D4D initialization failed");
        return;
    }
    D4D_SetOrientation(D4D_ORIENT_LANDSCAPE);
    D4D_CheckTouchScreen();
    D4D_CalibrateTouchScreen();
    D4D_Poll();    
}

unsigned long tic = 0;
unsigned long toc = 0;
unsigned int count = 0;
unsigned long ticTocSum = 0;

void loop() {
    // move this to Ticks later
    D4D_TimeTickPut();
    D4D_CheckTouchScreen();
    D4D_Poll();
    
    // Serial print performance
    tic = toc;
    toc = micros();
    ticTocSum += toc-tic;
    count++;
    // print loop time every 10 loops
    if(count >= 10){
        Serial.print("Avg loop time (us):");
        Serial.println(ticTocSum / 10);
        count = 0;
        ticTocSum = 0;
    }
}
