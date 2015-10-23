#include "application.h"

#ifndef _INCL_LABEL
#define _INCL_LABEL

#include "Buffer.h"

#define DOT '.'

#define END_OF_NAME 0x0
#define LABEL_POINTER 0xc0
#define MAX_LABEL_SIZE 63
#define INVALID_OFFSET -1

#define UNKNOWN_NAME -1
#define BUFFER_UNDERFLOW -2

class Label {
    private:

        class Iterator;

    public:
        class Matcher {
            public:
                Matcher(Label ** labels, uint8_t labelCount);

                int8_t match(Buffer * buffer);

                String getLastName();

            private:
                Label ** labels;
                uint8_t labelCount;

                String lastName;
        };

        Label(String name, Label * nextLabel = NULL, bool caseSensitive = false);

        uint8_t getSize();

        uint8_t getWriteSize();

        void write(Buffer * buffer);

        void reset();

    private:
        class Reader {
            public:
                Reader(Buffer * buffer);

                bool hasNext();

                uint8_t next();

                bool endOfName();
            private:
                Buffer * buffer;
                uint8_t c = 1;
        };

        class Iterator {
            public:
                Iterator(Label * label);

                void match(uint8_t c);

                bool matched();

            private:
                Label * label;
                uint8_t size;
                uint8_t offset = 0;
                bool matches = true;

                bool equalsIgnoreCase(uint8_t c);
        };

        uint8_t * EMPTY_DATA = { END_OF_NAME };
        uint8_t * data;
        bool caseSensitive;
        Label * nextLabel;
        int16_t writeOffset = INVALID_OFFSET;
};

#endif
