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
#include <stdint.h>
#include "Platform.h"

// Visitor should be implemented for all classes that can be accessed through an interface,
// but not for the interfaces themselves.
class ActuatorBool;
class ActuatorInvalid;
class ActuatorMutexDriver;
class ActuatorMutexGroup;
class ActuatorNop;
class ActuatorOneWire;
class ActuatorPin;
class ActuatorPwm;
class ActuatorSetPoint;
class ActuatorTimeLimited;
class ActuatorValue;
class OneWireTempSensor;
class Pid;
class SetPointConstant;
class SetPointMinMax;
class SetPointSimple;
class TempSensor;
class TempSensorDisconnected;
class TempSensorExternal;
class TempSensorFallback;
class TempSensorMock;
class ValveController;

class VisitorBase {
protected:
    virtual ~VisitorBase() = default;

public:
	virtual void visit(ActuatorBool& thisRef) = 0;
	virtual void visit(ActuatorInvalid& thisRef) = 0;
	virtual void visit(ActuatorMutexDriver& thisRef) = 0;
	virtual void visit(ActuatorMutexGroup& thisRef) = 0;
	virtual void visit(ActuatorNop& thisRef) = 0;
	virtual void visit(ActuatorOneWire& thisRef) = 0;
	virtual void visit(ActuatorPwm& thisRef) = 0;
	virtual void visit(ActuatorSetPoint& thisRef) = 0;
	virtual void visit(ActuatorTimeLimited& thisRef) = 0;
	virtual void visit(ActuatorValue& thisRef) = 0;
	virtual void visit(Pid& thisRef) = 0;
	virtual void visit(SetPointConstant& thisRef) = 0;
	virtual void visit(SetPointMinMax& thisRef) = 0;
	virtual void visit(SetPointSimple& thisRef) = 0;
	virtual void visit(TempSensor& thisRef) = 0;
	virtual void visit(TempSensorDisconnected& thisRef) = 0;
	virtual void visit(TempSensorExternal& thisRef) = 0;
	virtual void visit(TempSensorFallback& thisRef) = 0;
	virtual void visit(TempSensorMock& thisRef) = 0;
	virtual void visit(OneWireTempSensor& thisRef) = 0;
	virtual void visit(ValveController& thisRef) = 0;
#if WIRING
	virtual void visit(ActuatorPin& thisRef) = 0;
#endif
};
