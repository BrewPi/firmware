
#include "EepromTypes.h"
#include "EepromAccessImpl.h"

#include "Values.h"
#include "Ticks.h"
#include "ValueTicks.h"
#include "ValueModels.h"
#include "PersistChangeValue.h"
#include "Commands.h"

#include "Platform.h"


void connectionStarted(DataOut& out)
{
	out.writeAnnotation("brewpi controlbox");
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
	CurrentTicksValue::create,								// type 1
	DynamicContainer::create,								// type 2
	EepromValue::create,									// type 3
	PersistChangeValue::create,								// type 4
	IndirectValue::create,									// type 5
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
Object* createApplicationObject(ObjectDefinition& def, bool dryRun)
{
	uint8_t type = def.type;
	if (dryRun || type>=sizeof(createObjectHandlers)/sizeof(createObjectHandlers[0]))
		type = 0;		// null object creator. Ensures stream is properly consumed even for invalid type values.

	Object* result = createObjectHandlers[type](def);
	return result;
}


void setup()
{
	platform_init();
	controlbox_setup(0);
}

void loop()
{
	controlbox_loop();
}


TicksImpl baseticks;
EepromAccess eepromAccess;
