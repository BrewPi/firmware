
#include "strnlen.h"
#include <string.h>


size_t strnlen( const char* start, size_t maxlen )
{
	const char* end = (const char *)memchr(start, '\0', maxlen);
	return (end) ? (size_t)(end - start) : maxlen;
}

