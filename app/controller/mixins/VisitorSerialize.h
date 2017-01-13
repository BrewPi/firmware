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

    void visit(ActuatorBool& thisRef) final;
    void visit(ActuatorInvalid& thisRef) final;
    void visit(ActuatorMutexDriver& thisRef) final;
    void visit(ActuatorMutexGroup& thisRef) final;
    void visit(ActuatorNop& thisRef) final;
    void visit(ActuatorOneWire& thisRef) final;
    void visit(ActuatorPwm& thisRef) final;
    void visit(ActuatorSetPoint& thisRef) final;
    void visit(ActuatorTimeLimited& thisRef) final;
    void visit(ActuatorValue& thisRef) final;
    void visit(Pid& thisRef) final;
    void visit(SetPointConstant& thisRef) final;
    void visit(SetPointMinMax& thisRef) final;
    void visit(SetPointSimple& thisRef) final;
    void visit(TempSensor& thisRef) final;
    void visit(TempSensorDisconnected& thisRef) final;
    void visit(TempSensorExternal& thisRef) final;
    void visit(TempSensorFallback& thisRef) final;
    void visit(TempSensorMock& thisRef) final;
    void visit(OneWireTempSensor& thisRef) final;
    void visit(ValveController& thisRef) final;
#if WIRING
    void visit(ActuatorPin& thisRef) final;
#endif

private:
    JSON::Adapter& adapter;
};
