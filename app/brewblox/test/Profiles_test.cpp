/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of BrewBlox.
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

#include "BrewBloxTestBox.h"
#include <catch.hpp>
#include <cstdio>

#include "testHelpers.h"

SCENARIO("Active profiles can written through the profile object at ID 1")
{
    BrewBloxTestBox testBox;
    using commands = cbox::Box::CommandID;

    testBox.reset();

    // write profiles block
    testBox.put(commands::WRITE_OBJECT);
    testBox.put(cbox::obj_id_t(1));
    testBox.put(uint8_t(0xFF));
    testBox.put(cbox::ProfilesObject::staticTypeId());

    testBox.put(uint8_t(0x3));

    testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    // read profiles block
    testBox.put(commands::READ_OBJECT);
    testBox.put(cbox::obj_id_t(1));

    auto reply = testBox.processInput();
    CHECK(testBox.lastReplyHasStatusOk());

    std::stringstream expected;

    expected << cbox::addCrc("010100")
             << "|" << cbox::addCrc("00"   // no error
                                    "0100" // object id 2
                                    "FF"   // profiles 0xFF
                                    "FEFF" // object type
                                    "03")  // object data
             << "\n";
    CHECK(reply == expected.str());
}
