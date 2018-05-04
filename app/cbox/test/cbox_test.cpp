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
#include "Platform.h"

SCENARIO("creating a profile is persisted"){
    GIVEN("clean static cbox app")
    {
        platform_init();
        eepromAccess.init();
        cbox::systemProfile.initializeEeprom();

        cbox::controlbox_setup(0);
        cbox::systemProfile.initialize();

        REQUIRE(cbox::systemProfile.currentProfile() == cbox::SYSTEM_PROFILE_DEFAULT);

        WHEN("a profile is created and activated")
        {
        	cbox::profile_id_t p = cbox::systemProfile.createProfile();
        	cbox::systemProfile.activateProfile(p);
            THEN("It gets ID 0")
            {
                REQUIRE(cbox::systemProfile.currentProfile() == 0);
            }
            AND_WHEN("The app is re-initialized"){
            	cbox::controlbox_setup(false);
                THEN("the profile is selected by default")
                {
                    REQUIRE(cbox::systemProfile.currentProfile() == 0);
                }
            }
        }
    }
}
