#pragma once

#include "EepromAwareWritableValue.h"
#include "SetPointSimple.pb.h"
#include "../nanopb_callbacks.h"
#include "SetPoint.h"

class SetPointSimpleCbox: public EepromAwareWritableValue {
private:
    SetPointSimple setpoint;

public:
    SetPointSimpleCbox() {
    }

    virtual void readTo(DataOut& out) override {
        pb_ostream_t stream = { &dataOutStreamCallback, &out, readStreamSize(), 0 };
        bool status = pb_encode_delimited(&stream, blox_SetPointSimple_fields, setpoint.settingsPtr());
    }

    static const uint8_t maxSize(){
        return blox_SetPointSimple_size + 1; // +1 for varint for length in delimited message
    }

    virtual uint8_t readStreamSize() override {
        return maxSize();
    }

    virtual void writeMaskedFrom(DataIn& dataIn, DataIn& maskIn) override {
        pb_istream_t stream = { &dataInStreamCallback, &dataIn, readStreamSize(), 0 };
        bool status = pb_decode_delimited_noinit(&stream, blox_SetPointSimple_fields, setpoint.settingsPtr());
        if(status){
            storeSettings();
        }
    }

    static Object* create(ObjectDefinition& defn) {
        auto obj = new_object(SetPointSimpleCbox);
        pb_istream_t stream = { &dataInStreamCallback, defn.in, defn.len, 0 };
        bool status = pb_decode_delimited_noinit(&stream, blox_SetPointSimple_fields, obj->setpoint.settingsPtr());
        return obj;
    }

    bool storeSettings(){
        if(eeprom_offset() == eptr_t(-1)){
            return false; // EEPROM location is not set
        }
        eptr_t offset = eeprom_offset();
        pb_ostream_t stream = { &eepromOutStreamCallback, &offset, readStreamSize(), 0 };
        bool status = pb_encode_delimited(&stream, blox_SetPointSimple_fields, setpoint.settingsPtr());

        return status;
    }

    SetPointSimple& get() {
        return setpoint;
    }
};
