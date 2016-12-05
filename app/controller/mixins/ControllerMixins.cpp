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
#include "TempSensorInterface.h"
#include "TempSensorMock.h"
#include "OneWireTempSensor.h"
#include "TempSensorExternal.h"
#include "TempSensorFallback.h"
#include "ActuatorInterfaces.h"
#include "ActuatorTimeLimited.h"
#include "ActuatorSetPoint.h"
#include "ActuatorPwm.h"
#include "ActuatorMutexGroup.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorMocks.h"
#include "SetPoint.h"

#if WIRING

#include "ActuatorPin.h"
#include "ActuatorOneWire.h"
#include "ValveController.h"

#endif

// These macros are equivalent to ESJ, except for that they add obj-> in front of the member variable name
// use JSON_E for local variables and JSON_OE for member variables of the obj pointer
// Serializable element with more elements to follow (alternative to JSON_E)
#define JSON_OE(json_adapter,class_member)  JSON::stream(json_adapter,_ASTRING(#class_member),obj->class_member,true)

// Serializable terminator, keep things simple for the JSON writer. No stream manipulations required (alternative to JSON_T)
#define JSON_OT(json_adapter,class_member)  JSON::stream(json_adapter,_ASTRING(#class_member),obj->class_member,false)


void PidMixin::serialize(JSON::Adapter & adapter)
{
    Pid * obj = static_cast<Pid *>(this);

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
    TempSensor * obj = static_cast<TempSensor *>(this);

    JSON::Class root(adapter, "TempSensor");
    std::string name(getName());    // get name as std string for json_writer
    JSON_E(adapter, name);
    JSON_OT(adapter, sensor);
}

void TempSensorMockMixin::serialize(JSON::Adapter & adapter)
{
    TempSensorMock * obj = static_cast<TempSensorMock *>(this);

    JSON::Class root(adapter, "TempSensorMock");
    JSON_OE(adapter, value);
    JSON_OT(adapter, connected);
}

void OneWireTempSensorMixin::serialize(JSON::Adapter & adapter)
{
    OneWireTempSensor * obj = static_cast<OneWireTempSensor *>(this);

    JSON::Class root(adapter, "OneWireTempSensor");

    temp_t value = obj -> read();

    JSON_E(adapter, value);

    bool connected = obj -> isConnected();

    JSON_E(adapter, connected);

    char addressBuf[17];

    printBytes(obj -> sensorAddress, 8, addressBuf);    // print to hex string
    std::string address(addressBuf);    // convert to std string
    JSON_E(adapter, address);
    JSON_OT(adapter, calibrationOffset);
}

void TempSensorDisconnectedMixin::serialize(JSON::Adapter & adapter)
{
    TempSensorDisconnected * obj       = static_cast<TempSensorDisconnected *>(this);
    temp_t                   value     = obj -> read();
    bool                     connected = false;

    JSON::Class root(adapter, "TempSensorDisconnected");
    JSON_E(adapter, value);
    JSON_T(adapter, connected);
}

void TempSensorExternalMixin::serialize(JSON::Adapter & adapter)
{
    TempSensorExternal * obj = static_cast<TempSensorExternal *>(this);

    JSON::Class root(adapter, "TempSensorExternal");
    JSON_OE(adapter, value);
    JSON_OT(adapter, connected);
}

void TempSensorFallbackMixin::serialize(JSON::Adapter & adapter)
{
    TempSensorFallback * obj = static_cast<TempSensorFallback *>(this);

    JSON::Class root(adapter, "TempSensorFallback");
    JSON_OE(adapter, onBackupSensor);
    TempSensorInterface * sensor = obj->activeSensor();
    JSON_T(adapter, sensor);
}

void ActuatorTimeLimitedMixin::serialize(JSON::Adapter & adapter)
{
    ActuatorTimeLimited * obj = static_cast<ActuatorTimeLimited *>(this);

    JSON::Class root(adapter, "ActuatorTimeLimited");
    JSON_OE(adapter, minOnTime);
    JSON_OE(adapter, minOffTime);
    JSON_OE(adapter, maxOnTime);
    JSON_OE(adapter, state);
    JSON_OT(adapter, target);
}

void ActuatorSetPointMixin::serialize(JSON::Adapter & adapter)
{
    ActuatorSetPoint * obj = static_cast<ActuatorSetPoint *>(this);

    JSON::Class root(adapter, "ActuatorSetPoint");
    JSON_OE(adapter, targetSetPoint);
    JSON_OE(adapter, targetSensor);
    JSON_OE(adapter, referenceSetPoint);

    temp_t output = obj -> getValue();

    JSON_E(adapter, output);

    temp_t achieved = obj -> readValue();

    JSON_E(adapter, achieved);
    JSON_OE(adapter, minimum);
    JSON_OT(adapter, maximum);
}

