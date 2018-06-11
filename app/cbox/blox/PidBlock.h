#pragma once

#include "Block.h"
#include "Pid.pb.h"
#include "Pid.h"

#include "ProcessValueDelegate.h"
#include "CboxLink.h"

class PidBlock: public Block {
private:
    CboxLookup inputLookup;
    CboxLookup outputLookup;

    ProcessValueDelegate input;
    ProcessValueDelegate output;

    Pid pid;

public:
    PidBlock() :
        input(inputLookup),
        output(outputLookup),
        pid(input, output)
    {}

    static const uint8_t persistedMaxSize(){
        static_assert(blox_Pid_Persisted_size < 128, "varint for settings size will be larger than 1 byte");
        return blox_Pid_Persisted_size + 1;
    }
    virtual uint8_t readStreamSize() override final {
        /* maximum size of settings  +1 for varint for length in delimited message */
        return persistedMaxSize();
    }

    virtual void writeFrom(cbox::DataIn& dataIn) override final {
        writeFromImpl(dataIn, true);
    }

    void copyPersistedTo(blox_Pid_Persisted & to){
        to.settings = copy_struct_as<blox_Pid_Settings>(pid.getSettings());
        inputLookup.copyTo(to.links.input);
        outputLookup.copyTo(to.links.output);
        to.filtering.input = pid.getInputFiltering();
        to.filtering.derivative = pid.getDerivativeFiltering();
    }

    void copyPersistedFrom(blox_Pid_Persisted & from){
        pid.setSettings(copy_struct_as<Pid::Settings>(from.settings));
        inputLookup.copyFrom(from.links.input);
        outputLookup.copyFrom(from.links.output);
        pid.setInputFiltering(from.filtering.input);
        pid.setDerivativeFiltering(from.filtering.derivative);
    }

    void writeFromImpl(cbox::DataIn& dataIn, bool storeToEeprom){
        blox_Pid_Persisted newData;
        size_t maxSize = persistedMaxSize();
        pb_istream_t stream = { &dataInStreamCallback, &dataIn, maxSize};
        bool success = pb_decode_delimited_noinit(&stream, blox_Pid_Persisted_fields, &newData);
        /* if no errors occur, write new settings to wrapped object */
        if(success){
            copyPersistedFrom(newData);
            if(storeToEeprom){
                storeSettings(newData);
            }
        }
    }

    static cbox::Object* create(cbox::ObjectDefinition& defn) {
        auto obj = new PidBlock;
        if(obj != nullptr){
            obj->writeFromImpl(*defn.in, false);
        }
        return obj;
    }

    bool storeSettings(blox_Pid_Persisted & settings){
        if(eeprom_offset() == eptr_t(-1)){
            return false; /* EEPROM location is not set */
        }
        eptr_t offset = eeprom_offset();
        pb_ostream_t stream = { &eepromOutStreamCallback, &offset, eepromSize(), 0 };

        bool status = pb_encode_delimited(&stream, blox_Pid_Persisted_fields, &settings);

        return status;
    }

    Pid & get() {
        return pid;
    }

    virtual Interface * getApplicationInterfaceImpl() override final{
        return &pid;
    }

    virtual void readTo(cbox::DataOut& out) override final {
        blox_Pid message;

        assert_size<sizeof(message.links.output), MAX_ID_CHAIN_LENGHT>();
        assert_size<sizeof(message.links.input), MAX_ID_CHAIN_LENGHT>();

        message.settings = copy_struct_as<blox_Pid_Settings>(pid.getSettings());
        message.state = copy_struct_as<blox_Pid_State>(pid.getState());
        inputLookup.copyTo(message.links.input);
        outputLookup.copyTo(message.links.output);
        message.filtering.input = pid.getInputFiltering();
        message.filtering.derivative = pid.getDerivativeFiltering();

        static_assert(blox_Pid_size < 128, "varint for size will be larger than 1 byte");
        pb_ostream_t stream = { &dataOutStreamCallback, &out, blox_Pid_size + 1, 0 };
        pb_encode_delimited(&stream, blox_Pid_fields, &message);
    }

    virtual cbox::obj_type_t typeID() override {
    	// use function overloading and templates to manage type IDs in a central place (AppTypeRegistry)
    	return resolveTypeID(this);
    }
};
