#pragma once

#include "../../../controlbox/src/lib/Object.h"
#include "Interface.h"
#include "../nanopb_callbacks.h"
#include "assert_size_helper.h"
#include "struct_copy_helper.h"

// Base BrewBlox block implementation
// An object that can be read to the stream, but not written
class Block : public cbox::WritableObject {
public:
	virtual Interface * getApplicationInterface() override final {
		return getApplicationInterfaceImpl();
	}

	virtual Interface * getApplicationInterfaceImpl() = 0;

	virtual uint32_t update() override final {
	    Interface * appInterface = getApplicationInterface();
	    if(appInterface != nullptr){
	        appInterface->update();
	    }
	    return 0; // TODO have application classes return the time until next update
	}
};

// helpers functions to stream protobuf fields

cbox::Object::StreamToResult streamProtoTo(cbox::DataOut& out, const void* srcStruct, const pb_field_t fields[], size_t maxSize);
cbox::Object::StreamFromResult streamProtoFrom(cbox::DataIn& in, void* destStruct, const pb_field_t fields[], size_t maxSize);
