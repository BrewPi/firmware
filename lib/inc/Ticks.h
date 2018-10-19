/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of the BrewBlox Control Library.
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

#pragma once

#include "TicksTypes.h"
#include <ctime>

template <typename Impl>
class Ticks {
    Impl impl;
    ticks_seconds_t& utcStart;

public:
    Ticks(ticks_seconds_t& utcStartRef)
        : impl()
        , utcStart(utcStartRef)
    {
    }

    inline ticks_millis_t millis()
    {
        return impl.millis();
    }
    inline ticks_micros_t micros()
    {
        return impl.micros();
    }
    inline ticks_seconds_t seconds()
    {
        return impl.seconds();
    }
    inline ticks_seconds_t secondsSince(ticks_seconds_t timeStamp)
    {
        return ::secondsSince(seconds(), timeStamp);
    }
    inline ticks_millis_t millisSince(ticks_millis_t timeStamp)
    {
        return ::millisSince(millis(), timeStamp);
    }

    void setNow(const ticks_seconds_t& utcNow)
    {
        utcStart = utcNow - seconds();
    }

    ticks_seconds_t getNow()
    {
        return utcStart + seconds();
    }

    struct tm calendarTime()
    {
        struct tm* calendar_time;
        calendar_time = getNow();
        calendar_time->tm_year += 1900;
        return *calendar_time;
    }
};
