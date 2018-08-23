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

public:

    IStreamDataIn(std::istream& in_) : in(in_) {}

    virtual bool hasNext() override {
        int next = in.peek();
        // we don't want the peek to set the eof or fail flag because no input is available
        // this prevents streaming in new data for testing
        in.clear(in.rdstate() & std::istream::badbit); // only keep badbit
        return next !=EOF;
    }

    virtual uint8_t next() override {
        char val = 0;
        if(hasNext()){
            in.get(val);
        }
        return uint8_t(val);
    }

    virtual uint8_t peek() override {
        if(hasNext()){
            return uint8_t(in.peek());
        }
        return 0;
    }

    virtual stream_size_t available() override {
        return hasNext() ? 1 : 0; // don't use in.eof() as the stream is already in error state then
    }
};


/**
 * Provides a DataOut stream by wrapping a std::ostream.
 */
class OStreamDataOut final : public DataOut
{
    std::ostream& out;
public:

    OStreamDataOut(std::ostream& out_) : out(out_) {}

    virtual void writeAnnotation(std::string && ann) override final {};
    virtual void writeResponseSeparator() override final {};
    virtual void writeListSeparator() override final {};
    virtual void endMessage() override final {};
    virtual void flush() override final {};

    virtual bool write(uint8_t data) override final {
        out.put(char(data));
        return true;
    }
};


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
 StringStreamConnection(std::stringstream& _in, std::stringstream& _out) :
        in(_in),
        out(_out),
        dataIn(_in),
        dataOut(_out)
    {
    }

    virtual DataOut& getDataOut() override final {
        return dataOut;
    };
    virtual DataIn& getDataIn() override final {
        return dataIn;
    }

    virtual bool isConnected() override final {
        return !(in.bad() || out.bad()); // use badbit of either stream to simulate disconnect
    }
};

class StringStreamConnectionSource : public ConnectionSource {
private:
    std::queue<std::unique_ptr<StringStreamConnection>> connectionQueue;

public:
    void add(std::stringstream& in, std::stringstream& out){
        auto newConnection = std::make_unique<StringStreamConnection>(in, out);
        connectionQueue.push(std::move(newConnection));
    }

    virtual std::unique_ptr<Connection> newConnection() override final {
        if(connectionQueue.empty()){
            return nullptr;
        }
        std::unique_ptr<Connection> retval = std::move(connectionQueue.front());
        connectionQueue.pop();
        return std::move(retval);
    }
};
} // end namespace cbox
