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
    uint8_t position = getPosition();

    if(desiredAction != action){
        write(desiredAction);
    }

    if((desiredAction == VALVE_OPENING && position == VALVE_OPENED) ||
            (desiredAction == VALVE_CLOSING && position == VALVE_CLOSED)){
        // fully opened/closed. Stop driving the valve
        idle();
    }
}

void ValveController::write(uint8_t action) {
    desiredAction = action;
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

uint8_t ValveController::getPosition() const {
    if(!device->isConnected()){
        return VALVE_ERROR;
    }
    uint8_t states = device->readPios(true);
    if(output == 0){
        states = states >> 4;
    }
    return states & 0b11;
}

uint8_t ValveController::getAction() const {
    uint8_t latches = device->readLatches(true);
    if(output == 0){ // A is on upper bits
        latches = latches >> 4;
    }
    return (latches >> 2) & 0b11;
}

void ValveController::setState(State state, int8_t priority){
    if(state == State::Active){
        open();
    }
    else {
        close();
    }
}

ActuatorDigital::State ValveController::getState() const {
    if(!device->isConnected()){
        return State::Unknown;
    }
    if(getAction() == VALVE_OPENING){
        return State::Active;
    }
    if(getAction() == VALVE_CLOSING){
    	return State::Inactive;
    }
    if(getPosition() == VALVE_OPENED){
    	return State::Active;
    }
    if(getPosition() == VALVE_CLOSED){
    	return State::Inactive;
    }
    // If we end up here, the valve is halfway or disconnected form the board (the feedback switches have a pullup).
    // If the valve is half open, this is an active state.
    // Returning true here is safer, because the caller will call setActive(false) to recover from this.
    // If the default would be false, the valve could be half open or unknown, but it will not get a close signal.
    return State::Unknown;
}

uint8_t ValveController::read(bool doUpdate){
    if(doUpdate){
        device->update();
    }
    return (getAction() << 2 | getPosition());
}

#endif
