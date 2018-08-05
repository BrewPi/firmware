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

#include "Ticks.h"
#include "TicksObject.h"
#include "Object.h"
#include "Container.h"
#include "Box.h"

#include "blox/OneWireTempSensorBlock.h"
//#include "blox/PidBlock.h"
//#include "blox/SensorSetPointPairBlock.h"
#include "blox/SetPointSimpleBlock.h"
#include "OneWireBusBlock.h"
#include "EepromTypes.h"
#include "EepromAccessImpl.h"
#include <memory>


//OneWireBusBlock oneWireBus;
DelayImpl wait = DelayImpl(DELAY_IMPL_CONFIG);
TicksImpl baseticks;
cbox::ScaledTicksValue<TicksImpl> ticks(baseticks);

class DeviceIdObject : public cbox::RawStreamObject<std::array<uint8_t,12>> {
public:
    DeviceIdObject(){
#if PLATFORM_ID != PLATFORM_GCC
        obj = ID1;
#else
        obj = {0};
#endif
	}

	virtual cbox::obj_type_t typeID() override final{
		// use function overloading and templates to manage type IDs in a central place (TypeRegistry)
		return cbox::resolveTypeID(this);
	}
};


#define OBJECT_FACTORY_ENTRY(className) {cbox::resolveTypeID<className>(), cbox::createObject<className>}

cbox::ObjectFactory objectFactories[] = {
        OBJECT_FACTORY_ENTRY(OneWireTempSensorBlock),
		OBJECT_FACTORY_ENTRY(SetPointSimpleBlock)
		//OBJECT_FACTORY_ENTRY(SensorSetPointPairBlock),
		//OBJECT_FACTORY_ENTRY(PidBlock)
};

namespace cbox {
void connectionStarted(DataOut& out)
{
    out.writeAnnotation("Connected to BrewBlox v0.1.0");
    out.flush();
}
}

OneWireBusBlock oneWireBus(0);

cbox::ObjectContainer objects = {
        cbox::ContainedObject(1, 0xFF, std::make_shared<DeviceIdObject>()),
        cbox::ContainedObject(2, 0xFF, std::shared_ptr<cbox::Object>(&oneWireBus)),
};

EepromAccess eeprom;
cbox::EepromObjectStorage objectStore(eeprom);

cbox::ConnectionPool connections = {};

cbox::Box appBox(objects, objectStore, connections);

/**
 * The application supplied object factory.
 */
std::shared_ptr<cbox::Object> createApplicationObject(cbox::obj_type_t typeId, cbox::DataIn& in, cbox::CommandError& errorCode)
{
    errorCode = cbox::CommandError::no_error;
    std::shared_ptr<cbox::Object> obj;
    std::shared_ptr<cbox::Object> (*createFn)(cbox::DataIn& def, cbox::StreamResult &streamResult) = nullptr;
    for(uint8_t i =0; i<sizeof(objectFactories)/sizeof(objectFactories[0]); i++) {
    	if(typeId == objectFactories[i].typeId) {
    		createFn = objectFactories[i].createFn;
    	}
    }
    if(createFn == nullptr){
        errorCode = cbox::CommandError::invalid_type;
    }
    else {
        cbox::StreamResult streamResult;
        obj = createFn(in, streamResult);
        if (obj == nullptr) {
            errorCode = cbox::CommandError::insufficient_heap;
        }
        else if ( streamResult != cbox::StreamResult::success){
            errorCode = cbox::CommandError::stream_error;
            return nullptr;
        }

    }
    return obj;
}
