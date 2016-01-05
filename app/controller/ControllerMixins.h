#pragma once

#include "json_adapter.h"
#include "Nameable.h"

class BaseMixin {};

class Serializable
{
public:
    Serializable(){}
    virtual ~Serializable(){}
    virtual void serialize(JSON::Adapter& adapter) = 0;
};

class TempSensorBasicMixin : public virtual Serializable
{
};

class TempSensorMockMixin : public virtual Serializable
{
public:
    virtual void serialize(JSON::Adapter& adapter) override;
};

class TempSensorMixin : public virtual Nameable, public virtual Serializable
{
public:
    virtual void serialize(JSON::Adapter& adapter) override;
};

class PidMixin : public virtual Nameable, public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};

class OneWireTempSensorMixin : public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};

class TempSensorDisconnectedMixin : public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};

class TempSensorExternalMixin : public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};

class ValveControllerMixin : public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};

class ActuatorMixin : public virtual Serializable {};


class ActuatorTimeLimitedMixin : public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};

class ActuatorSetPointMixin : public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};

class ActuatorPwmMixin : public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};

class ActuatorOneWireMixin : public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};

class ActuatorMutexGroupMixin : public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};

class ActuatorMutexDriverMixin : public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};

class ActuatorValueMixin : public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};

class ActuatorBoolMixin : public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};

class ActuatorNopMixin : public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};

class ActuatorInvalidMixin : public virtual Serializable
{
public:
    void serialize(JSON::Adapter& adapter) override;
};









