#pragma once

#include "Object.h"
#include "Interface.h"
#include "../nanopb_callbacks.h"
#include "ResolveType.h"

// Base BrewBlox block implementation
// This links the controlbox Object to the application object
class Block : public cbox::Object {
public:
	virtual Interface * getApplicationInterface() override final {
		return getApplicationInterfaceImpl();
	}

	virtual Interface * getApplicationInterfaceImpl() = 0;

	virtual update_t update(const update_t & t) override final {
	    Interface * appInterface = getApplicationInterface();
	    if(appInterface != nullptr){
	        return appInterface->update(t);
	    }
	    return t + 1000; // retry objects that cannot be updated every second
	}
};

// helpers functions to stream protobuf fields

cbox::CboxError streamProtoTo(cbox::DataOut& out, const void* srcStruct, const pb_field_t fields[], size_t maxSize);
cbox::CboxError streamProtoFrom(cbox::DataIn& in, void* destStruct, const pb_field_t fields[], size_t maxSize);
