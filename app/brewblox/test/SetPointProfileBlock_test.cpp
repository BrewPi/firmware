/*
 * Copyright 2018 BrewPi
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

#include "BrewBloxTestBox.h"
#include "MockTicks.h"
#include "blox/SetpointProfileBlock.h"
#include "blox/TicksBlock.h"
#include "cbox/DataStreamIo.h"
#include "proto/test/cpp/SetpointProfile.test.pb.h"
#include "proto/test/cpp/Ticks.test.pb.h"
#include <catch.hpp>
#include <sstream>

using namespace cbox;

SCENARIO("A SetpointProfile block")
{
    WHEN("a SetpointProfileBlock is created")
    {
        BrewBloxTestBox testBox;
        using commands = cbox::Box::CommandID;

        testBox.reset();

        // create mock sensor
        testBox.put(commands::CREATE_OBJECT);
        testBox.put(cbox::obj_id_t(100));
        testBox.put(uint8_t(0xFF));
        testBox.put(SetpointProfileBlock::staticTypeId());

        auto message = blox::SetpointProfile();
        {
            auto newPoint = message.add_points();
            newPoint->set_time(20'010);
            newPoint->set_temperature(cnl::unwrap(temp_t(20)));
        }

        {
            auto newPoint = message.add_points();
            newPoint->set_time(20'020);
            newPoint->set_temperature(cnl::unwrap(temp_t(21)));
        }

        testBox.put(message);

        testBox.processInput();
        CHECK(testBox.lastReplyHasStatusOk());

        testBox.update(10'000);

        auto lookup = brewbloxBox().makeCboxPtr<SetpointProfileBlock>(100);
        auto spPtr = lookup.lock();
        REQUIRE(spPtr);

        WHEN("The box has not received the current time (in seconds since epoch")
        {
            THEN("The profile setpoint is invalid")
            {
                CHECK(spPtr->get().setting() == 0);
                CHECK(spPtr->get().valid() == false);
            }
        }

        WHEN("The box has received the current time (in seconds since epoch")
        {
            // create mock sensor
            testBox.put(commands::WRITE_OBJECT);
            testBox.put(cbox::obj_id_t(3)); // ticks block is at 3
            testBox.put(uint8_t(0xFF));
            testBox.put(TicksBlock<MockTicks>::staticTypeId());

            auto message = blox::Ticks();
            message.set_secondssinceepoch(20'000);
            testBox.put(message);

            auto reply = blox::Ticks();

            testBox.processInputToProto(reply);

            THEN("The system time is updated correctly")
            {
                CHECK(testBox.lastReplyHasStatusOk());

                CHECK(reply.millissinceboot() == 10'000);
                CHECK(reply.secondssinceepoch() == 20'000);
            }

            testBox.update(25000); // system is running for 25 seconds, so seconds since epoch should be 20.015 now

            THEN("The profile setpoint is valid")
            {
                CHECK(spPtr->get().valid() == true);
            }
            AND_THEN("The setting is correctly interpolated")
            {
                CHECK(spPtr->get().setting() == temp_t(20.5)); // halfway between points
            }
            AND_WHEN("The SetpointProfile block streams out protobuf settings, the data is as expected")
            {
                testBox.put(commands::READ_OBJECT);
                testBox.put(cbox::obj_id_t(100));

                auto decoded = blox::SetpointProfile();
                testBox.processInputToProto(decoded);
                CHECK(testBox.lastReplyHasStatusOk());

                CHECK(decoded.ShortDebugString() == "points { time: 20010 temperature: 81920 } points { time: 20020 temperature: 86016 }");
            }
        }
    }
}