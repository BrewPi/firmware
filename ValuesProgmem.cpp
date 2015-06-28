
#include "ValuesProgmem.h"
#include "Platform.h"

#ifdef ARDUINO
#include "avr/pgmspace.h"
#endif

void ProgmemStringValue::readTo(DataOut& out) {
	const char* v = value;
	uint8_t b;
	do {
		b = pgm_read_byte(v++);
		out.write(b);
	} while (b);
}

uint8_t ProgmemStringValue::streamSize() {
	return strlen_P(value);
}
