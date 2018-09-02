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
#include <iomanip>
#include <iostream>

#include "../BrewBlox.h"
#include "blox/SetpointSensorPairBlock.h"
#include "cbox/Box.h"
#include "cbox/DataStream.h"
#include "cbox/DataStreamIo.h"
#include "cbox/Object.h"

template <typename... Args>
void
sendToBox(cbox::Box::CommandID c, Args... args)
{
    std::stringstream ss;
    cbox::OStreamDataOut helper(ss);
    helper.put(uint8_t(c));
    for (const auto& p : {args...}) {
        helper.put(p);
    }
    std::cout << ss.str();
}

SCENARIO("A Blox SetpointSensorPair object can be created from streamed protobuf data")
{
    static auto& box = brewbloxBox();
    auto resetBox = []() {
        std::stringstream ssIn;
    };
}
