#pragma once

#include "Block.h"
#include "Pid.pb.h"
#include "Pid.h"

#include "ProcessValueDelegate.h"
#include "CboxLink.h"

#if 0

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


    virtual cbox::stream_size_t streamFromMaxSize() override final {
        return blox_Pid_Persisted_size;
    }

    virtual cbox::stream_size_t streamToMaxSize() override final {
        return blox_Pid_size;
    }

    void copyPersistedTo(blox_Pid_Persisted & to){
        to.settings = copy_struct_as<blox_Pid_Settings>(pid.getSettings());
        to.links.input = inputLookup;
        to.links.output = outputLookup;
        to.filtering.input = pid.getInputFiltering();
        to.filtering.derivative = pid.getDerivativeFiltering();
    }

    void copyPersistedFrom(blox_Pid_Persisted & from){
        pid.setSettings(copy_struct_as<Pid::Settings>(from.settings));
        inputLookup = from.links.input;
        outputLookup = from.links.output;
        pid.setInputFiltering(from.filtering.input);
        pid.setDerivativeFiltering(from.filtering.derivative);
    }

    virtual cbox::Object::StreamFromResult streamFrom(cbox::DataIn& dataIn) override final {
        blox_Pid_Persisted newData;
        size_t maxSize = streamFromMaxSize();
        pb_istream_t stream = { &dataInStreamCallback, &dataIn, maxSize};
        bool success = pb_decode(&stream, blox_Pid_Persisted_fields, &newData);
        if(!success){
            return cbox::Object::StreamFromResult::stream_error;
        }
        copyPersistedFrom(newData);
        return cbox::Object::StreamFromResult::success_persist;
    }

    virtual cbox::Object::StreamToResult streamTo(cbox::DataOut& out) override final {
        blox_Pid message;

        message.settings = copy_struct_as<blox_Pid_Settings>(pid.getSettings());
        message.state = copy_struct_as<blox_Pid_State>(pid.getState());
        message.links.input = inputLookup;
        message.links.output = outputLookup;
        message.filtering.input = pid.getInputFiltering();
        message.filtering.derivative = pid.getDerivativeFiltering();

        pb_ostream_t stream = { &dataOutStreamCallback, &out, blox_Pid_size, 0 };
        bool success = pb_encode(&stream, blox_Pid_fields, &message);
        return (success) ? cbox::Object::StreamToResult::success : cbox::Object::StreamToResult::stream_error;
    }

    virtual cbox::Object::StreamToResult streamPersistedTo(cbox::DataOut& out) override final {
        blox_Pid_Persisted message;

        message.settings = copy_struct_as<blox_Pid_Settings>(pid.getSettings());
        message.links.input = inputLookup;
        message.links.output = outputLookup;
        message.filtering.input = pid.getInputFiltering();
        message.filtering.derivative = pid.getDerivativeFiltering();

        pb_ostream_t stream = { &dataOutStreamCallback, &out, persistedMaxSize(), 0 };
        bool success = pb_encode(&stream, blox_Pid_fields, &message);
        return (success) ? cbox::Object::StreamToResult::success : cbox::Object::StreamToResult::stream_error;
    }

    Pid & get() {
        return pid;
    }

    virtual Interface * getApplicationInterfaceImpl() override final{
        return &pid;
    }

    virtual cbox::obj_type_t typeID() override {
    	// use function overloading and templates to manage type IDs in a central place (AppTypeRegistry)
    	return resolveTypeID(this);
    }
};

#endif
