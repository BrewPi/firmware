/*
 * Copyright 2016 BrewPi/Elco Jacobs.
 * Copyright 2016 Matthew McGowan.
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

#include "ControllerMixins.h"
#include "Pid.h"
#include "TempSensor.h"
#include "TempSensorBasic.h"
#include "TempSensorMock.h"
#include "OneWireTempSensor.h"
#include "TempSensorExternal.h"

// These macros are equivalent to ESJ, except for that they add obj-> in front of the member variable name

// Serializable element with more elements to follow (alternative to JSON_E)
#define JSON_OE(json_adapter,class_member)  JSON::stream(json_adapter,_ASTRING(#class_member),obj->class_member,true)
// Serializable terminator, keep things simple for the JSON writer. No stream manipulations required (alternative to JSON_T)
#define JSON_OT(json_adapter,class_member)  JSON::stream(json_adapter,_ASTRING(#class_member),obj->class_member,false)



void PidMixin::serialize(JSON::Adapter & adapter)
{
    Pid * obj = (Pid *) this;

    JSON::Class root(adapter, "Pid");
    std::string name(getName());    // get name as std string for json_writer
    JSON_E(adapter, name);
    JSON_OE(adapter, enabled);
    JSON_OE(adapter, setPoint);
    JSON_OE(adapter, inputSensor);
    JSON_OE(adapter, inputError);
    JSON_OE(adapter, Kp);
    JSON_OE(adapter, Ti);
    JSON_OE(adapter, Td);
    JSON_OE(adapter, p);
    JSON_OE(adapter, i);
    JSON_OE(adapter, d);
    JSON_OE(adapter, actuatorIsNegative);
    JSON_OT(adapter, outputActuator);
}


void TempSensorMixin::serialize(JSON::Adapter & adapter)
{
    TempSensor * obj = (TempSensor *) this;

    JSON::Class root(adapter, "TempSensor");
    std::string name(getName());    // get name as std string for json_writer
    JSON_E(adapter, name);
    JSON_OT(adapter, sensor);
}


void TempSensorMockMixin::serialize(JSON::Adapter& adapter){
    TempSensorMock * obj = (TempSensorMock *) this;

    JSON::Class root(adapter, "TempSensorMock");
    JSON_OE(adapter, value);
    JSON_OT(adapter, connected);
}


void OneWireTempSensorMixin::serialize(JSON::Adapter& adapter){
    OneWireTempSensor * obj = (OneWireTempSensor *) this;

    JSON::Class root(adapter, "OneWireTempSensor");

    temp_t value = obj->read();
    JSON_E(adapter, value);

    bool connected = obj->isConnected();
    JSON_E(adapter, connected);

    char addressBuf[17];
    printBytes(obj->sensorAddress, 8, addressBuf); // print to hex string
    std::string address(addressBuf); // convert to std string
    JSON_E(adapter, address);

    JSON_OT(adapter, calibrationOffset);
}


void TempSensorDisconnectedMixin::serialize(JSON::Adapter& adapter){
    TempSensorDisconnected * obj = (TempSensorDisconnected *) this;

    temp_t value = obj->read();
    bool connected = false;
    JSON::Class root(adapter, "TempSensorDisconnected");
    JSON_E(adapter, value);
    JSON_T(adapter, connected);
}

void TempSensorExternalMixin::serialize(JSON::Adapter& adapter){
    TempSensorExternal * obj = (TempSensorExternal *) this;

    JSON::Class root(adapter, "TempSensorExternal");
    JSON_OE(adapter, value);
    JSON_OT(adapter, connected);
}
