#pragma once

#include "../EepromAwareWritableValue.h"
#include "../nanopb_callbacks.h"
#include "assert_size_helper.h"

class Bloc : virtual public EepromAwareWritableValue {
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
