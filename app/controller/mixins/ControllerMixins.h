#pragma once

#include "json_adapter.h"
#include "Nameable.h"
#include "ActuatorInstallHelper.h"

/*
 * Using protected non-virtual destructors to prevent destruction through base class
 */

class Serializable
{
public:
    Serializable() = default;
protected:
    ~Serializable() = default;
public:
    void serialize(JSON::Adapter& adapter);
};

class VirtualSerializable
{
public:
    VirtualSerializable() = default;
protected:
    ~VirtualSerializable() = default;
public:
    virtual void serialize(JSON::Adapter& adapter) = 0;
};

class TempSensorBasicMixin :
        public virtual VirtualSerializable
{
protected:
    ~TempSensorBasicMixin() = default;
};

class TempSensorMockMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~TempSensorMockMixin() = default;
};

class TempSensorMixin :
        public Nameable,
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~TempSensorMixin() = default;
};


class TempSensorFallbackMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~TempSensorFallbackMixin() = default;
};

class PidMixin :
        public Nameable,
        public Serializable
{
public:
    void serialize(JSON::Adapter& adapter);
protected:
    ~PidMixin() = default;
};

class OneWireTempSensorMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~OneWireTempSensorMixin() = default;
};

class TempSensorDisconnectedMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~TempSensorDisconnectedMixin() = default;
};

class TempSensorExternalMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~TempSensorExternalMixin() = default;
};


class ActuatorMixin :
        public virtual VirtualSerializable,
        public virtual ActuatorInstallHelper
{
protected:
    ~ActuatorMixin() = default;
};

class ActuatorForwarderMixin :
        public ActuatorInstallHelperForwarder
{
protected:
    ~ActuatorForwarderMixin() = default;
};


class ActuatorTimeLimitedMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorTimeLimitedMixin() = default;
};

class ActuatorSetPointMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorSetPointMixin() = default;
};

class ActuatorPwmMixin :
        public virtual  VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorPwmMixin() = default;
};

class ActuatorMutexGroupMixin :
        public Serializable
{
public:
    void serialize(JSON::Adapter& adapter);
protected:
    ~ActuatorMutexGroupMixin() = default;
};

class ActuatorMutexDriverMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorMutexDriverMixin() = default;
};

class ActuatorValueMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorValueMixin() = default;};

class ActuatorBoolMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorBoolMixin() = default;
};

class ActuatorNopMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorNopMixin() = default;
};

class ActuatorInvalidMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorInvalidMixin() = default;
};

class ActuatorDigitalMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorDigitalMixin() = default;
};

class SetPointMixin :
        public virtual VirtualSerializable
{
protected:
    ~SetPointMixin() = default;
};

class SetPointSimpleMixin :
        public Nameable,
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~SetPointSimpleMixin() = default;
};

class SetPointConstantMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~SetPointConstantMixin() = default;
};

class SetPointMinMaxMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~SetPointMinMaxMixin() = default;
};

class ActuatorPinMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorPinMixin() = default;
};

class ValveControllerMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~ValveControllerMixin() = default;
};

class ActuatorOneWireMixin :
        public virtual VirtualSerializable
{
public:
    void serialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorOneWireMixin() = default;
};

