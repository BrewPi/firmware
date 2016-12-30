#pragma once
#include "Values.h"
#include "OneWire.h"
#include "Board.h"

/**
 * Provides OneWire services.
 *
 *  Some design choices: since scanning the bus for addresses 
 */
class OneWireBusCBox: public WritableValue {

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
    void outputSearch(DataOut& out) {
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
    virtual void readTo(DataOut& out) {
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
    virtual void writeMaskedFrom(DataIn& dataIn, DataIn& maskIn) {
        // this doesn't support masking since incremental updates make no sense
        // todo - need a way to indicate that masking isn't supported
        // (or it's simply documented int he object description)
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

	static Object* create(ObjectDefinition& defn) {
        return new_object(OneWireBusCBox(oneWirePin));
	}

};

