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
#include "MockTicks.h"
#include "blox/ActuatorPinBlock.h"
#include "blox/TicksBlock.h"
#include "cbox/Box.h"
#include "cbox/CboxPtr.h"
#include "cbox/DataStream.h"
#include "cbox/DataStreamIo.h"
#include "testHelpers.h"
#include <sstream>
#include <type_traits>

class BrewBloxTestBox {
public:
    std::shared_ptr<std::stringstream> in;
    std::shared_ptr<std::stringstream> out;
    cbox::OStreamDataOut inOs;
    cbox::BinaryToHexTextOut toHex;
    cbox::HexCrcDataOut inEncoder;
    ProtoDataOut inProto;
    bool lastReplyOk = false;
    MockTicks& ticks;

    BrewBloxTestBox()
        : in(std::make_shared<std::stringstream>())
        , out(std::make_shared<std::stringstream>())
        , inOs(*in)
        , toHex(inOs)
        , inEncoder(toHex)
        , inProto(inEncoder)
        , ticks(brewbloxBox().makeCboxPtr<TicksBlock<MockTicks>>(3).lock()->get())
    {
        testConnectionSource().add(in, out);
    }
    ~BrewBloxTestBox(){};

    void clearStreams()
    {
        in->str("");
        in->clear();
        out->str("");
        out->clear();
    }

    void reset()
    {
        // reset system objects: pin actuators
        for (cbox::obj_id_t id = 10; id <= 14; ++id) {
            auto cbPtr = brewbloxBox().makeCboxPtr<ActuatorPinBlock>(id);
            if (auto pinObj = cbPtr.lock()) {
                pinObj->getConstrained().removeAllConstraints();
                pinObj->getConstrained().resetHistory();
                pinObj->getConstrained().state(ActuatorDigital::State::Inactive);
            }
        }
        brewbloxBox().update(0);
        clearStreams();
        inEncoder.put(cbox::Box::CommandID::CLEAR_OBJECTS);
        inEncoder.endMessage();
        brewbloxBox().hexCommunicate();
        clearStreams();
    }

    bool lastReplyHasStatusOk()
    {
        return lastReplyOk;
    }

    std::string processInput()
    {
        endInput();
        brewbloxBox().hexCommunicate();
        lastReplyOk = out->str().find("|00") != std::string::npos; // no errors
        auto retv = out->str();
        clearStreams();
        return retv;
    }

    void processInputToProto(::google::protobuf::Message& message)
    {
        endInput();
        brewbloxBox().hexCommunicate();
        lastReplyOk = out->str().find("|00") != std::string::npos; // no errors
        decodeProtoFromReply(*out, message);
        clearStreams();
    }

    template <typename T>
    void put(const T& t, typename std::enable_if_t<!std::is_base_of<::google::protobuf::Message, T>::value>* = 0)
    {
        inEncoder.put(t);
    }

    void put(const ::google::protobuf::Message& message)
    {
        inProto.put(message);
    }

    void endInput()
    {
        inEncoder.endMessage();
    }

    void update(const cbox::update_t& now)
    {
        ticks.reset(now);

        brewbloxBox().update(now);
    }
};
