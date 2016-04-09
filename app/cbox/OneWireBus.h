#pragma once

/**
 * Normally objects go in their own header file if they are created as part of this project.
 * This file is for brewpi objects that are based on external code, so we don't want to modify the base object definition.
 */

#include "Values.h"

#include "OneWire.h"

/**
 * An object that provides an interface to a onewire bus.
 */
// mdma: I didn't push the Object functionality down into OneWire because that code comes from an external source - less intrusive to subclass.
class OneWireBus : public Value
{
	OneWire _onewire;
	
public:	
	OneWireBus(uint8_t pin) : _onewire(pin) { }
	
	OneWire* onewire() { return &_onewire; }
	
	static Object* create(ObjectDefinition& def) {		
		uint8_t pin = def.in->next();
		return new_object(OneWireBus(pin));
	}
	
	/**
	 * Enumerates the one-wire bus and returns the devices found.
	 */
	void readTo(DataOut& out) {
		_onewire.reset_search();
		uint8_t address[8];
		while (_onewire.search(address)) {
			// hardware device type from OneWire family ID
			out.writeBuffer(address, 8);
		}		
	}
	
	uint8_t streamSize() {
		return 0;
	}
	
};

