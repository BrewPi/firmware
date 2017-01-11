

#include "timems.h"

class Ticks
{
public:
	ticks_millis_t millis()
	{
		return ticks_millis_t(millisSinceStartup());
	}
};

class Delay
{
public:
	void wait(ticks_millis_t delay)
	{
	}
};

typedef Ticks TicksImpl;


