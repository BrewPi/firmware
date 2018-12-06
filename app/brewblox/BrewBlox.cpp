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

#include "AppTicks.h"
#include "Board.h"
#include "Logger.h"
#include "blox/ActuatorAnalogMockBlock.h"
#include "blox/ActuatorOffsetBlock.h"
#include "blox/ActuatorPinBlock.h"
#include "blox/ActuatorPwmBlock.h"
#include "blox/BalancerBlock.h"
#include "blox/MutexBlock.h"
#include "blox/OneWireBusBlock.h"
#include "blox/PidBlock.h"
#include "blox/SetpointProfileBlock.h"
#include "blox/SetpointSensorPairBlock.h"
#include "blox/SetpointSimpleBlock.h"
#include "blox/SysInfoBlock.h"
#include "blox/TempSensorMockBlock.h"
#include "blox/TempSensorOneWireBlock.h"
#include "cbox/Box.h"
#include "cbox/Connections.h"
#include "cbox/EepromObjectStorage.h"
#include "cbox/ObjectContainer.h"
#include "cbox/ObjectFactory.h"
#include "cbox/spark/SparkEepromAccess.h"
#include <memory>

using EepromAccessImpl = cbox::SparkEepromAccess;

// define separately to make it available for tests
#if !defined(SPARK)
cbox::StringStreamConnectionSource&
testConnectionSource()
{
    static cbox::StringStreamConnectionSource connSource;
    return connSource;
}
#endif

cbox::ConnectionPool&
theConnectionPool()
{
#if defined(SPARK)
    static cbox::TcpConnectionSource tcpSource(8332);
#if PLATFORM_ID != 3 || defined(STDIN_SERIAL)
    static cbox::SerialConnectionSource serialSource;
    static cbox::ConnectionPool connections = {tcpSource, serialSource};
#else
    static cbox::ConnectionPool connections = {tcpSource};
#endif
#else
    static cbox::ConnectionPool connections = {testConnectionSource()};
#endif

    return connections;
}

cbox::Box&
makeBrewBloxBox()
{
    static cbox::ObjectContainer objects({
        // profiles will be at position 1
        cbox::ContainedObject(2, 0xFF, std::make_shared<SysInfoBlock>()),
        cbox::ContainedObject(3, 0xFF, std::make_shared<TicksBlock<TicksClass>>(ticks)),
        cbox::ContainedObject(4, 0xFF, std::make_shared<OneWireBusBlock>(theOneWire())),
    });

#ifdef PIN_V3_BOTTOM1
    objects.add(std::make_shared<ActuatorPinBlock>(objects, PIN_V3_BOTTOM1), 0xFF, 10);
#endif
#ifdef PIN_V3_BOTTOM2
    objects.add(std::make_shared<ActuatorPinBlock>(objects, PIN_V3_BOTTOM2), 0xFF, 11);
#endif
#ifdef PIN_V3_TOP1
    objects.add(std::make_shared<ActuatorPinBlock>(objects, PIN_V3_TOP1), 0xFF, 12);
#endif
#ifdef PIN_V3_TOP2
    objects.add(std::make_shared<ActuatorPinBlock>(objects, PIN_V3_TOP2), 0xFF, 13);
#endif
#ifdef PIN_V3_TOP3
    objects.add(std::make_shared<ActuatorPinBlock>(objects, PIN_V3_TOP3), 0xFF, 14);
#endif
#ifdef PIN_ACTUATOR0
    if (getSparkVersion() == SparkVersion::V2) {
        objects.add(std::make_shared<ActuatorPinBlock>(objects, PIN_ACTUATOR0), 0xFF, 15);
    }
#endif
#ifdef PIN_ACTUATOR1
    objects.add(std::make_shared<ActuatorPinBlock>(objects, PIN_ACTUATOR1), 0xFF, 16);
#endif
#ifdef PIN_ACTUATOR2
    objects.add(std::make_shared<ActuatorPinBlock>(objects, PIN_ACTUATOR2), 0xFF, 17);
#endif
#ifdef PIN_ACTUATOR3
    objects.add(std::make_shared<ActuatorPinBlock>(objects, PIN_ACTUATOR3), 0xFF, 18);
#endif

    static cbox::ObjectFactory objectFactory = {
        {TempSensorOneWireBlock::staticTypeId(), std::make_shared<TempSensorOneWireBlock>},
        {SetpointSimpleBlock::staticTypeId(), std::make_shared<SetpointSimpleBlock>},
        {SetpointSensorPairBlock::staticTypeId(), []() { return std::make_shared<SetpointSensorPairBlock>(objects); }},
        {TempSensorMockBlock::staticTypeId(), std::make_shared<TempSensorMockBlock>},
        {ActuatorAnalogMockBlock::staticTypeId(), std::make_shared<ActuatorAnalogMockBlock>},
        {PidBlock::staticTypeId(), []() { return std::make_shared<PidBlock>(objects); }},
        {ActuatorPwmBlock::staticTypeId(), []() { return std::make_shared<ActuatorPwmBlock>(objects); }},
        {ActuatorOffsetBlock::staticTypeId(), []() { return std::make_shared<ActuatorOffsetBlock>(objects); }},
        {BalancerBlock::staticTypeId(), std::make_shared<BalancerBlock>},
        {MutexBlock::staticTypeId(), std::make_shared<MutexBlock>},
        {SetpointProfileBlock::staticTypeId(), []() { return std::make_shared<SetpointProfileBlock>(bootTimeRef()); }},
    };

    static EepromAccessImpl eeprom;
    static cbox::EepromObjectStorage objectStore(eeprom);
    static cbox::ConnectionPool& connections = theConnectionPool();

    std::vector<std::unique_ptr<cbox::ScanningFactory>> scanningFactories;
#if PLATFORM_ID == 3
    scanningFactories.push_back(std::unique_ptr<cbox::ScanningFactory>(new MockOneWireScanningFactory(objects, theOneWire())));
#else
    scanningFactories.push_back(std::unique_ptr<cbox::ScanningFactory>(new OneWireScanningFactory(objects, theOneWire())));
#endif

    static cbox::Box box(objectFactory, objects, objectStore, connections, std::move(scanningFactories));

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
    static auto owDriver = OneWireDriver(ONEWIRE_ARG);
    static auto ow = OneWire(owDriver);
    return ow;
}

Logger&
logger()
{
    static auto logger = Logger([](Logger::LogLevel level, const std::string& log) {
        cbox::DataOut& out = theConnectionPool().logDataOut();
        out.write('<');
        for (const auto& c : log) {
            out.write(c);
        }
        out.write('>');
    });
    return logger;
}

void
updateBrewbloxBox()
{
    brewbloxBox().update(ticks.millis());
#if PLATFORM_ID == 3
    ticks.delayMillis(10); // prevent 100% cpu usage
#endif
}

namespace cbox {
void
connectionStarted(DataOut& out)
{
    char msg[] = "<!Connected to BrewBlox v0.1.0>";
    out.writeBuffer(&msg, strlen(msg));
}
}
