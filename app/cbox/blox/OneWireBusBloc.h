#pragma once

#include "Values.h"
#include "OneWire.h"
#include "Board.h"
#include "OneWireBus.pb.h"
#include "nanopb_callbacks.h"
#include "assert_size_helper.h"

class OneWireBusBloc: public WritableValue {
private:

    OneWire bus;

    _blox_OneWireCommand command;

    static const uint8_t NO_OP = 0;
    static const uint8_t RESET = 1;
    static const uint8_t SEARCH = 2; // pass family as data, 00 for all

protected:

    // stream result of a bus search, with arg pointing to the onewire bus
    static bool streamAdresses(pb_ostream_t *stream, const pb_field_t *field, void * const *arg){
        uint8_t address[8] = {0};
        OneWire * busPtr = (OneWire *) *arg;
        if(busPtr == nullptr){
            return false;
        }
        while (busPtr->search(address)) {
            if(!pb_encode_tag_for_field(stream, field)){
                return false;
            }
            if(!pb_encode_string(stream, address, 8)){
                return false;
            }
        }
        return true;
    }

public:
    OneWireBusBloc(int pin=oneWirePin) : bus(pin), command({NO_OP, 0}) {
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
    virtual void readTo(DataOut& out) override final{
        blox_OneWireRead message = {0};
        message.lastCommand = command;
        message.address.funcs.encode = nullptr;
        message.address.arg = &bus;
        switch (command.command) {
            case NO_OP:
                break;
            case RESET:
                bus.reset();
                break;
            case SEARCH:
                bus.reset_search();
                if(command.data){
                    bus.target_search(command.data);
                }
                message.address.funcs.encode = &streamAdresses;
                break;
        }
        pb_ostream_t stream = { &dataOutStreamCallback, &out, SIZE_MAX, 0 };
        pb_encode_delimited(&stream, blox_OneWireRead_fields, &message);

        // commands are one-shot - once the command is done clear it.
        command.command = NO_OP;
        command.data = 0;
    }

    virtual uint8_t readStreamSize() override final {
        return 0;   // we don't know the size - it's variable
    }

    /**
     * Set the command to be executed next form the input stream
     * - byte 0: command
     *   00: no-op
     *   01: reset bus
     *   02: search bus:
     *   03: search the bus, limiting to the given family code byte passed as data
     *   (later: search bus alarm state?)
     *   (later: set bus power? (off if next byte is 00, on if it's 01) )
     */
    virtual void writeFrom(DataIn& dataIn) override final{
        blox_OneWireCommand message = command;

        assert_size<sizeof(message), 2>(); // one byte for command, one for data

        pb_istream_t stream = { &dataInStreamCallback, &dataIn, blox_OneWireCommand_size + 1};
        bool success = pb_decode_delimited_noinit(&stream, blox_OneWireCommand_fields, &message);
        /* if no errors occur, write new settings to wrapped object */
        if(success){
            command = message;
        }
    }

    static Object* create(ObjectDefinition& defn) {
        return new_object(OneWireBusBloc(oneWirePin));
    }
};
