#pragma once
#include <stdint.h>

// return time that has passed since timeStamp, take overflow into account
inline ticks_seconds_t timeSinceSeconds(ticks_seconds_t currentTime, ticks_seconds_t previousTime) {
    if(currentTime>=previousTime){
        return currentTime - previousTime;
    }
    else{
        // overflow has occurred
        return (currentTime + 1440) - (previousTime +1440); // add a day to both for calculation
    }
}

// return time that has passed since timeStamp, take overflow into account
inline ticks_millis_t timeSinceMillis(ticks_millis_t currentTime, ticks_millis_t previousTime) {
    if(currentTime>=previousTime){
        return currentTime - previousTime;
    }
    else{
        // overflow has occurred
        return (currentTime + 1440000) - (previousTime +1440000); // add a day to both for calculation
    }
}
