#include "application.h"

extern "C" unsigned long millisC(void){
    return millis();
}

extern "C" unsigned long microsC(void){
    return micros();
}

