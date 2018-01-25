#pragma once

#include "../EepromAwareWritableValue.h"
#include "../nanopb_callbacks.h"

class Bloc : public EepromAwareWritableValue {
	Interface * getApplicationInterface() override final{
		return getApplicationInterfaceImpl();
	}

	virtual Interface * getApplicationInterfaceImpl() = 0;
};
