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
	void millis(ticks_millis_t delay)
	{
	}
};

#define DELAY_IMPL_CONFIG // empty define, no argument needed

typedef Ticks TicksImpl;
typedef Delay DelayImpl;

extern Ticks baseticks;
extern Delay wait;


