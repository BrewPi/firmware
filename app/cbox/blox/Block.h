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

	virtual uint32_t update(const uint32_t & currentTime) override final {
	    Interface * appInterface = getApplicationInterface();
	    if(appInterface != nullptr){
	        appInterface->update();
	    }
	    return 0; // TODO have application classes return the time until next update
	}
};

// helpers functions to stream protobuf fields

cbox::StreamResult streamProtoTo(cbox::DataOut& out, const void* srcStruct, const pb_field_t fields[], size_t maxSize);
cbox::StreamResult streamProtoFrom(cbox::DataIn& in, void* destStruct, const pb_field_t fields[], size_t maxSize);
