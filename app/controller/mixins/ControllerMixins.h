#pragma once

#include "VisitorBase.h"
#include "json_adapter.h"
#include "Nameable.h"
#include "ActuatorInstallHelper.h"
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

class ActuatorInterfaceMixin :
    public virtual ActuatorInstallHelper
{
protected:
    ~ActuatorInterfaceMixin() = default;
};



class ActuatorDigitalInterfaceMixin {
protected:
    ~ActuatorDigitalInterfaceMixin() = default;
};


class ActuatorRangeInterfaceMixin {
protected:
    ~ActuatorRangeInterfaceMixin() = default;
};

class ActuatorThresholdInterfaceMixin {
protected:
    ~ActuatorThresholdInterfaceMixin() = default;
};

class TempSensorInterfaceMixin {
protected:
    ~TempSensorInterfaceMixin() = default;
};

class TempSensorMockMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~TempSensorMockMixin() = default;
};

class TempSensorDelegateMixin: public Nameable {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~TempSensorDelegateMixin() = default;
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


class ActuatorForwarderMixin: public ActuatorInstallHelperForwarder {
protected:
    ~ActuatorForwarderMixin() = default;
};

class ActuatorTimeLimitedMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorTimeLimitedMixin() = default;
};

class ActuatorSetPointMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorSetPointMixin() = default;
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

class SetPointInterfaceMixin {
protected:
    ~SetPointInterfaceMixin() = default;
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

class ValveControllerMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ValveControllerMixin() = default;
};

class ActuatorOneWireMixin {
public:
    void serializeImpl(JSON::Adapter& adapter);
protected:
    ~ActuatorOneWireMixin() = default;
};



