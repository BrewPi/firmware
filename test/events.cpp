#include "catch.hpp"
#include "fakeit.hpp"
#include "Box.h"

using namespace fakeit;

SCENARIO("A created event is sent to listeners after an object is created")
{
	GIVEN("")
	{
		Mock<AllCallbacks> mock;
		AllInOneBox box(mock.get());
	}
	WHEN("")
	{

	}
	THEN("")
	{

	}

}
