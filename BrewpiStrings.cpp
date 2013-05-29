/*
 * BrewpiStrings.cpp
 *
 * Created: 29/05/2013 17:29:33
 *  Author: mat
 */ 

#include "Brewpi.h"
#include "BrewpiStrings.h"

// some useful strings
const char STR_FMT_S[] PROGMEM = "%s";
const char STR_FMT_U[] PROGMEM = "%u";
const char STR_FMT_D[] PROGMEM = "%d";
const char STR_SPACES[] PROGMEM = "             ";



#if !indexOf_inline
int8_t indexOf(const char* s, char c)
{
	char c2;
	int8_t idx = -1;
	while ((c2=s[++idx]))
	{
		if (c==c2)
		return idx;
	}
	return -1;
}
#endif


