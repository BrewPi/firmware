#pragma once

#include <stddef.h>

#ifdef __cpluplus
extern "C" {
#endif

#ifdef WIN32
// MinGW doesn't provide strnlen
size_t strnlen( const char* start, size_t maxlen );
#else
#error windows
#endif


#ifdef __cpluplus
}
#endif
