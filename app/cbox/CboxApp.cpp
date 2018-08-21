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

#include "TicksObject.h"
#include "Object.h"
#include "Container.h"
#include "Box.h"
#include "ObjectFactory.h"
#include "Connections.h"
#include "ConnectionsTcp.h"

#include "blox/OneWireTempSensorBlock.h"
//#include "blox/PidBlock.h"
#include "blox/SensorSetPointPairBlock.h"
#include "blox/SetPointSimpleBlock.h"
#include "OneWire.h"
#include "OneWireBusBlock.h"
#include "EepromObjectStorage.h"
#include <memory>
#include "TicksWiring.h"

OneWire oneWireBus(0);

class DeviceIdObject : public cbox::RawStreamObject<std::array<uint8_t,12>> {
public:
    DeviceIdObject(){
#if PLATFORM_ID != PLATFORM_GCC
        obj = ID1;
#else
        obj = {0};
#endif
	}

	virtual cbox::obj_type_t typeId() const override final{
		// use function overloading and templates to manage type IDs in a central place (TypeRegistry)
		return cbox::resolveTypeId(this);
	}
};


namespace cbox {
void connectionStarted(DataOut& out)
{
    out.writeAnnotation("Connected to BrewBlox v0.1.0");
    out.flush();
}
}

class SensorSetPointPairBlock;

using TicksClass = Ticks<TicksWiring>;
TicksClass ticks;

cbox::Box & brewbloxBox(){
    static cbox::ObjectContainer objects = {
            cbox::ContainedObject(1, 0xFF, std::make_shared<DeviceIdObject>()),
            cbox::ContainedObject(2, 0xFF, std::make_shared<cbox::TicksObject<TicksClass>>(ticks)),
            cbox::ContainedObject(3, 0xFF, std::make_shared<OneWireBusBlock>(oneWireBus))
    };

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

    static cbox::Box appBox(objectFactory, objects, objectStore, connections);

    return appBox;
}



