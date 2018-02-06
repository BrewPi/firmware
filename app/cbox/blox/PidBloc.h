#pragma once

#include "bloc.h"
#include "Pid.pb.h"
#include "Pid.h"

#include "ProcessValueDelegate.h"
#include "CboxLink.h"

// use union to copy between blox struct and app struct.
// a memcpy is not guaranteed to do a valid deep copy
union PidSettings_union {
    PidSettings_union() : app(){}
    ~PidSettings_union() = default;
    Pid::Settings app;
    blox_Pid_Settings blox;
};
union PidState_union {
    PidState_union() : app(){}
    ~PidState_union() = default;
    Pid::State app;
    blox_Pid_State blox;
};


class PidBloc: public Bloc {
private:
    CboxLookup inputLookup;
    CboxLookup outputLookup;

    ProcessValueDelegate input;
    ProcessValueDelegate output;

    Pid pid;

public:
    PidBloc() :
        input(inputLookup),
        output(outputLookup),
        pid(input, output)
    {}

    static const size_t persistedMaxSize(){
        static_assert(blox_Pid_Persisted_size < 128, "varint for settings size will be larger than 1 byte");
        return blox_Pid_Persisted_size + 1;
    }
    virtual uint8_t readStreamSize() override final {
        /* maximum size of settings  +1 for varint for length in delimited message */
        return persistedMaxSize();
    }

    virtual void writeFrom(DataIn& dataIn) override final {
        writeFromImpl(dataIn, true);
    }

    void copyPersistedTo(blox_Pid_Persisted & to){
        PidSettings_union settings_union;
        pid.copySettingsTo(settings_union.app);
        to.settings = settings_union.blox;

        inputLookup.copyTo(to.links.input);
        outputLookup.copyTo(to.links.output);
        to.filtering.input = pid.getInputFiltering();
        to.filtering.derivative = pid.getDerivativeFiltering();
    }

    void copyPersistedFrom(blox_Pid_Persisted & from){
        PidSettings_union settings_union;
        settings_union.blox = from.settings;
        pid.copySettingsFrom(settings_union.app);
        inputLookup.copyFrom(from.links.input);
        outputLookup.copyFrom(from.links.output);
        pid.setInputFiltering(from.filtering.input);
        pid.setDerivativeFiltering(from.filtering.derivative);
    }

    void writeFromImpl(DataIn& dataIn, bool storeToEeprom){
        /* copy old settings, because the update can be sparse and can only overwrite some of the values */
        blox_Pid_Persisted newData;
        /* copy old settings in case of a sparse update */
        copyPersistedTo(newData);

        /* stream in new settings, overwriting copy of old settings */
        size_t maxSize = persistedMaxSize();
        pb_istream_t stream = { &dataInStreamCallback, &dataIn, maxSize, 0 };
        bool success = pb_decode_delimited_noinit(&stream, blox_Pid_Persisted_fields, &newData);
        /* if no errors occur, write new settings to wrapped object */
        if(success){
            copyPersistedFrom(newData);

            if(storeToEeprom){
                storeSettings(newData);
            }
        }
    }

    static Object* create(ObjectDefinition& defn) {
        auto obj = new PidBloc;
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

    virtual void readTo(DataOut& out) override final {
        blox_Pid message;

        assert_size<sizeof(message.settings), sizeof(Pid::Settings)>();
        assert_size<sizeof(message.state), sizeof(Pid::State)>();
        assert_size<sizeof(message.links.input), MAX_ID_CHAIN_LENGHT>();
        assert_size<sizeof(message.links.output), MAX_ID_CHAIN_LENGHT>();
        PidSettings_union settings_union;
        pid.copySettingsTo(settings_union.app);
        message.settings = settings_union.blox;

        PidState_union state_union;
        pid.copyStateTo(state_union.app);
        message.state = state_union.blox;
        inputLookup.copyTo(message.links.input);
        outputLookup.copyTo(message.links.output);
        message.filtering.input = pid.getInputFiltering();
        message.filtering.derivative = pid.getDerivativeFiltering();

        static_assert(blox_Pid_size < 128, "varint for size will be larger than 1 byte");
        pb_ostream_t stream = { &dataOutStreamCallback, &out, blox_Pid_size + 1, 0 };
        pb_encode_delimited(&stream, blox_Pid_fields, &message);
    }
};
