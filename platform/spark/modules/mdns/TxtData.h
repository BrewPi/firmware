#include "application.h"

#ifndef _INCL_TXTDATA
#define _INCL_TXTDATA

#include "Buffer.h"

class TxtData {
    public:
        bool addEntry(String key, String value);

        void write(Buffer * buffer);

    private:

        uint8_t * data;
        uint16_t size = 0;
};

#endif
