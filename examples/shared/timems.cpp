/*
 * Copyright 2013 BrewPi/Elco Jacobs.
 * Copyright 2013 Matthew McGowan.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "stdint.h"
#include "timems.h"

#if defined(__WIN32__)

#include <windows.h>

inline msec_t time_ms(void)
{
    return GetTickCount();
}

#else

#include <sys/time.h>

msec_t time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (msec_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

#endif


static msec_t startupMillis = time_ms();

msec_t millisSinceStartup()
{
    return time_ms()-startupMillis;
}
