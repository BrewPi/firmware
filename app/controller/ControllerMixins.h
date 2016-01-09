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
    Serializable(){}
    void serialize(JSON::Adapter& adapter){
        doSerialize(adapter);
    };
private:
    virtual void doSerialize(JSON::Adapter& adapter) = 0;
protected:
    ~Serializable(){}
};

class TempSensorBasicMixin : public virtual Serializable
{
protected:
    ~TempSensorBasicMixin(){}
};

class TempSensorMockMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~TempSensorMockMixin(){}
};

class TempSensorMixin : public virtual Nameable, public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~TempSensorMixin(){}
};

class PidMixin : public virtual Nameable, public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~PidMixin(){}
};

class OneWireTempSensorMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~OneWireTempSensorMixin(){}
};

class TempSensorDisconnectedMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~TempSensorDisconnectedMixin(){}
};

class TempSensorExternalMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~TempSensorExternalMixin(){}
};

class ValveControllerMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~ValveControllerMixin(){}
};

class ActuatorMixin : public virtual Serializable {
protected:
    ~ActuatorMixin(){}
};


class ActuatorTimeLimitedMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~ActuatorTimeLimitedMixin(){}
};

class ActuatorSetPointMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~ActuatorSetPointMixin(){}
};

class ActuatorPwmMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~ActuatorPwmMixin(){}
};

class ActuatorOneWireMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~ActuatorOneWireMixin(){}
};

class ActuatorMutexGroupMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~ActuatorMutexGroupMixin(){}
};

class ActuatorMutexDriverMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~ActuatorMutexDriverMixin(){}
};

class ActuatorValueMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~ActuatorValueMixin(){}};

class ActuatorBoolMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~ActuatorBoolMixin(){}
};

class ActuatorNopMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~ActuatorNopMixin(){}
};

class ActuatorInvalidMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~ActuatorInvalidMixin(){}
};

class ActuatorDigitalMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~ActuatorDigitalMixin(){}
};

#if WIRING
class ActuatorPinMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~ActuatorPinMixin(){}
};
#endif

class SetPointMixin : public virtual Serializable
{
protected:
    ~SetPointMixin(){}
};

class SetPointSimpleMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~SetPointSimpleMixin(){}
};

class SetPointConstantMixin : public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~SetPointConstantMixin(){}
};

class SetPointNamedMixin : public virtual Nameable, public virtual Serializable
{
private:
    virtual void doSerialize(JSON::Adapter& adapter) override;
protected:
    ~SetPointNamedMixin(){}
};





