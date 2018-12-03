/*
 * Copyright 2018 BrewPi
 *
 * This file is part of BrewBlox.
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
#include <stdint.h>

struct D4D_SCREEN;

class DisplayClass {
private:
    static D4D_SCREEN* onTimeoutScreen;
    static D4D_SCREEN* nextScreen;
    static ticks_millis_t screenStartTime;

public:
    DisplayClass();
    ~DisplayClass() = default;

    /**
     * Initialize the UI components.
     */
    static uint8_t init();

    /**
     * Display the startup page and retrieve how long the startup page should be displayed 
     * for in milliseconds before moving on to the main page.
     */
    static uint32_t showStartupPage();

    /**
     * Called as often as possible by the application. Allows the UI to perform
     * quick background tasks, such as reading the touch screen.
     */
    static void tick();

    /**
     * Called from the main loop to update the current page and perform event
     * processing.
     */
    static void update(const ticks_millis_t& now);
};

DisplayClass&
displayRef()
{
    static DisplayClass d;
    return d;
}

#define Display displayRef()