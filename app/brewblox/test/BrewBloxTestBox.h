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

#pragma once

#include "BrewBlox.h"
#include "cbox/Box.h"
#include "cbox/DataStream.h"
#include "cbox/DataStreamIo.h"
#include "testHelpers.h"
#include <sstream>

class BrewBloxTestBox {
public:
    cbox::Box& box;
    std::stringstream in;
    std::stringstream out;
    std::stringstream expected;
    cbox::OStreamDataOut inOs;
    cbox::BinaryToHexTextOut toHex;
    cbox::HexCrcDataOut inEncoder;
    ProtoDataOut inProto;

    BrewBloxTestBox()
        : box(brewbloxBox())
        , inOs(in)
        , toHex(inOs)
        , inEncoder(toHex)
        , inProto(inEncoder)
    {
        testConnectionSource().add(in, out);
    }
    ~BrewBloxTestBox() = default;

    void clearStreams()
    {
        in.str("");
        in.clear();
        out.str("");
        out.clear();
        expected.str("");
        expected.clear();
    }

    void reset()
    {
        box.update(0);
        clearStreams();
        inEncoder.put(cbox::Box::CommandID::CLEAR_OBJECTS);
        inEncoder.endMessage();
        box.hexCommunicate();
    }

    bool lastReplyHasStatusOk()
    {
        return out.str().find("|00") != std::string::npos; // no errors
    }

    template <typename T>
    void put(const T& t)
    {
        inEncoder.put(t);
    }

    void put(::google::protobuf::Message& message)
    {
        inProto.put(message);
    }

    void endMessage()
    {
        inEncoder.endMessage();
    }
};
