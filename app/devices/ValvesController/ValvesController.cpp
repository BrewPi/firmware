/* 
 * File:   ValvesController.cpp
 * Author: Elco
 * 
 * Created on 16 december 2014, 15:28
 */

#include "ValvesController.h"

ValvesController::ValvesController() {
}

ValvesController::~ValvesController() {
}

/*
 * Updates the status of the member variables from what is read back from the valve
 * Checks whether the valves are done with opening/closing and stops driving them
 */

void ValvesController::update() {
    uint8_t switchState = accessRead();
    // content of switchState:
    // bit 7-6: Valve A action: 01 = open, 10 = close, 11 = off, 00 = off but LEDS on
    // bit 5-4: Valve A status: 01 = opened, 10 = closed, 11 = in between
    // bit 3-2: Valve B action: 01 = open, 10 = close, 11 = off, 00 = off but LEDS on
    // bit 1-0: Valve B status: 01 = opened, 10 = closed, 11 = in between

    actA = (switchState >> 6) & 0x3;
    senseA = (switchState >> 4) & 0x3;
    actB = (switchState >> 2) & 0x3;
    senseB = switchState & 0x3;

    if (actA == senseA) {
        // fully opened/closed. Stop driving the valve
        actA = 0b11;
    }
    if (actB == senseB) {
        // fully opened/closed. Stop driving the valve
        actB = 0b11;
    }
    uint8_t newAct = (actA << 6) + (actB << 2);
    if (newAct != (switchState & 0b11001100)) {
        accessWrite(newAct | 0b00110011); // write new state, but ensure keep sense bits as inputs
    }
}

uint8_t ValvesController::read(uint8_t valve, bool doUpdate) {
    if (doUpdate) {
        update();
    }
    return (valve == 0) ? senseA : senseB;
}

void ValvesController::write(uint8_t valve, uint8_t state) {
    update();
    switch (valve) {
        case 0:
            accessWrite(0b00110011 | (state << 6) | (actB << 2));
            break;
        case 1:
            accessWrite(0b00110011 | (actA << 6) | (state << 2));
            break;
    }
}

void ValvesController::open(uint8_t valve) {

    write(valve, OPEN);
}

void ValvesController::close(uint8_t valve) {

    write(valve, CLOSE);
}

void ValvesController::stop(uint8_t valve) {
    write(valve, OFF);
}