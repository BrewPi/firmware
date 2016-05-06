#include "catch.hpp"
#include <cstdio>
#include "examplebox.h"
#include "BoxApi.h"
#include "catch_output.h"

#include <regex>

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

void configure_ticks_example(ExampleBox& box)
{
    box.initialize();
    BoxApi api(box.get_box());
    REQUIRE(api.active_profile().is_valid()==false);
    Profile p = api.create_profile();
    REQUIRE(p.is_valid());
    api.activate_profile(p);
    REQUIRE(api.active_profile()==p);

    api.create_object(container_id(0), ExampleBox::as_int(ExampleBox::object_type::ValueTicksScaled));
}


SCENARIO("logging values command")
{
    GIVEN("a configured box")
    {
        ExampleBox box;
        configure_ticks_example(box);
        BoxApi api(box.get_box());

        WHEN("the profile is logged")
        {
            std::string result = api.run_command("0a 00");
            THEN("the log should list the created object")
            {
                INFO("result " << result);
                REQUIRE(std::regex_match(result, std::regex("00 06 ([[:xdigit:]]{2} ){4}01 00 ")));
            }
        }

        WHEN("object 0 is logged")
        {
            std::string result = api.run_command("0a 01 00");
            THEN("the log should list the created object")
            {
                INFO("result " << result);
                REQUIRE(std::regex_match(result, std::regex("00 06 ([[:xdigit:]]{2} ){4}01 00 ")));
            }
        }
    }
}
