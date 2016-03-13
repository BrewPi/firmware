#include "catch.hpp"
#include <cstdio>
#include "examplebox.h"
#include "BoxApi.h"
#include "catch_output.h"

SCENARIO("creating a profile is persisted")
{
    std::string filename = "eeprom_test.bin";

    GIVEN("clean examplebox")
    {
        remove(filename.c_str());
        ExampleBox box(filename);
        box.initialize();
        BoxApi api(box.get_box());

        REQUIRE(api.active_profile().is_valid()==false);

        WHEN("a profile is created and activated")
        {
            Profile p = api.create_profile();
            REQUIRE(p.is_valid());
            api.activate_profile(p);
            REQUIRE(api.active_profile()==p);

            AND_WHEN("the box is shutdown and restarted")
            {
                box.shutdown();

                ExampleBox box(filename);
                box.initialize();
                BoxApi api(box.get_box());

                THEN("the profile is selected by default")
                {
                    REQUIRE(api.active_profile()==p);
                }
            }
        }
    }

}
