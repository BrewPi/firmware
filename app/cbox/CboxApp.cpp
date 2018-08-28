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

#include "Platform.h"

#include "Box.h"
#include "Connections.h"
#include "ConnectionsTcp.h"
#include "ObjectFactory.h"

#include "blox/OneWireTempSensorBlock.h"
//#include "blox/PidBlock.h"
#include "EepromObjectStorage.h"
#include "blox/OneWireBusBlock.h"
#include "blox/SensorSetPointPairBlock.h"
#include "blox/SetPointSimpleBlock.h"
#include "theOneWire.h"
#include <memory>

#include "ObjectContainer.h"
#include "SysInfoBlock.h"
#include "TicksWiring.h"
#include "blox/TicksBlock.h"
#include "theOneWire.h"

namespace cbox {
void
connectionStarted(DataOut& out)
{
    //    out.writeAnnotation("Connected to BrewBlox v0.1.0");
}
}

class SensorSetPointPairBlock;

using TicksClass = Ticks<TicksWiring>;
TicksClass ticks;

cbox::Box&
makeTheBox()
{
    static cbox::ObjectContainer objects = {
        cbox::ContainedObject(1, 0xFF, std::make_shared<SysInfoBlock>()),
        cbox::ContainedObject(2, 0xFF, std::make_shared<TicksBlock<TicksClass>>(ticks)),
        cbox::ContainedObject(3, 0xFF, std::make_shared<OneWireBusBlock>(theOneWire()))};

    static cbox::ObjectFactory objectFactory = {
        OBJECT_FACTORY_ENTRY(OneWireTempSensorBlock),
        OBJECT_FACTORY_ENTRY(SetPointSimpleBlock),
        OBJECT_FACTORY_ENTRY(SensorSetPointPairBlock)
        //OBJECT_FACTORY_ENTRY(PidBlock)
    };

    static EepromAccess eeprom;
    static cbox::EepromObjectStorage objectStore(eeprom);

    static cbox::TcpConnectionSource tcpSource(8332);
    static cbox::ConnectionPool connections = {tcpSource};

    static cbox::Box box(objectFactory, objects, objectStore, connections);

    return box;
}

cbox::Box&
brewbloxBox()
{
    static cbox::Box& box = makeTheBox();
    return box;
}

OneWire&
theOneWire()
{
    static OneWire ow(0);
    return ow;
}
