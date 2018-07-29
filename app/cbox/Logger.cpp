#include "Logger.h"
#include "Comms.h"
#include <stdarg.h>

void printFormatted(const char *fmt, ... ){
    char buf[128];

    va_list args;
    va_start (args, fmt );
    int written = vsnprintf(buf, 127, fmt, args);
    va_end (args);
    cbox::comms.dataOut().writeBuffer(buf, written);
}


void BrewPiLogger::logMessageVaArg(char type, LOG_ID_TYPE errorID, const char * varTypes, ...){
    va_list args;
    cbox::comms.dataOut().write('<');
    cbox::comms.dataOut().write('!');
    printFormatted("%c,", type);
    printFormatted("%d,", errorID);

    va_start (args, varTypes);
    uint8_t index = 0;

    while(varTypes[index]){
        switch(varTypes[index]){
            case 'd': // integer, signed or unsigned
                printFormatted("%d", va_arg(args, int));
                break;
            case 's': // string
                printFormatted("%s", va_arg(args, char*));
                break;
            case 't': // temperature in fixed point format
                char buf[12];
                printFormatted("%s", (*(temp_t *) va_arg(args,void*)).toString(buf, 3, 12));
                break;
            case 'a': // onewire address, print as hex
                printFormatted("%08x", va_arg(args, uint64_t));
                break;
        }
        if(varTypes[++index]){
        	cbox::comms.dataOut().write(',');
        }
    }
    va_end (args);
    cbox::comms.dataOut().write('>');
}

