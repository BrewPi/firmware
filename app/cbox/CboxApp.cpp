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
#include "EepromTypes.h"
#include "EepromAccessImpl.h"

#include "Values.h"
#include "Ticks.h"
#include "ValueTicks.h"
#include "ValueModels.h"
#include "Commands.h"

#include "PersistChangeValue.h"
#include "OneWireBusCBox.h"
#include "OneWireTempSensorBloc.h"
#include "SetPointSimpleBloc.h"
#include "SensorSetPointPairBloc.h"


OneWireBusCBox oneWireBus;
DelayImpl wait = DelayImpl(DELAY_IMPL_CONFIG);
TicksImpl baseticks;
ScaledTicksValue ticks;
EepromAccess eepromAccess;


Commands::ObjectFactory createObjectHandlers[] = {
        nullFactory,                                            // type 0
        ScaledTicksValue::create,                               // type 1
        DynamicContainer::create,                               // type 2
        EepromValue::create,                                    // type 3
        PersistChangeValue::create,                             // type 4
        IndirectValue::create,                                  // type 5
        OneWireTempSensorBloc::create,                          // type 6
        SetPointSimpleBloc::create,                             // type 7
        SensorSetPointPairBloc::create,                             // type 8
        NULL

        // When defining a new object type, add the handler above the last NULL value (it's just there to make
        // editing the code easier).
        // The Object definition passed to the create handler contains the stream and the block length.
};


void connectionStarted(StandardConnection& connection, DataOut& out)
{
    out.writeAnnotation("\"a\":\"brewpi\",\"v\":\"0.3.0\"");
    out.flush();
#if PLATFORM_ID!=3
    StandardConnectionDataType& data = connection.getData();
    // this is needed since the Photon writes data to serial before it's
    // connected and the data may be discarded on some platforms.
    data.callback_until_first_request = true;
    wait.millis(100);
#endif
}

Container& systemRootContainer()
{
    static data_block_ref id;
    platform_device_id(id);

    static ExternalReadOnlyValue idValue(id.data, id.size);
    idValue.setTypeID(0);       // this is just a buffer  for the ID
    ticks.setTypeID(1);
    // todo - lookup the type ID from the xxx::create function. This can
    // be resolved at compile-time.

    static Object* values[] = { &idValue, &ticks, &oneWireBus };
    static FixedContainer root(sizeof(values)/sizeof(values[0]), values);
    return root;
}

/**
 * Provide the root container for the current profile. This should be dynamically allocated
 * (since it will be disposed by calling delete.)
 *
 */
Container* createRootContainer()
{
    DynamicContainer* d = new DynamicContainer();
    return d;
}

/**
 * The application supplied object factory.
 * Fetches the object type from the stream and looks this up against an array of object factories.
 * It's critical that the create code reads len bytes from the stream so that the data is
 * Spooled to eeprom to the persisted object definition.
 */
int8_t createApplicationObject(Object*& result, ObjectDefinition& def, bool dryRun)
{
    uint8_t type = def.type;
    int8_t error = errorCode(no_error);
    if (type>=sizeof(createObjectHandlers)/sizeof(createObjectHandlers[0])) {
        error = errorCode(invalid_type);
    }
    else {
        if (dryRun){
            type = 0; // null object creator. Ensures stream is properly consumed even for invalid type values.
        }
        result = createObjectHandlers[type](def);
        if (!result) {
            error = errorCode(insufficient_heap);
        }
    }
    return error;
}

