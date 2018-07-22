/*
 * Copyright 2017 BrewPi
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

#include "TicksObject.h"

extern MockTicks baseticks;
static cbox::ScaledTicksValue<MockTicks> ticks(baseticks);

class MockDelay {
public:
    MockDelay() {}
    void seconds(uint16_t seconds){
        baseticks.advance(ticks_millis_t(1000) * ticks_millis_t(seconds));
    }
    void millis(uint16_t millis){
        baseticks.advance(millis);
    }
    void microseconds(uint32_t micros) {}
};

typedef MockDelay DelayImpl;
typedef MockTicks TicksImpl;
#define DELAY_IMPL_CONFIG // empty define

extern DelayImpl wait;
