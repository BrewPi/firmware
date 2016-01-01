#pragma once

#include "json_adapter.h"
#include "Nameable.h"

class BaseMixin {};

class Serializable
{
public:
    virtual void serialize(JSON::Adapter& adapter) = 0;
};

class TempSensorBasicMixin : public virtual Serializable
{
public:
};

class TempSensorMixin : public virtual Nameable, public virtual Serializable
{
public:
    virtual void serialize(JSON::Adapter& adapter) override;

};

class PidMixin : public Nameable
{
public:
    void serialize(JSON::Adapter& adapter);

};



