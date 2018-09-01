/*
 * Copyright 2018 BrewBlox / Elco Jacobs
 *
 * This file is part of Controlbox.
 *
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include <memory>
#include <queue>

#include "Connections.h"
#include "DataStreamIo.h"
#include <sstream>

namespace cbox {

/**
 * A connection source that emulates a connection by two string streams, used for testing
 * 
 **/

class StringStreamConnection : public Connection {
private:
    std::stringstream& in;
    std::stringstream& out;
    IStreamDataIn dataIn;
    OStreamDataOut dataOut;

public:
    StringStreamConnection(std::stringstream& _in, std::stringstream& _out)
        : in(_in)
        , out(_out)
        , dataIn(_in)
        , dataOut(_out)
    {
    }

    virtual DataOut& getDataOut() override final
    {
        return dataOut;
    }

    virtual DataIn& getDataIn() override final
    {
        return dataIn;
    }

    virtual bool isConnected() override final
    {
        return !(in.bad() || out.bad()); // use badbit of either stream to simulate disconnect
    }
};

class StringStreamConnectionSource : public ConnectionSource {
private:
    std::queue<std::unique_ptr<StringStreamConnection>> connectionQueue;

public:
    void add(std::stringstream& in, std::stringstream& out)
    {
        auto newConnection = std::make_unique<StringStreamConnection>(in, out);
        connectionQueue.push(std::move(newConnection));
    }

    virtual std::unique_ptr<Connection> newConnection() override final
    {
        if (connectionQueue.empty()) {
            return nullptr;
        }
        std::unique_ptr<Connection> retval = std::move(connectionQueue.front());
        connectionQueue.pop();
        return std::move(retval);
    }
};
} // end namespace cbox
