#pragma once

#include "../EepromAwareWritableValue.h"
#include "../nanopb_callbacks.h"
#include "assert_size_helper.h"
#include "struct_copy_helper.h"

class Bloc : virtual public cbox::EepromAwareWritableValue {
public:
	virtual Interface * getApplicationInterface() override final{
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
