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

#include "ValveController.h"

void
ValveController::update()
{
    if (auto devPtr = m_device()) {
        devPtr->update();
    }
    uint8_t act = action();
    uint8_t pos = position();

    if (m_desiredAction != act) {
        write(m_desiredAction);
    }

    if ((m_desiredAction == VALVE_OPENING && pos == VALVE_OPENED) || (m_desiredAction == VALVE_CLOSING && pos == VALVE_CLOSED)) {
        // fully opened/closed. Stop driving the valve
        idle();
    }
}

void
ValveController::write(uint8_t newAction)
{
    m_desiredAction = newAction;
    if (auto devPtr = m_device()) {
        uint8_t latch = devPtr->getLatchCache();
        devPtr->update();

        newAction = newAction & 0b11; // make sure action only has lower 2 bits non-zero

        if (m_output == 'A') { // A is on upper bits
            latch = latch & 0b00111111;
            latch = latch | newAction << 6;
        } else {
            latch = latch & 0b11110011;
            latch = latch | newAction << 2;
        }
        latch |= 0b00110011; // make sure latch of input stays off at all times
        devPtr->writeLatches(latch);
    }
}

uint8_t
ValveController::position() const
{
    if (auto devPtr = m_device()) {
        if (devPtr->connected()) {
            uint8_t states = devPtr->readPios(true);
            if (m_output == 'A') {
                states = states >> 4;
            }
            return states & 0b11;
        }
    }
    return VALVE_ERROR;
}

uint8_t
ValveController::action() const
{
    if (auto devPtr = m_device()) {
        uint8_t latches = devPtr->readLatches(true);
        if (m_output == 'A') { // A is on upper bits
            latches = latches >> 4;
        }
        return (latches >> 2) & 0b11;
    }
    return VALVE_IDLE_INIT;
}

void
ValveController::state(const State& state)
{
    if (state == State::Active) {
        open();
    } else {
        close();
    }
}

ActuatorDigital::State
ValveController::state() const
{
    if (auto devPtr = m_device()) {
        if (!devPtr->connected()) {
            return State::Unknown;
        }
        if (action() == VALVE_OPENING) {
            return State::Active;
        }
        if (action() == VALVE_CLOSING) {
            return State::Inactive;
        }
        if (position() == VALVE_OPENED) {
            return State::Active;
        }
        if (position() == VALVE_CLOSED) {
            return State::Inactive;
        }
    }
    // If we end up here, the valve is halfway or disconnected form the board (the feedback switches have a pullup).
    // If the valve is half open, this is an active state.
    // Returning true here is safer, because the caller will call setActive(false) to recover from this.
    // If the default would be false, the valve could be half open or unknown, but it will not get a close signal.
    return State::Unknown;
}

uint8_t
ValveController::read(bool doUpdate)
{
    if (doUpdate) {
        if (auto devPtr = m_device()) {
            devPtr->update();
        }
    }
    return (action() << 2 | action());
}
