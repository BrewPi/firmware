
//#include "OneWireBusCBox.h"
//#include "OneWireTempSensorCBox.h"
//#include "EepromTypes.h"
#include "EepromAccessImpl.h"

#include "Values.h"
#include "Ticks.h"
#include "ValueTicks.h"
#include "ValueModels.h"
#include "PersistChangeValue.h"
#include "Commands.h"
#include "Platform.h"

DelayImpl wait = DelayImpl(DELAY_IMPL_CONFIG);
ScaledTicksValue ticks;

void connectionStarted(StandardConnection& connection, DataOut& out)
{
}

// OneWireBusCBox oneWireBus;

Container& systemRootContainer()
{
	static FixedContainer root; // empty root container
	return root;
}

/**
 * Provide the root container for the current profile. This should be dynamically allocated
 * (since it will be disposed by calling delete.)
 *
 */
Container* createRootContainer()
{
	DynamicContainer* d = new DynamicContainer();
	return d;
}


Commands::ObjectFactory createObjectHandlers[] = {
	nullFactory,                                            // type 0
	ScaledTicksValue::create,								// type 1
	DynamicContainer::create,								// type 2
	EepromValue::create,									// type 3
	PersistChangeValue::create,								// type 4
	IndirectValue::create,									// type 5
//	OneWireTempSensorCBox::create,					        // type 6
	NULL

	// When defining a new object type, add the handler above the last NULL value (it's just there to make
	// editing the code easier).
	// The Object definition passed to the create handler contains the stream and the block length.
};

/**
 * The application supplied object factory.
 * Fetches the object type from the stream and looks this up against an array of object factories.
 * It's critical that the create code reads len bytes from the stream so that the data is
 * Spooled to eeprom to the persisted object definition.
 */
int8_t createApplicationObject(Object*& result, ObjectDefinition& def, bool dryRun)
{
    uint8_t type = def.type;
    int8_t error = errorCode(no_error);
    if (type>=sizeof(createObjectHandlers)/sizeof(createObjectHandlers[0])) {
        error = errorCode(invalid_type);
    }
    else {
        if (dryRun){
            type = 0; // null object creator. Ensures stream is properly consumed even for invalid type values.
        }
        result = createObjectHandlers[type](def);
        if (!result) {
            error = errorCode(insufficient_heap);
        }
    }
    return error;
}

TicksImpl baseticks;

EepromAccess eepromAccess;
