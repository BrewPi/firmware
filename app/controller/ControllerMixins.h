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

