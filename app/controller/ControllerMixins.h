#pragma once

#include "json_adapter.h"
#include "Nameable.h"

class BaseMixin {};

struct Serializable
{
    virtual void serialize(JSON::Adapter& adapter) = 0;
};

struct TempSensorBasicMixin : public virtual Serializable
{

};

struct TempSensorMixin : public virtual Nameable, public virtual Serializable
{
    virtual void serialize(JSON::Adapter& adapter) override;

};

struct PidMixin : public Nameable
{
    void serialize(JSON::Adapter& adapter);

};



