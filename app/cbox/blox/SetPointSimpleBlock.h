#pragma once

#include "Block.h"
#include "SetPointSimple.pb.h"
#include "SetPoint.h"

class SetPointSimpleBlock: public Block {
private:
    SetPointSimple setpoint;

public:
    SetPointSimpleBlock() :
        setpoint()
{}

    virtual cbox::stream_size_t streamFromMaxSize() override final {
        return blox_SetPointSimple_Persisted_size;
    }

    virtual cbox::stream_size_t streamToMaxSize() override final {
        return blox_SetPointSimple_size;
    }

    virtual cbox::Object::StreamFromResult streamFrom(cbox::DataIn& dataIn) override final {
        blox_SetPointSimple_Persisted newData;
        size_t maxSize = streamFromMaxSize();
        pb_istream_t stream = { &dataInStreamCallback, &dataIn, maxSize};
        bool success = pb_decode(&stream, blox_SetPointSimple_Persisted_fields, &newData);
        /* if no errors occur, write new settings to wrapped object */
        if(!success){
            return cbox::Object::StreamFromResult::stream_error;
        }
        setpoint.setSettings(copy_struct_as<SetPointSimple::Settings>(newData.settings));
        return cbox::Object::StreamFromResult::success_persist;
    }

    virtual cbox::Object::StreamToResult streamTo(cbox::DataOut& out) override final {
        blox_SetPointSimple message;
        message.settings = copy_struct_as<blox_SetPointSimple_Settings>(setpoint.getSettings());

        pb_ostream_t stream = { &dataOutStreamCallback, &out, streamToMaxSize(), 0 };
        bool success = pb_encode(&stream, blox_SetPointSimple_fields, &message);
        return (success) ? cbox::Object::StreamToResult::success : cbox::Object::StreamToResult::stream_error;
    }

    SetPointSimple & get() {
        return setpoint;
    }

    virtual Interface * getApplicationInterfaceImpl() override final{
        return &setpoint;
    }


    virtual cbox::obj_type_t typeID() override {
    	// use function overloading and templates to manage type IDs in a central place (ResolveType.cpp)
    	return resolveTypeID(this);
    }

};
