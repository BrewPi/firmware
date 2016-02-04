/*
 * Copyright 2012-2015 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "ActuatorInterfaces.h"
#include "ActuatorMocks.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorMutexGroup.h"
#include "ActuatorPwm.h"
#include "ActuatorSetPoint.h"
#include "ActuatorTimeLimited.h"
#include "ControlLib.h"
#include "json_adapter.h"
#include "OneWireTempSensor.h"
#include "SetPoint.h"
#include "Pid.h"
#include "TempSensor.h"
#include "TempSensorBasic.h"
#include "TempSensorDisconnected.h"
#include "TempSensorExternal.h"
#include "TempSensorMock.h"

#if WIRING
#include "ActuatorOneWire.h"
#include "ActuatorPin.h"
#include "ValveController.h"
#endif

#include "Nameable.h"

template <typename App, typename Lib>
struct Fetcher
{
public:
    static App* from(Lib* lib) {
        App* result = nullptr;
        if (lib) {
            result = (App*)lib->get_handle();
        }
        return result;
    }
};

template <typename App, typename Lib>
class Wrapper
{
protected:
    Wrapper() {
        App* app = reinterpret_cast<App*>(this);
        Lib* lib = *app;
        lib->set_handle(app);
    }
};

class Actuator //: public Wrapper<Actuator, CONTROL_LIB_NAMESPACE::Actuator>
{
public:
    virtual void serialize(JSON::Adapter & adapter)=0;
    virtual ~Actuator() {}

    static ::Actuator* from(CONTROL_LIB_NAMESPACE::Actuator* lib) {
        return Fetcher<Actuator, CONTROL_LIB_NAMESPACE::Actuator>::from(lib);
    }

    virtual CONTROL_LIB_NAMESPACE::Actuator& actuator() const=0;
};

class ActuatorDigital : public Wrapper<ActuatorDigital, CONTROL_LIB_NAMESPACE::ActuatorDigital>, public Actuator
{
public:
    ActuatorDigital() {}
    virtual ~ActuatorDigital() {};

    virtual void serialize(JSON::Adapter & adapter)=0;
    virtual CONTROL_LIB_NAMESPACE::ActuatorDigital& actuator() const=0;

    operator CONTROL_LIB_NAMESPACE::ActuatorDigital*() {
        return &this->actuator();
    }

    static ::ActuatorDigital* from(CONTROL_LIB_NAMESPACE::ActuatorDigital* lib) {
        return Fetcher<ActuatorDigital, CONTROL_LIB_NAMESPACE::ActuatorDigital>::from(lib);
    }

    bool isActive() const { return actuator().isActive(); }
    void setActive(bool active) { actuator().setActive(active); }

};


class ActuatorBool : public CONTROL_LIB_NAMESPACE::ActuatorBool, public ActuatorDigital
{
public:
    void serialize(JSON::Adapter & adapter) override;

    virtual CONTROL_LIB_NAMESPACE::ActuatorDigital& actuator() const override {
        return *const_cast<ActuatorBool*>(this);
    }

};


class ActuatorRange : public Actuator, public Wrapper<ActuatorRange, CONTROL_LIB_NAMESPACE::ActuatorRange>
{
public:
    virtual ~ActuatorRange() {}
    virtual CONTROL_LIB_NAMESPACE::ActuatorRange& actuator() const=0;

    operator CONTROL_LIB_NAMESPACE::ActuatorRange*() {
        return &actuator();
    }

    static ::ActuatorRange* from(CONTROL_LIB_NAMESPACE::ActuatorRange* lib) {
        return Fetcher<ActuatorRange, CONTROL_LIB_NAMESPACE::ActuatorRange>::from(lib);
    }

};


class ActuatorInvalid : public CONTROL_LIB_NAMESPACE::ActuatorInvalid, public Actuator
{
public:
    void serialize(JSON::Adapter & adapter);

    virtual CONTROL_LIB_NAMESPACE::Actuator& actuator() const {
        return *const_cast<ActuatorInvalid*>(this);
    }
};


class ActuatorMutexGroup : public CONTROL_LIB_NAMESPACE::ActuatorMutexGroup,
    public Wrapper<ActuatorMutexGroup, CONTROL_LIB_NAMESPACE::ActuatorMutexGroup>,
    public Fetcher<ActuatorMutexGroup, CONTROL_LIB_NAMESPACE::ActuatorMutexGroup>
{
public:
    void serialize(JSON::Adapter & adapter);

    operator CONTROL_LIB_NAMESPACE::ActuatorMutexGroup*() {
        return this;
    }
};

class ActuatorMutexDriver : public CONTROL_LIB_NAMESPACE::ActuatorMutexDriver, public ::ActuatorDigital
{
    using base = CONTROL_LIB_NAMESPACE::ActuatorMutexDriver;
public:
    ActuatorMutexDriver(CONTROL_LIB_NAMESPACE::ActuatorDigital * target) : base(target) {}
    ActuatorMutexDriver(CONTROL_LIB_NAMESPACE::ActuatorDigital * target, ActuatorMutexGroup * m) : base(target, m) {}

    void serialize(JSON::Adapter & adapter) override;

    virtual CONTROL_LIB_NAMESPACE::ActuatorDigital& actuator() const override {
        return *const_cast<ActuatorMutexDriver*>(this);
    }

};



class ActuatorNop : public CONTROL_LIB_NAMESPACE::ActuatorNop, public ActuatorDigital
{
public:
    void serialize(JSON::Adapter & adapter) override;

    virtual CONTROL_LIB_NAMESPACE::ActuatorDigital& actuator() const override {
        return *const_cast<ActuatorNop*>(this);
    }

};


#if WIRING
class ActuatorOneWire : public CONTROL_LIB_NAMESPACE::ActuatorOneWire, public ActuatorDigital
{
    using base = CONTROL_LIB_NAMESPACE::ActuatorOneWire;
public:
    ActuatorOneWire(::OneWire *     bus,
                            DeviceAddress address,
                            pio_t         pio,
                            bool          invert = true) : base(bus, address, pio, invert) {}
    void serialize(JSON::Adapter & adapter) override;

    virtual CONTROL_LIB_NAMESPACE::ActuatorDigital& actuator() const override {
        return *const_cast<ActuatorOneWire*>(this);
    }

};


class ActuatorPin : public CONTROL_LIB_NAMESPACE::ActuatorPin, public ActuatorDigital
{
    using base = CONTROL_LIB_NAMESPACE::ActuatorPin;
public:

    ActuatorPin(uint8_t pin,bool    invert) : base(pin, invert) {}
    void serialize(JSON::Adapter & adapter) override;

    virtual CONTROL_LIB_NAMESPACE::ActuatorDigital& actuator() const override {
        return *const_cast<ActuatorPin*>(this);
    }

};
#endif


class ActuatorPwm : public CONTROL_LIB_NAMESPACE::ActuatorPwm, public ActuatorRange
{
    using base = CONTROL_LIB_NAMESPACE::ActuatorPwm;
public:
    ActuatorPwm(CONTROL_LIB_NAMESPACE::ActuatorDigital * _target, uint16_t _period) : base(_target, _period) {}

    void serialize(JSON::Adapter & adapter);

    CONTROL_LIB_NAMESPACE::ActuatorRange& actuator() const override {
        return *const_cast<ActuatorPwm*>(this);
    }

};


class ActuatorSetPoint : public CONTROL_LIB_NAMESPACE::ActuatorSetPoint, public ActuatorRange
{
    using base = CONTROL_LIB_NAMESPACE::ActuatorSetPoint;
public:
    ActuatorSetPoint(CONTROL_LIB_NAMESPACE::SetPoint * targSetPoint = CONTROL_LIB_NAMESPACE::defaultSetPoint(), // set point to manipulate
                        CONTROL_LIB_NAMESPACE::TempSensorBasic * targSensor = CONTROL_LIB_NAMESPACE::defaultTempSensorBasic(), // sensor to go with target setpoint
                        CONTROL_LIB_NAMESPACE::SetPoint * refSetPoint = CONTROL_LIB_NAMESPACE::defaultSetPoint(), //set point to offset from
                         temp_t mini = temp_t::min(), // minimum actuator value (targ - ref)
                         temp_t maxi = temp_t::max())  // maximum actuator value
            :   base(targSetPoint, targSensor, refSetPoint, mini, maxi) {}

    void serialize(JSON::Adapter& adapter);

    CONTROL_LIB_NAMESPACE::ActuatorRange& actuator() const override {
        return *const_cast<ActuatorSetPoint*>(this);
    }

};


class ActuatorTimeLimited : public CONTROL_LIB_NAMESPACE::ActuatorTimeLimited, public ::ActuatorDigital
{
    using base = CONTROL_LIB_NAMESPACE::ActuatorTimeLimited;
public:
    ActuatorTimeLimited(CONTROL_LIB_NAMESPACE::ActuatorDigital * _target,
            ticks_seconds_t   _minOnTime = 120,
            ticks_seconds_t   _minOffTime = 180,
            ticks_seconds_t   _maxOnTime = UINT16_MAX) : base(_target, _minOnTime, _minOffTime, _maxOnTime) {}

    virtual void serialize(JSON::Adapter & adapter) override;

    virtual CONTROL_LIB_NAMESPACE::ActuatorDigital& actuator() const override {
        return *const_cast<ActuatorTimeLimited*>(this);
    }

};


class ActuatorValue : public CONTROL_LIB_NAMESPACE::ActuatorValue
{
public:
    void serialize(JSON::Adapter & adapter);
};


class OneWireTempSensor : public CONTROL_LIB_NAMESPACE::OneWireTempSensor
{
    using base = CONTROL_LIB_NAMESPACE::OneWireTempSensor;
public:
    OneWireTempSensor(OneWire* bus, DeviceAddress address, temp_t calibrationOffset) : base(bus, address, calibrationOffset) {}

    void serialize(JSON::Adapter & adapter);
};

class Pid;


class SetPoint : public Wrapper<SetPoint, CONTROL_LIB_NAMESPACE::SetPoint>, public Fetcher<SetPoint, CONTROL_LIB_NAMESPACE::SetPoint>
{

public:
    virtual void serialize(JSON::Adapter & adapter)=0;
    virtual ~SetPoint() = default;

    // avoid the need for a virtual base.
    virtual CONTROL_LIB_NAMESPACE::SetPoint& setpoint()=0;

    operator CONTROL_LIB_NAMESPACE::SetPoint*() {
        return &setpoint();
    }
};


class SetPointConstant : public CONTROL_LIB_NAMESPACE::SetPointConstant, public SetPoint
{
    using base = CONTROL_LIB_NAMESPACE::SetPointConstant;
public:
    SetPointConstant(const temp_t val) : base(val) {}
    void serialize(JSON::Adapter & adapter) override;

    virtual CONTROL_LIB_NAMESPACE::SetPoint& setpoint() override {
        return *this;
    }
};


class SetPointMinMax : public CONTROL_LIB_NAMESPACE::SetPointMinMax, public SetPoint
{
public:
    void serialize(JSON::Adapter & adapter) override;

    virtual CONTROL_LIB_NAMESPACE::SetPoint& setpoint() override {
        return *this;
    }

};


class SetPointNamed : public CONTROL_LIB_NAMESPACE::SetPointNamed, public SetPoint, public Nameable
{
    using base = CONTROL_LIB_NAMESPACE::SetPointNamed;
public:
    SetPointNamed(CONTROL_LIB_NAMESPACE::SetPoint * sp) : base(sp) {}

    void serialize(JSON::Adapter & adapter) override;

    virtual CONTROL_LIB_NAMESPACE::SetPoint& setpoint() override {
        return *this;
    }

};


class SetPointSimple : public CONTROL_LIB_NAMESPACE::SetPointSimple, public SetPoint
{
    using base = CONTROL_LIB_NAMESPACE::SetPointSimple;
public:
    SetPointSimple(temp_t val = temp_t::disabled()) : base(val) {}
    void serialize(JSON::Adapter & adapter) override;

    virtual CONTROL_LIB_NAMESPACE::SetPoint& setpoint() override {
        return *this;
    }
};


class TempSensorBasic : public Wrapper<::TempSensorBasic, CONTROL_LIB_NAMESPACE::TempSensorBasic>,
    public Fetcher<TempSensorBasic, CONTROL_LIB_NAMESPACE::TempSensorBasic>
{
public:
    virtual void serialize(JSON::Adapter & adapter)=0;
    virtual ~TempSensorBasic() = default;
    virtual CONTROL_LIB_NAMESPACE::TempSensorBasic& sensor() const=0;

    operator CONTROL_LIB_NAMESPACE::TempSensorBasic*() { return &sensor(); }
    operator const CONTROL_LIB_NAMESPACE::TempSensorBasic*() const { return &sensor(); }
};


class TempSensor : public CONTROL_LIB_NAMESPACE::TempSensor, public Nameable, public ::TempSensorBasic
{
    using base = CONTROL_LIB_NAMESPACE::TempSensor;
public:

    TempSensor() = default;
    TempSensor(::TempSensorBasic * s) : base(*s) {}

    TempSensor(CONTROL_LIB_NAMESPACE::TempSensorBasic * s) : base(s) {}

    virtual CONTROL_LIB_NAMESPACE::TempSensorBasic& sensor() const override {
        return *this->getSensor();
    }

    void serialize(JSON::Adapter & adapter) override;
};

class TempSensorDisconnected : public CONTROL_LIB_NAMESPACE::TempSensorDisconnected, public ::TempSensorBasic
{
public:
    void serialize(JSON::Adapter & adapter) override;

    virtual CONTROL_LIB_NAMESPACE::TempSensorBasic& sensor() const override {
        return *(const_cast<TempSensorDisconnected*>(this));
    }

};


class TempSensorExternal : public CONTROL_LIB_NAMESPACE::TempSensorExternal, public ::TempSensorBasic
{
public:
    virtual void serialize(JSON::Adapter & adapter) override;

    virtual CONTROL_LIB_NAMESPACE::TempSensorBasic& sensor() const override {
        return *(const_cast<TempSensorExternal*>(this));
    }

};


class TempSensorMock : public CONTROL_LIB_NAMESPACE::TempSensorMock, public ::TempSensorBasic
{
    using base = CONTROL_LIB_NAMESPACE::TempSensorMock;
public:
    TempSensorMock(temp_t initial) : base(initial) {}
    virtual void serialize(JSON::Adapter & adapter) override;

    virtual CONTROL_LIB_NAMESPACE::TempSensorBasic& sensor() const override {
        return *(const_cast<TempSensorMock*>(this));
    }

};


#if WIRING
class ValveController : public CONTROL_LIB_NAMESPACE::ValveController
{
    using base = CONTROL_LIB_NAMESPACE::ValveController;
public:
    ValveController(::OneWire *     bus,
                        DeviceAddress address,
                        pio_t         pio_) : base(bus, address, pio_) {}

    void serialize(JSON::Adapter & adapter);
};
#endif

class Pid : public CONTROL_LIB_NAMESPACE::Pid, public Nameable
{
    using base = CONTROL_LIB_NAMESPACE::Pid;
public:
    Pid(CONTROL_LIB_NAMESPACE::TempSensorBasic * input, CONTROL_LIB_NAMESPACE::ActuatorRange * output, CONTROL_LIB_NAMESPACE::SetPoint * setPoint) : base(input, output, setPoint) {}
    Pid() {}
    void serialize(JSON::Adapter& adapter);

    friend class TempControl;

};

