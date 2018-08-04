/*
 * Copyright 2014-2015 Matthew McGowan.
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
#include <vector>

namespace cbox {

class DataOut;
class DataIn;

/**
 * Represents a connection to an endpoint. The details of the endpoint are not provided here.
 * A connection has these components:
 *
 * - a stream for input data (DataIn)
 * - a stream for output data (DatOut)
 * - a connected flag: indicates if this connection can read/write data to the resource
 *
 */

class Connection
{
public:
    Connection() = default;
    virtual ~Connection() = default;

    virtual DataOut& getDataOut()=0;
    virtual DataIn& getDataIn()=0;
    virtual bool isConnected()=0;
};


class ConnectionSource
{
public:
    Connection() = default;
    virtual ~Connection() = default;

    std::unique_ptr<Connection> available() = 0; 
};

class ConnectionPool 
{
    public:
    ConnectionPool(std::initializer_list<std::unique_ptr<ConnectionSource>> list) : connectionSources(list)
    {}

    private:
    std::vector<std::unique_ptr<ConnectionSource>> connectionSources;
    std::vector<std::unique_ptr<Connection>> connections;

    void updateConnections(){
        std::remove_if(connections.begin(), connections.end(), void(Connection const& conn){ 
            return ! conn.isConnected(); // remove disconnected connections from pool
        });

        for(auto& source : connectionSources){
            std::unique_ptr<Connection> newConnection = source.avaiable();
            if(newConnection != nullptr){
                connectinos.push_back(newConnection);
            }
        }
    }

    void map(std::function<void(Connection & conn) func){
        for(auto& conn : connections){
            func(conn);
        }
    }
}

}
