#pragma once

#include "Values.h"
#include "FilterCascaded.h"


// Todo wonder if the different values should be made available as a container.
// Container is best if the values need to be logged, but may not be needed if the ValueSource* is itself
// a logged value.
class FilterValue : public Value, ValueSource
{
	ValueSource* source;
	CascadedFilter filter;
	
public:	

	/**
	 * Value 0 - 2 bytes, current output
	 * Value 1 - 4 bytes, current output (high precision)
	 */
	bool getValue(void* value, uint8_t id) {
		uint8_t result = true;
		if (id==0)
			*((fixed7_9*)value) = filter.readOutput();
		else if (id==1)
			*((fixed23_9*)value) = filter.readOutputDoublePrecision();
		return result;
	}
	
	void readTo(DataOut& out) {
		fixed23_9 value = filter.readOutputDoublePrecision();
		writeBytes(&value, 4, out);
		
	}
	
	void writeMaskedFrom(DataIn& in, DataIn& mask) {
		
	}

	// read/write externally provides both the filter value and filter settings.
	// masking is needed to avoid altering the filter value when changing settings.	
	static Object* Create(ObjectDef	inition& def)
	{		
		// 1-byte co-efficients
		// id_chain of ValueSource to fetch input value from
		// the filter could be instantiated directly by the controller as part of the container
		// provided by the controller so that no need for a separate create command in eeprom.
		// And the controller knows which object to use as input. This would save a lot of eeprom space.
		return new_object(FilterValue());
	}
};