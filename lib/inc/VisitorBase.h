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
class ActuatorDigitalReference;
class ActuatorInvalid;
class ActuatorMutexDriver;
class ActuatorMutexGroup;
class ActuatorNop;
class ActuatorOneWire;
class ActuatorPin;
class ActuatorPwm;
class ActuatorRangeReference;
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
	virtual void visit(ActuatorBool& thisRef){};
	virtual void visit(ActuatorDigitalReference& thisRef){};
	virtual void visit(ActuatorInvalid& thisRef){};
	virtual void visit(ActuatorMutexDriver& thisRef){};
	virtual void visit(ActuatorMutexGroup& thisRef){};
	virtual void visit(ActuatorNop& thisRef){};
	virtual void visit(ActuatorOneWire& thisRef){};
	virtual void visit(ActuatorPwm& thisRef){};
	virtual void visit(ActuatorRangeReference& thisRef){};
	virtual void visit(ActuatorSetPoint& thisRef){};
	virtual void visit(ActuatorTimeLimited& thisRef){};
	virtual void visit(ActuatorValue& thisRef){};
	virtual void visit(Pid& thisRef){};
	virtual void visit(SetPointConstant& thisRef){};
	virtual void visit(SetPointMinMax& thisRef){};
	virtual void visit(SetPointSimple& thisRef){};
	virtual void visit(TempSensor& thisRef){};
	virtual void visit(TempSensorDisconnected& thisRef){};
	virtual void visit(TempSensorExternal& thisRef){};
	virtual void visit(TempSensorFallback& thisRef){};
	virtual void visit(TempSensorMock& thisRef){};
	virtual void visit(OneWireTempSensor& thisRef){};
	virtual void visit(ValveController& thisRef){};
#if WIRING
	virtual void visit(ActuatorPin& thisRef){};
#endif
};
