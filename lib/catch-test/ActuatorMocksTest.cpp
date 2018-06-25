#include <string>
#include <sstream>

#include "Catch.hpp"
#include "TestMatchers.hpp"

#include "ActuatorInterfaces.h"
#include "ActuatorMocks.h"

SCENARIO("Basic test of the digital actuator", "[actuator]") {
    GIVEN( "A fresh acutuator boolean" ) {
		ActuatorBool act;

        REQUIRE(act.getState() == ActuatorDigital::State::Inactive); // defaults to inactive

        WHEN("set to active") {
			act.setState(ActuatorDigital::State::Active);
            THEN( "the state changes to active" ) {
                REQUIRE(act.getState() == ActuatorDigital::State::Active);
            }
        }

        WHEN("set to inactive") {
			act.setState(ActuatorDigital::State::Inactive);
            THEN( "the state changes to inactive" ) {
                REQUIRE(act.getState() == ActuatorDigital::State::Inactive);
            }
        }
    }
}