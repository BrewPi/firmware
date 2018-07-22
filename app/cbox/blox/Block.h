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
