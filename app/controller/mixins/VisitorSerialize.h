/*
 * Copyright 2016 BrewPi/Elco Jacobs.
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
#include "VisitorBase.h"
#include "json_adapter.h"
#include "Platform.h"


class VisitorSerialize :
    public VisitorBase
{
public:
    VisitorSerialize(JSON::Adapter& a) : adapter(a){}
    ~VisitorSerialize() = default;

    virtual void visit(ActuatorBool& thisRef) override final;
    virtual void visit(ActuatorInvalid& thisRef) override final;
    virtual void visit(ActuatorMutexDriver& thisRef) override final;
    virtual void visit(ActuatorMutexGroup& thisRef) override final;
    virtual void visit(ActuatorNop& thisRef) override final;
#if BREWPI_DS2413
    virtual void visit(ActuatorOneWire& thisRef) override final;
#endif
    virtual void visit(ActuatorPwm& thisRef) override final;
    virtual void visit(ActuatorOffset& thisRef) override final;
    virtual void visit(ActuatorTimeLimited& thisRef) override final;
    virtual void visit(ActuatorValue& thisRef) override final;
    virtual void visit(Pid& thisRef) override final;
    virtual void visit(SetpointConstant& thisRef) override final;
    virtual void visit(SetpointMinMax& thisRef) override final;
    virtual void visit(SetpointSimple& thisRef) override final;
    virtual void visit(TempSensorDisconnected& thisRef) override final;
    virtual void visit(TempSensorExternal& thisRef) override final;
    virtual void visit(TempSensorFallback& thisRef) override final;
    virtual void visit(TempSensorMock& thisRef) override final;
    virtual void visit(OneWireTempSensor& thisRef) override final;
#if BREWPI_DS2408
    virtual void visit(ValveController& thisRef) override final;
#endif
    virtual void visit(TempSensorDelegate& thisRef) override final;
    virtual void visit(ActuatorDigitalDelegate& thisRef) override final;
    virtual void visit(ProcessValueDelegate& thisRef) override final;
    virtual void visit(SetpointDelegate& thisRef) override final;
    virtual void visit(SetpointSensorPair& thisRef) override final;
#if WIRING
    virtual void visit(ActuatorPin& thisRef) override final;
#endif

private:
    JSON::Adapter& adapter;
};
