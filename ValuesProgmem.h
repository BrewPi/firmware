#pragma once

/*
 * ValuesProgmem.h
 *
 * Created: 06/02/2014 10:35:19
 *  Author: mat
 */ 

#include "Values.h"


class ProgmemStringValue : public Value
{	
	private:
		const char* value;
	
	public:
		ProgmemStringValue(const char* v) : value(v)  {}

		virtual void readTo(DataOut& out);
		virtual uint8_t streamSize();
				
};


