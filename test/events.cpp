#include "catch.hpp"
#include "fakeit.hpp"
#include "Box.h"

using namespace fakeit;

SCENARIO("A created event is sent to listeners after an object is created")
{
	GIVEN("")
	{
		Mock<AllCallbacks> mock;
		AllCallbacksDelegate d(mock.get());
		AllInOneBox box(d);
	}
	WHEN("")
	{

	}
	THEN("")
	{

	}

}
