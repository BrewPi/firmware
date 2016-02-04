#include "Controller.h"

// These macros are equivalent to ESJ, except for that they add obj-> in front of the member variable name
// use JSON_E for local variables and JSON_OE for member variables of the obj pointer
// Serializable element with more elements to follow (alternative to JSON_E)
#define JSON_OE(json_adapter,class_member)  JSON::stream(json_adapter,_ASTRING(#class_member),obj->class_member,true)

// Serializable terminator, keep things simple for the JSON writer. No stream manipulations required (alternative to JSON_T)
#define JSON_OT(json_adapter,class_member)  JSON::stream(json_adapter,_ASTRING(#class_member),obj->class_member,false)


void Pid::serialize(JSON::Adapter & adapter)
{
    Pid * obj = static_cast<Pid *>(this);

    JSON::Class root(adapter, "Pid");
    std::string name(getName());    // get name as std string for json_writer
    JSON_E(adapter, name);
    JSON_OE(adapter, enabled);
    SetPoint* setPoint = (SetPoint*)obj->setPoint;
    JSON_E(adapter, setPoint);
    TempSensorBasic* inputSensor = TempSensorBasic::from(obj->inputSensor);
    JSON_E(adapter, inputSensor);
    JSON_OE(adapter, inputError);
    JSON_OE(adapter, Kp);
    JSON_OE(adapter, Ti);
    JSON_OE(adapter, Td);
    JSON_OE(adapter, p);
    JSON_OE(adapter, i);
    JSON_OE(adapter, d);
    JSON_OE(adapter, actuatorIsNegative);
    ::ActuatorRange* outputActuator = ::ActuatorRange::from(obj->outputActuator);
    JSON_T(adapter, outputActuator);
}

void ActuatorSetPoint::serialize(JSON::Adapter & adapter)
{
    ActuatorSetPoint * obj = static_cast<ActuatorSetPoint *>(this);

    JSON::Class root(adapter, "ActuatorSetPoint");
    SetPoint* targetSetPoint = SetPoint::from(obj->targetSetPoint);
    JSON_E(adapter, targetSetPoint);
    TempSensorBasic* targetSensor = TempSensorBasic::from(obj->targetSensor);
    JSON_E(adapter, targetSensor);
    SetPoint* referenceSetPoint = SetPoint::from(obj->referenceSetPoint);
    JSON_E(adapter, referenceSetPoint);

    temp_t output = obj -> getValue();

    JSON_E(adapter, output);

    temp_t achieved = obj -> readValue();
    JSON_E(adapter, achieved);
    JSON_OE(adapter, minimum);
    JSON_OT(adapter, maximum);
}


void TempSensor::serialize(JSON::Adapter & adapter)
{
    TempSensor * obj = static_cast<TempSensor *>(this);

    JSON::Class root(adapter, "TempSensor");
    std::string name(getName());    // get name as std string for json_writer
    JSON_E(adapter, name);
    ::TempSensorBasic* sensor = ::TempSensorBasic::from(&obj->sensor());
    JSON_T(adapter, sensor);
}

void TempSensorMock::serialize(JSON::Adapter & adapter)
{
    TempSensorMock * obj = static_cast<TempSensorMock *>(this);

    JSON::Class root(adapter, "TempSensorMock");
    JSON_OE(adapter, value);
    JSON_OT(adapter, connected);
}

void TempSensorDisconnected::serialize(JSON::Adapter & adapter)
{
    TempSensorDisconnected * obj       = static_cast<TempSensorDisconnected *>(this);
    temp_t                   value     = obj -> sensor().read();
    bool                     connected = false;

    JSON::Class root(adapter, "TempSensorDisconnected");
    JSON_E(adapter, value);
    JSON_T(adapter, connected);
}

void OneWireTempSensor::serialize(JSON::Adapter & adapter)
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

void TempSensorExternal::serialize(JSON::Adapter & adapter)
{
    TempSensorExternal * obj = static_cast<TempSensorExternal *>(this);

    JSON::Class root(adapter, "TempSensorExternal");
    JSON_OE(adapter, value);
    JSON_OT(adapter, connected);
}

void ActuatorTimeLimited::serialize(JSON::Adapter & adapter)
{
    ActuatorTimeLimited * obj = static_cast<ActuatorTimeLimited *>(this);

    JSON::Class root(adapter, "ActuatorTimeLimited");
    JSON_OE(adapter, minOnTime);
    JSON_OE(adapter, minOffTime);
    JSON_OE(adapter, maxOnTime);
    JSON_OE(adapter, state);
    ::ActuatorDigital* target = ::ActuatorDigital::from(obj->target);
    JSON_T(adapter, target);
}

