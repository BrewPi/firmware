#pragma once

#include "Values.h"
#include "Interface.h"
#include "../nanopb_callbacks.h"
#include "assert_size_helper.h"
#include "struct_copy_helper.h"

// Base BrewBlox block implementation
// An object that can be read to the stream, but not written
class Block : public cbox::EepromAwareWritableValue {
public:
	virtual Interface * getApplicationInterface() override final {
		return getApplicationInterfaceImpl();
	}

	virtual Interface * getApplicationInterfaceImpl() = 0;

	virtual void update() override final {
	    Interface * appInterface = getApplicationInterface();
	    if(appInterface != nullptr){
	        appInterface->update();
	    }
	}
};
