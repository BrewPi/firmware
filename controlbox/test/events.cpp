#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"

#include "catch.hpp"
#include "fakeit.hpp"
#pragma GCC diagnostic pop
#include "Box.h"

using namespace fakeit;

SCENARIO("A created event is sent to listeners after an object is created")
{
	GIVEN("")
	{
		Mock<AllCallbacks> mock;
		AllCallbacksDelegate d(mock.get());

		EmptyDataIn dataIn;
		BlackholeDataOut dataOut;
		FixedContainer root(0, nullptr);

		When(Method(mock, getDataIn)).Return(dataIn);
		When(Method(mock, getDataOut)).Return(dataOut);

		WHEN("")
		{
			AllInOneBox box(root, d);
		}
		THEN("")
		{

		}
	}

}
