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

#include "../../controlbox/src/cbox/spark/SparkEepromAccess.h"
#include "../../lib/inc/DS248x.h"
#include "blox/OneWireBusBlock.h"
#include "blox/OneWireTempSensorBlock.h"
#include "blox/SetPointSimpleBlock.h"
#include "blox/SetpointSensorPairBlock.h"
#include "blox/SysInfoBlock.h"
#include "blox/TicksBlock.h"
#include "cbox/Box.h"
#include "cbox/Connections.h"
#include "cbox/EepromObjectStorage.h"
#include "cbox/ObjectContainer.h"
#include "cbox/ObjectFactory.h"
#include "cbox/spark/ConnectionsTcp.h"
#include "cbox/spark/SparkEepromAccess.h"
#include "spark/modules/Platform/Platform.h"
#include "theOneWire.h"
#include "wiring/TicksWiring.h"
#include <memory>

namespace cbox {
void
connectionStarted(DataOut& out)
{
    //    out.writeAnnotation("Connected to BrewBlox v0.1.0");
}
}

class SetpointSensorPairBlock;

using TicksClass = Ticks<TicksWiring>;
TicksClass ticks;

cbox::Box&
makeBrewBloxBox()
{
    static cbox::ObjectContainer objects = {
        cbox::ContainedObject(1, 0xFF, std::make_shared<SysInfoBlock>()),
        cbox::ContainedObject(2, 0xFF, std::make_shared<TicksBlock<TicksClass>>(ticks)),
        cbox::ContainedObject(3, 0xFF, std::make_shared<OneWireBusBlock>(theOneWire()))};

    auto makeSetpointSensorPair = []() {
        return std::make_unique<SetpointSensorPairBlock>(objects);
    };

    static cbox::ObjectFactory objectFactory = {
        OBJECT_FACTORY_ENTRY(OneWireTempSensorBlock),
        OBJECT_FACTORY_ENTRY(SetPointSimpleBlock),
        {cbox::resolveTypeId<SetpointSensorPairBlock>(), makeSetpointSensorPair}};

    static cbox::SparkEepromAccess eeprom;
    static cbox::EepromObjectStorage objectStore(eeprom);
    static cbox::TcpConnectionSource tcpSource(8332);
    static cbox::ConnectionPool connections = {tcpSource};

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
    static DS248x owDriver(0);
    static OneWire ow(owDriver);
    return ow;
}
