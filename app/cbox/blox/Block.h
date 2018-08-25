#pragma once

#include "Object.h"
#include "ResolveType.h"
#include "pb.h"

// forward declarations
namespace cbox {
class DataOut;
class DataIn;
}

class Interface;

// Base BrewBlox block implementation
// This links the controlbox Object to the application object
class Block : public cbox::Object {
public:
    virtual Interface* getApplicationInterface() override final
    {
        return getApplicationInterfaceImpl();
    }

    virtual Interface* getApplicationInterfaceImpl() = 0;

    virtual cbox::update_t update(const cbox::update_t& now) override final
    {
        Interface* appInterface = getApplicationInterface();
        if (appInterface != nullptr) {
            return appInterface->update(now);
        }
        return now + 1000; // retry objects that cannot be updated every second
    }
};

// helpers functions to stream protobuf fields
cbox::CboxError
streamProtoTo(cbox::DataOut& out, const void* srcStruct, const pb_field_t fields[], size_t maxSize);
cbox::CboxError
streamProtoFrom(cbox::DataIn& in, void* destStruct, const pb_field_t fields[], size_t maxSize);
