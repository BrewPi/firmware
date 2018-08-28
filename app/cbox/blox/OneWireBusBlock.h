#pragma once

#include "Block.h"
#include "Board.h"
#include "ObjectBase.h"
#include "OneWire.h"
#include "OneWireAddress.h"
#include "OneWireBus.pb.h"
#include "nanopb_callbacks.h"
#include <limits.h>

class OneWireBusBlock : public cbox::ObjectBase<OneWireBusBlock> { // not a block type, because it doesn't need to implement Interface*
private:
    OneWire& bus;

    mutable _blox_OneWireBus_Command command; // declared mutable so const streamTo functions can reset it

    static const uint8_t NO_OP = 0;
    static const uint8_t RESET = 1;
    static const uint8_t SEARCH = 2; // pass family as data, 00 for all

protected:
    // stream result of a bus search, with arg pointing to the onewire bus
    static bool streamAdresses(pb_ostream_t* stream, const pb_field_t* field, void* const* arg)
    {
        OneWireAddress address;
        OneWire* busPtr = (OneWire*)*arg;
        if (busPtr == nullptr) {
            return false;
        }
        while (busPtr->search(address.asUint8ptr())) {
            if (!pb_encode_tag_for_field(stream, field)) {
                return false;
            }
            uint64_t addr = uint64_t(address);
            if (!pb_encode_fixed64(stream, &addr)) {
                return false;
            }
        }
        return true;
    }

public:
    OneWireBusBlock(OneWire& ow)
        : bus(ow)
        , command({NO_OP, 0})
    {
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
    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final
    {
        blox_OneWireBus message = {0};
        message.command = command;
        message.address.funcs.encode = nullptr;
        message.address.arg = &bus;
        switch (command.opcode) {
        case NO_OP:
            break;
        case RESET:
            bus.reset();
            break;
        case SEARCH:
            bus.reset_search();
            if (command.data) {
                bus.target_search(command.data);
            }
            message.address.funcs.encode = &streamAdresses;
            break;
        }
        // commands are one-shot - once the command is done clear it.
        command.opcode = NO_OP;
        command.data = 0;
        return streamProtoTo(out, &message, blox_OneWireBus_fields, std::numeric_limits<size_t>::max());
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
    virtual cbox::CboxError streamFrom(cbox::DataIn& dataIn) override final
    {
        blox_OneWireBus message;

        cbox::CboxError res = streamProtoFrom(dataIn, &message, blox_OneWireBus_fields, std::numeric_limits<size_t>::max());
        /* if no errors occur, write new settings to wrapped object */
        if (res == cbox::CboxError::OK) {
            command = message.command;
        }
        return res;
    }

    virtual cbox::CboxError streamPersistedTo(cbox::DataOut& out) const override final
    {
        return cbox::CboxError::OK;
    }

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        // No updates for now. Alternatively, a periodic bus scan for new devices?
        return update_never(now);
    }
};
