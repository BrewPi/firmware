#pragma once

#include "Platform.h"
#include "VisitorBase.h"
#include "json_adapter.h"
#include "Nameable.h"
/*
 * Using protected non-virtual destructors to prevent destruction through base class
 */

class InterfaceMixin
{
protected:
    ~InterfaceMixin() = default;
public:
    void serialize(JSON::Adapter& adapter);
};

class ActuatorDigitalMixin {
protected:
    ~ActuatorDigitalMixin() = default;
};

class ActuatorAnalogMixin {
protected:
    ~ActuatorAnalogMixin() = default;
};

class ActuatorThresholdMixin {
protected:
    ~ActuatorThresholdMixin() = default;
};

class ProcessValueMixin {
protected:
    ~ProcessValueMixin() = default;
};

class TempSensorMixin {
protected:
    ~TempSensorMixin() = default;
};

class TempSensorMockMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~TempSensorMockMixin() = default;
};

class TempSensorFallbackMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~TempSensorFallbackMixin() = default;
};

class PidMixin: public Nameable {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~PidMixin() = default;
};

class OneWireTempSensorMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~OneWireTempSensorMixin() = default;
};

class TempSensorDisconnectedMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~TempSensorDisconnectedMixin() = default;
};

class TempSensorExternalMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~TempSensorExternalMixin() = default;
};

class ActuatorTimeLimitedMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorTimeLimitedMixin() = default;
};

class ActuatorOffsetMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorOffsetMixin() = default;
};

class ActuatorPwmMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorPwmMixin() = default;
};

class ActuatorMutexGroupMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorMutexGroupMixin() = default;
};

class ActuatorMutexDriverMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorMutexDriverMixin() = default;
};

class ActuatorValueMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorValueMixin() = default;
};

class ActuatorBoolMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorBoolMixin() = default;
};

class ActuatorNopMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorNopMixin() = default;
};

class ActuatorInvalidMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorInvalidMixin() = default;
};

class SetPointMixin {
protected:
    ~SetPointMixin() = default;
};

class SetPointSimpleMixin: public Nameable {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~SetPointSimpleMixin() = default;
};

class SetPointConstantMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~SetPointConstantMixin() = default;
};

class SetPointMinMaxMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~SetPointMinMaxMixin() = default;
};

class ActuatorPinMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorPinMixin() = default;
};

#if BREWPI_DS2408
class ValveControllerMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ValveControllerMixin() = default;
};
#endif

#if BREWPI_DS2413
class ActuatorOneWireMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorOneWireMixin() = default;
};
#endif

class TempSensorDelegateMixin: public Nameable {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~TempSensorDelegateMixin() = default;
};

class ActuatorDigitalDelegateMixin: public Nameable {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorDigitalDelegateMixin() = default;
};

class SetPointDelegateMixin: public Nameable {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~SetPointDelegateMixin() = default;
};

class ProcessValueDelegateMixin: public Nameable {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ProcessValueDelegateMixin() = default;
};

class SensorSetPointPairMixin: public Nameable {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~SensorSetPointPairMixin() = default;
};

