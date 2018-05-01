/*
 * Copyright 2017 BrewPi
 *
 * This file is part of BrewPi.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "catch.hpp"
#include <cstdio>
#include "Integration.h"

SCENARIO("creating a profile is persisted"){
    GIVEN("clean static cbox app")
    {
        platform_init();
        eepromAccess.init();
        systemProfile.initializeEeprom();

        controlbox_setup(0);
        systemProfile.initialize();

        REQUIRE(systemProfile.currentProfile() == SYSTEM_PROFILE_DEFAULT);

        WHEN("a profile is created and activated")
        {
            profile_id_t p = systemProfile.createProfile();
            systemProfile.activateProfile(p);
            THEN("It gets ID 0")
            {
                REQUIRE(systemProfile.currentProfile() == 0);
            }
            AND_WHEN("The app is re-initialized"){
                controlbox_setup(false);
                THEN("the profile is selected by default")
                {
                    REQUIRE(systemProfile.currentProfile() == 0);
                }
            }
        }
    }
}
