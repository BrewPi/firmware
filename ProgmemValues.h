#pragma once

/*
 * ValuesProgmem.h
 *
 * Created: 06/02/2014 10:35:19
 *  Author: mat
 */ 

#include "pgmspace.h"
#include "Values.h"

typedef PCSTR const char*;

class ProgmemStringValue : public Readable<PCSTR>, public AbstractValue 
{	
	private:
		PCSTR value;
	
	public:

		PCSTR read() {
			return value;
		}
	
		void stream(DataOut& out) {
			PCSTR v = value;
			do {
				uint8_t v = pgm_read_byte(v++);
				out.write(v);
			} while (v);
		}
	
};


