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

#pragma once
#include "Values.h"
#include "OneWire.h"
#include "Board.h"

/**
 * Provides OneWire services.
 *
 *  Some design choices: since scanning the bus for addresses 
 */
class OneWireBusCBox: public cbox::WritableValue {

    OneWire bus;
    uint8_t command_id, command_data;

    static const uint8_t NO_OP = 0;
    static const uint8_t RESET = 1;
    static const uint8_t SEARCH = 2;
    static const uint8_t SEARCH_FAMILY = 3;

protected:

    /**
     * Scans the onewire bus and writes the found addresses to the output stream.
     */
    void outputSearch(cbox::DataOut& out) {
        uint8_t address[8];
        memset(address, 0, 8);
        while (bus.search(address)) {
            for (uint8_t i=0; i<8; i++) {
                out.write(address[i]);
            }
        }
    }

public:
    OneWireBusCBox(int pin=oneWirePin) : bus(pin), command_id(NO_OP), command_data(0) {
    		bus.init();
    }

    OneWire& oneWire() { return bus; }

    /**
     * Read value
     * - an ID encoded as a variable length ID chain (values > 0x80 mean there is more data)
     * - previous command data count - N
     * - previous command data - N bytes
     * - command rsult (variable length)
     * - cmd 00: no-op always 00 (success)
     * - cmd 01: reset bus (00 on success, FF on failure)
     * - cmd 02: search bus: a sequence of 0 or more 8-byte addresses, MSB first that were found on the bus
     */
    virtual void readTo(cbox::DataOut& out) {
        switch (command_id) {
            case NO_OP:
                out.write(0);
                break;
            case RESET:
                bus.reset();
                out.write(0);
                break;
            case SEARCH:
                bus.reset_search();
                outputSearch(out);
                break;
            case SEARCH_FAMILY:
                bus.target_search(command_data);
                outputSearch(out);
        }
        // commands are one-shot - once the command is done clear it.
        command_id = NO_OP;
    }

    virtual uint8_t readStreamSize() {
        return 0;   // we don't know the size - it's variable
    }

    /**
     * Write value 
     * - byte 0: command
     *   00: no-op
     *   01: reset bus
     *   02: search bus: 
     *   03: search the bus, limiting to the given family code byte
     *   (later: search bus alarm state)
     *   (later: set bus power (off if next byte is 00, on if it's 01) )
     */
    virtual void writeFrom(cbox::DataIn& dataIn) {
        uint8_t command_id = dataIn.next();
        uint8_t command_data = 0;
        switch (command_id) {
            case SEARCH_FAMILY:
                command_data = dataIn.next();
                break;
        }
        this->command_id = command_id;
        this->command_data = command_data;
    }

	static cbox::Object* create(cbox::ObjectDefinition& defn) {
        return new_object(OneWireBusCBox(oneWirePin));
	}

};

