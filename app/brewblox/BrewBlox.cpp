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

#include "blox/OneWireBusBlock.h"
#include "blox/SetPointSimpleBlock.h"
#include "blox/SetpointSensorPairBlock.h"
#include "blox/SysInfoBlock.h"
#include "blox/TempSensorMockBlock.h"
#include "blox/TempSensorOneWireBlock.h"
#include "blox/TicksBlock.h"
#include "cbox/Box.h"
#include "cbox/Connections.h"
#include "cbox/EepromObjectStorage.h"
#include "cbox/ObjectContainer.h"
#include "cbox/ObjectFactory.h"
#include <memory>

#if defined(SPARK)
#include "cbox/spark/ConnectionsTcp.h"
#include "cbox/spark/SparkEepromAccess.h"
#include "wiring/TicksWiring.h"
using EepromAccessImpl = cbox::SparkEepromAccess;
using TicksClass = Ticks<TicksWiring>;
#else
#include "MockTicks.h"
#include "cbox/ArrayEepromAccess.h"
#include "cbox/ConnectionsStringStream.h"
using EepromAccessImpl = cbox::ArrayEepromAccess<2048>;
using TicksClass = Ticks<MockTicks>;
#endif

#if !defined(PLATFORM_ID) || PLATFORM_ID == 3
#include "OneWireNull.h"
using OneWireDriver = OneWireNull;
#else
#include "DS248x.h"
using OneWireDriver = DS248x;
#endif

namespace cbox {
void
connectionStarted(DataOut& out)
{
    //    out.writeAnnotation("Connected to BrewBlox v0.1.0");
}
}

class SetpointSensorPairBlock;

TicksClass ticks;

// define separately to make it available for tests
#if !defined(SPARK)
cbox::StringStreamConnectionSource&
testConnectionSource()
{
    static cbox::StringStreamConnectionSource connSource;
    return connSource;
}
#endif

cbox::Box&
makeBrewBloxBox()
{
    static cbox::ObjectContainer objects = {
        cbox::ContainedObject(1, 0xFF, std::make_shared<SysInfoBlock>()),
        cbox::ContainedObject(2, 0xFF, std::make_shared<TicksBlock<TicksClass>>(ticks)),
        cbox::ContainedObject(3, 0xFF, std::make_shared<OneWireBusBlock>(theOneWire()))};

    static cbox::ObjectFactory objectFactory = {
        {TempSensorOneWireBlock::staticTypeId(), std::make_unique<TempSensorOneWireBlock>},
        {SetPointSimpleBlock::staticTypeId(), std::make_unique<SetPointSimpleBlock>},
        {SetpointSensorPairBlock::staticTypeId(), []() { return std::make_unique<SetpointSensorPairBlock>(objects); }},
        {TempSensorMockBlock::staticTypeId(), std::make_unique<TempSensorMockBlock>}};

    static EepromAccessImpl eeprom;
    static cbox::EepromObjectStorage objectStore(eeprom);
#if defined(SPARK)
    static cbox::TcpConnectionSource tcpSource(8332);
    static cbox::ConnectionPool connections = {tcpSource};
#else
    static cbox::ConnectionPool connections = {testConnectionSource()};
#endif

    static cbox::Box box(objectFactory, objects, objectStore, connections);

    return box;
}

cbox::Box&
brewbloxBox()
{
    static cbox::Box& box = makeBrewBloxBox();
    return box;
}

OneWire&
theOneWire()
{
    static OneWireDriver owDriver(0);
    static OneWire ow(owDriver);
    return ow;
}