void ActuatorPwmMixin::serialize(JSON::Adapter & adapter)
{
    ActuatorPwm * obj = static_cast<ActuatorPwm *>(this);

    JSON::Class root(adapter, "ActuatorPwm");
    JSON_OE(adapter, value);

    ticks_seconds_t period = obj -> getPeriod();    // don't use member directly, but value in seconds

    JSON_E(adapter, period);
    JSON_OE(adapter, minVal);
    JSON_OE(adapter, maxVal);
    JSON_OT(adapter, target);
}

void ActuatorMutexGroupMixin::serialize(JSON::Adapter & adapter)
{
    ActuatorMutexGroup * obj = static_cast<ActuatorMutexGroup *>(this);

    JSON::Class root(adapter, "ActuatorMutexGroup");
    JSON_OE(adapter, deadTime);
    ticks_millis_t waitTime = obj->getWaitTime();
    JSON_T(adapter, waitTime);
}

void ActuatorMutexDriverMixin::serialize(JSON::Adapter & adapter)
{
    ActuatorMutexDriver * obj = static_cast<ActuatorMutexDriver *>(this);

    JSON::Class root(adapter, "ActuatorMutexDriver");
    JSON_OE(adapter, mutexGroup);
    JSON_OT(adapter, target);
}

void ActuatorValueMixin::serialize(JSON::Adapter & adapter)
{
    ActuatorValue * obj = static_cast<ActuatorValue *>(this);

    JSON::Class root(adapter, "ActuatorValue");
    JSON_OE(adapter, value);
    JSON_OE(adapter, minimum);
    JSON_OT(adapter, maximum);
}

void ActuatorBoolMixin::serialize(JSON::Adapter & adapter)
{
    ActuatorBool * obj = static_cast<ActuatorBool *>(this);

    JSON::Class root(adapter, "ActuatorBool");
    JSON_OT(adapter, state);
}

void ActuatorNopMixin::serialize(JSON::Adapter & adapter)
{
    ActuatorNop * obj = static_cast<ActuatorNop *>(this);

    JSON::Class root(adapter, "ActuatorNop");

    bool state = obj -> isActive();

    JSON_T(adapter, state);
}

void ActuatorInvalidMixin::serialize(JSON::Adapter & adapter)
{
    ActuatorInvalid * obj     = static_cast<ActuatorInvalid *>(this);
    temp_t            value   = obj -> getValue();
    temp_t            minimum = obj -> min();
    temp_t            maximum = obj -> max();

    JSON::Class root(adapter, "ActuatorInvalid");
    JSON_E(adapter, value);
    JSON_E(adapter, minimum);
    JSON_T(adapter, maximum);
}

void ActuatorPinMixin::serialize(JSON::Adapter & adapter)
{
#if WIRING
    ActuatorPin * obj   = static_cast<ActuatorPin *>(this);
    bool          state = obj -> isActive();

    JSON::Class root(adapter, "ActuatorPin");
    JSON_E(adapter, state);
    JSON_OE(adapter, pin);
    JSON_OT(adapter, invert);
#endif
}

void ValveControllerMixin::serialize(JSON::Adapter & adapter)
{
#if WIRING
    ValveController * obj = static_cast<ValveController *>(this);

    JSON::Class root(adapter, "ValveController");
    JSON_OE(adapter, pio);
    JSON_OT(adapter, sense);
#endif
}

void ActuatorOneWireMixin::serialize(JSON::Adapter & adapter)
{
#if WIRING
    ActuatorOneWire * obj = static_cast<ActuatorOneWire *>(this);

    JSON::Class root(adapter, "ActuatorOneWire");

    bool state = obj -> isActive();

    JSON_E(adapter, state);
    JSON_OE(adapter, pio);
    JSON_OT(adapter, invert);
#endif
}


void SetPointSimpleMixin::serialize(JSON::Adapter & adapter)
{
    SetPointSimple * obj = static_cast<SetPointSimple *>(this);

    JSON::Class root(adapter, "SetPointSimple");
    std::string name(getName());    // get name as std string for json_writer
    JSON_E(adapter, name);
    JSON_OT(adapter, value);
}

void SetPointConstantMixin::serialize(JSON::Adapter & adapter)
{
    SetPointConstant * obj   = static_cast<SetPointConstant *>(this);
    temp_t             value = obj -> value;    // create non-const copy for template resolution to work

    JSON::Class root(adapter, "SetPointConstant");
    JSON_T(adapter, value);
}

void SetPointMinMaxMixin::serialize(JSON::Adapter & adapter)
{
    SetPointMinMax * obj = static_cast<SetPointMinMax *>(this);

    JSON::Class root(adapter, "SetPointMinMax");
    JSON_OE(adapter, value);
    JSON_OE(adapter, min);
    JSON_OT(adapter, max);
}

