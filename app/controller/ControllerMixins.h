#pragma once

#include "json_adapter.h"
#include "Nameable.h"

/* Using template method pattern to keep virtual functions private
 * Using protected non-virtual destructors to prevent destruction through base class
 */

class BaseMixin {};

class Serializable
{
public:
    Serializable() = default;
protected:
    ~Serializable() = default;
public:
    inline void serialize(JSON::Adapter& adapter){
        doSerialize(adapter);
    };
private:
    virtual void doSerialize(JSON::Adapter& adapter) = 0;
};

class TempSensorBasicMixin : public virtual Serializable
{
protected:
    ~TempSensorBasicMixin() = default;
};

class TempSensorMockMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~TempSensorMockMixin() = default;
};

class TempSensorMixin : public virtual Nameable, public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~TempSensorMixin() = default;
};

class PidMixin : public virtual Nameable, public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~PidMixin() = default;
};

class OneWireTempSensorMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~OneWireTempSensorMixin() = default;
};

class TempSensorDisconnectedMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~TempSensorDisconnectedMixin() = default;
};

class TempSensorExternalMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~TempSensorExternalMixin() = default;
};


class ActuatorMixin : public virtual Serializable {
protected:
    ~ActuatorMixin() = default;
};


class ActuatorTimeLimitedMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorTimeLimitedMixin() = default;
};

class ActuatorSetPointMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorSetPointMixin() = default;
};

class ActuatorPwmMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorPwmMixin() = default;
};

class ActuatorMutexGroupMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorMutexGroupMixin() = default;
};

class ActuatorMutexDriverMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorMutexDriverMixin() = default;
};

class ActuatorValueMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorValueMixin() = default;};

class ActuatorBoolMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorBoolMixin() = default;
};

class ActuatorNopMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorNopMixin() = default;
};

class ActuatorInvalidMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorInvalidMixin() = default;
};

class ActuatorDigitalMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorDigitalMixin() = default;
};

class SetPointMixin : public virtual Serializable
{
protected:
    ~SetPointMixin() = default;
};

class SetPointSimpleMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~SetPointSimpleMixin() = default;
};

class SetPointConstantMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~SetPointConstantMixin() = default;
};

class SetPointNamedMixin : public virtual Nameable, public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~SetPointNamedMixin() = default;
};


#if WIRING
class ActuatorPinMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorPinMixin() = default;
};

class ValveControllerMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~ValveControllerMixin() = default;
};

class ActuatorOneWireMixin : public virtual Serializable
{
private:
    void doSerialize(JSON::Adapter& adapter) final;
protected:
    ~ActuatorOneWireMixin() = default;
};
#endif
