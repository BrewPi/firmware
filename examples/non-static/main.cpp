#include "../../src/lib/CommsStdIO.h"
#include "Box.h"

#include "Ticks.h"
#include "ArrayEepromAccess.h"
#include "ValueTicks.h"
#include "ValueModels.h"
#include "Values.h"
#include "timems.h"


class SystemTicks : public Ticks
{
public:

	virtual ticks_millis_t millis()
	{
		return millisSinceStartup();
	}
};


class ExampleBox : public CommandCallbacks
{
	bool quit;
	ArrayEepromAccess<1024> eepromAccess;
	SystemTicks ticks;
	StdIOConnection connection;
	Box box;

public:


	ExampleBox(Object** objs, size_t count) : quit(false), box(connection, eepromAccess, ticks, *this, objs, count) {}

	void run()
	{
		box.setup();
		while (!should_quit())
		{
			box.loop();
		}
	}

	/**
	 * Application-provided function that creates an object from the object definition.
	 */
	virtual Object* createApplicationObject(ObjectDefinition& def, bool dryRun=false)
	{
		uint8_t type = def.type;
		if (dryRun)		// consume the stream without creating an object
			type = 0;

		switch (type)
		{
		case 1:
			return new ScaledTicksValue(ticks);

		default:
			return nullFactory(def);
		}
	}

	/**
	 * Function prototype expected by the commands implementation to perform
	 * a reset.
	 * @param exit false on first call, true on second call. The first call (exit==false) is
	 * during command processing, so that actions can be taken before the command response is sent.
	 * The second call (exit==true) is called to perform the actual reset.
	 */
	virtual void handleReset(bool exit=true)
	{
		quit = true;
	}

	bool should_quit() { return quit; }

	virtual void connectionStarted(DataOut& out)
	{
		out.writeAnnotation("Example app at your service.");
	}

	virtual Container* createRootContainer()
	{
		DynamicContainer* d = new DynamicContainer();
		return d;
	}

};

int main(int argc, char** argv)
{
	ExampleBox box(nullptr, 0);
	box.run();
}
