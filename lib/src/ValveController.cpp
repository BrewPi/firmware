/*
 * Copyright 2015 BrewPi/Elco Jacobs.
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

#include "Platform.h"

#if BREWPI_DS2408
#include "ValveController.h"


void ValveController::update() {
    device->update();

    uint8_t action = getAction();
    uint8_t state = getState();

    if((action == VALVE_OPENING && state == VALVE_OPENED) ||
            (action == VALVE_CLOSING && state == VALVE_CLOSED)){
        // fully opened/closed. Stop driving the valve
        idle();
    }
}


void ValveController::write(uint8_t action) {
    update();
    uint8_t latch = device->getLatchCache();
    action = action & 0b11; // make sure action only has lower 2 bits non-zero

    if(output == 0){ // A is on upper bits
        latch = latch & 0b00111111;
        latch = latch | action << 6;
    }
    else{
        latch = latch & 0b11110011;
        latch = latch | action << 2;
    }
    latch |= 0b00110011; // make sure latch of input stays off at all times
    device->writeLatches(latch);
}

#endif
