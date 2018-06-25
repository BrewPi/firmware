#include <string>
#include "Catch.hpp"

#include "SetPoint.h"
#include "defaultDevices.h"

SCENARIO("Basic test of a set point", "[setpoint]") {
    GIVEN("A fresh set point") {
        SetPointSimple sp;

        REQUIRE(sp.read() == temp_t::disabled());

        WHEN("temperature is written") {
            sp.write(temp_t(21.1));

            THEN("the value can be read") {
                REQUIRE(sp.read() == temp_t(21.1));
            }
        }
    }
}