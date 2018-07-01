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

SCENARIO("foo", "[setpoint]") {
    SetPointMinMax sp;

    sp.setMin(10.0);
    sp.setMax(30.0);
    
    WHEN("value is written within the range of max and min") {
        sp.write(15.0);

        THEN("the set value is read") {
            REQUIRE(sp.read() == temp_t(15.0));
        }
    }

    WHEN("value is set above the range of max and min") {
        sp.write(40.0);

        THEN("the max value is read") {
            REQUIRE(sp.read() == temp_t(30.0));
        }
    }
    
    WHEN("value is set below the range of max and min") {
        sp.write(5.0);

        THEN("the min value is read") {
            REQUIRE(sp.read() == temp_t(10.0));
        }
    }
}

SCENARIO("Default set point is immutable", "[setpoint]") {
    SetPoint * sp = defaultSetPoint();

    REQUIRE(sp->read() == temp_t::invalid());

    WHEN("temperature is written") {
        sp->write(temp_t(21.1));

        THEN("the value does not change") {
            REQUIRE(sp->read() == temp_t::invalid());
        }
    }

    delete sp;
}