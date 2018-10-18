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
#include "cbox/DataStream.h"
#include "cbox/DataStreamConverters.h"
#include "cbox/DataStreamIo.h"
#include <google/protobuf/message.h>

class ProtoDataOut {
public:
    cbox::HexCrcDataOut& out;
    ProtoDataOut(cbox::HexCrcDataOut& target)
        : out(target)
    {
    }

    void put(::google::protobuf::Message& message)
    {
        for (auto& c : message.SerializeAsString()) {
            out.write(c);
        }
        out.write(0); // zero terminate protobuf message
    }
};

inline void
decodeProtoFromReply(std::stringstream& ss, ::google::protobuf::Message& message)
{
    cbox::IStreamDataIn hex(ss);
    cbox::HexTextToBinaryIn decoder(hex);
    while (hex.next() != '|') { // spool command echo
    }
    // spool status, id, profiles and object type
    uint8_t header[6];
    decoder.read(header, 6);

    // pass the rest to the protobuf decoder
    std::stringstream ssProto;
    cbox::OStreamDataOut protoData(ssProto);
    decoder.push(protoData);
    message.ParseFromIstream(&ssProto);
};
