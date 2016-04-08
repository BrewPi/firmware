#include "Values.h"
#include "Ticks.h"
#include "ArrayEepromAccess.h"
#include "ValueTicks.h"
#include "ValueModels.h"
#include "PersistChangeValue.h"
#include "Commands.h"

bool quit = false;

/**
 * Function prototype expected by the commands implementation to perform
 * a reset.
 * @param exit false on first call, true on second call. The first call (exit==false) is
 * during command processing, so that actions can be taken before the command response is sent.
 * The second call (exit==true) is called to perform the actual reset.
 */
void handleReset(bool reset)
{
	quit = true;
}

void connectionStarted(DataOut& out)
{
	out.writeAnnotation("hello world");
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


int main()
{
	controlbox_setup(0);

	while (!quit)
	{
		controlbox_loop();
	}

	return 0;
}


Ticks baseticks;
EepromAccess eepromAccess;
