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
#include <queue>
#include "Connections.h"
#include <sstream>



namespace cbox {

class IStreamDataIn : public DataIn
{
    std::istream& in;
//    uint8_t prev;
public:

    IStreamDataIn(std::istream& in_) : in(in_) {}

    virtual bool hasNext() override {
        return in.peek()!=EOF;
    }

    virtual uint8_t next() override {
        char val;
        in.get(val);
        return uint8_t(val);
    }

    virtual uint8_t peek() override {
        return uint8_t(in.peek());
    }

    virtual stream_size_t available() override {
        return in.eof() ? 0 : 1;
    }
};


/**
 * Provides a DataOut stream by wrapping a std::ostream.
 */
class OStreamDataOut : public DataOut
{
    std::ostream& out;
public:

    OStreamDataOut(std::ostream& out_) : out(out_) {}

    virtual bool write(uint8_t data) override {
        out.put(char(data));
        return true;
    }
};


/**
 * A connection source that emulates a connection by two string streams, used for testing
 * 
 **/

class StringPairConnection : public Connection {
private:
    std::stringstream in;
    std::stringstream out;
    IStreamDataIn dataIn;
    OStreamDataOut dataOut;
    bool connected;

public:
    StringPairConnection(std::string& _in, std::string& _out) :
        in(_in),
        out(_out),
        dataIn(in),
        dataOut(out),
        connected(true)
    {}

    virtual DataOut& getDataOut() override final {
        return dataOut;
    };
    virtual DataIn& getDataIn() override final {
        return dataIn;
    }

    virtual bool isConnected() override final {
        return connected;
    }

    void disconnect(){
        connected = false;
    }
};

class StringPairConnectionSource : public ConnectionSource {
private:
    std::queue<std::unique_ptr<StringPairConnection>> connectionQueue;

public:
    void add(std::string& in, std::string& out){
        connectionQueue.emplace(new StringPairConnection(in, out));
    }

    virtual std::unique_ptr<Connection> newConnection() override final {
        std::unique_ptr<Connection> retval = std::move(connectionQueue.front());
        connectionQueue.pop();
        return retval;
    }
};
} // end namespace cbox