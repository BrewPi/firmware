#include "application.h"

SYSTEM_MODE(SEMI_AUTOMATIC)

void handleReset(bool exit) 
{ 
    if (exit)
        System.reset();
}


