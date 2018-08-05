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

#include "Connections.h"
#include "spark_wiring_tcpserver.h"

class TcpConnnection : public StreamConnection {
private:
    TcpClient client;
public:
    TcpConnection(TcpClient _client) :
        client(_client),
        StreamConnection(client)
    {}
    ~TcpConnection(){
        client->stop();    
    }
}

class TcpConnectionSource : public ConnectionSource
{
private:
    TCPServer server;
public:

    TcpConnectionSource() : server(8332){}
    
    std::unique_ptr<Connection> newConnection(){
        TCPClient newClient = server.available();
        if(newClient.isConnected()){
            return std::make_unique<TcpConnection>(newClient);
        }
        return nullptr;
    }
}