void ActuatorPwm::serialize(JSON::Adapter & adapter)
{
    ActuatorPwm * obj = static_cast<ActuatorPwm *>(this);

    JSON::Class root(adapter, "ActuatorPwm");
    JSON_OE(adapter, value);

    ticks_seconds_t period = obj -> getPeriod();    // don't use member directly, but value in seconds

    JSON_E(adapter, period);
    JSON_OE(adapter, minVal);
    JSON_OE(adapter, maxVal);
    ActuatorDigital* target = ActuatorDigital::from(obj->target);
    JSON_T(adapter, target);
}

void ActuatorMutexGroup::serialize(JSON::Adapter & adapter)
{
    ActuatorMutexGroup * obj = static_cast<ActuatorMutexGroup *>(this);

    JSON::Class root(adapter, "ActuatorMutexGroup");
    JSON_OE(adapter, deadTime);
    JSON_OT(adapter, lastActiveTime);
}

void ActuatorMutexDriver::serialize(JSON::Adapter & adapter)
{
    ActuatorMutexDriver * obj = static_cast<ActuatorMutexDriver *>(this);

    JSON::Class root(adapter, "ActuatorMutexDriver");
    ::ActuatorMutexGroup* mutexGroup = ::ActuatorMutexGroup::from(obj->mutexGroup);
    JSON_E(adapter, mutexGroup);
    ::ActuatorDigital* target = ::ActuatorDigital::from(obj->target);
    JSON_T(adapter, target);
}

void ActuatorValue::serialize(JSON::Adapter & adapter)
{
    ActuatorValue * obj = static_cast<ActuatorValue *>(this);

    JSON::Class root(adapter, "ActuatorValue");
    JSON_OE(adapter, value);
    JSON_OE(adapter, minimum);
    JSON_OT(adapter, maximum);
}

void ActuatorBool::serialize(JSON::Adapter & adapter)
{
    ActuatorBool * obj = static_cast<ActuatorBool *>(this);

    JSON::Class root(adapter, "ActuatorBool");
    JSON_OT(adapter, state);
}

void ActuatorNop::serialize(JSON::Adapter & adapter)
{
    ActuatorNop * obj = static_cast<ActuatorNop *>(this);

    JSON::Class root(adapter, "ActuatorNop");

    bool state = obj -> actuator().isActive();

    JSON_T(adapter, state);
}

void ActuatorInvalid::serialize(JSON::Adapter & adapter)
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

#if WIRING
void ActuatorPin::serialize(JSON::Adapter & adapter)
{
    ActuatorPin * obj   = static_cast<ActuatorPin *>(this);
    bool          state = obj -> actuator().isActive();

    JSON::Class root(adapter, "ActuatorPin");
    JSON_E(adapter, state);
    JSON_OE(adapter, pin);
    JSON_OT(adapter, invert);
}

void ValveController::serialize(JSON::Adapter & adapter)
{
    ValveController * obj = static_cast<ValveController *>(this);

    JSON::Class root(adapter, "ValveController");
    JSON_OE(adapter, pio);
    JSON_OT(adapter, sense);
}

void ActuatorOneWire::serialize(JSON::Adapter & adapter)
{
    ActuatorOneWire * obj = static_cast<ActuatorOneWire *>(this);

    JSON::Class root(adapter, "ActuatorOneWire");

    bool state = obj -> actuator().isActive();

    JSON_E(adapter, state);
    JSON_OE(adapter, pio);
    JSON_OT(adapter, invert);
}
#endif

void SetPointSimple::serialize(JSON::Adapter & adapter)
{
    SetPointSimple * obj = static_cast<SetPointSimple *>(this);

    JSON::Class root(adapter, "SetPointSimple");
    JSON_OT(adapter, value);
}

void SetPointConstant::serialize(JSON::Adapter & adapter)
{
    SetPointConstant * obj   = static_cast<SetPointConstant *>(this);
    temp_t             value = obj -> value;    // create non-const copy for template resolution to work

    JSON::Class root(adapter, "SetPointConstant");
    JSON_T(adapter, value);
}

void SetPointNamed::serialize(JSON::Adapter & adapter)
{
    SetPointNamed * obj = static_cast<SetPointNamed *>(this);

    JSON::Class root(adapter, "SetPointNamed");
    std::string name(getName());    // get name as std string for json_writer
    JSON_E(adapter, name);
    ::SetPoint* setPoint = ::SetPoint::from(obj->setPoint);
    JSON_T(adapter, setPoint);
}

void SetPointMinMax::serialize(JSON::Adapter & adapter)
{
    SetPointMinMax * obj = static_cast<SetPointMinMax *>(this);

    JSON::Class root(adapter, "SetPointMinMax");
    JSON_OE(adapter, value);
    JSON_OE(adapter, min);
    JSON_OT(adapter, max);
}


