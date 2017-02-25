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
#include "ActuatorMocks.h"
#include "ActuatorInterfaces.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorPwm.h"
#include "ActuatorOneWire.h"
#include "SetPoint.h"
#include "TempSensorDisconnected.h"
#include "TempSensorExternal.h"
#include "TempSensorFallback.h"
#include "TempSensorMock.h"
#include "Pid.h"
#include "OneWireSwitch.h"
#include "ActuatorTimeLimited.h"
#include "ActuatorSetPoint.h"
#include "OneWireTempSensor.h"
#include "TempSensorDelegate.h"
#include "ValveController.h"
#include "VisitorBase.h"
#include "TempSensorDelegate.h"
#include "ActuatorAnalogDelegate.h"
#include "ActuatorDigitalDelegate.h"
#include "ProcessValueDelegate.h"
#include "SetPointDelegate.h"
#include "SensorSetPointPair.h"

#if WIRING
#include "ActuatorPin.h"
#endif

template< class T >
class VisitorCast : public VisitorBase {
public:
    VisitorCast() : lastProcessed(nullptr) {};
    ~VisitorCast() = default;
public:
    void visit(ActuatorBool& thisRef) final { this->process(thisRef); };
    void visit(ActuatorInvalid& thisRef) final { this->process(thisRef); };
    void visit(ActuatorMutexDriver& thisRef) final { this->process(thisRef); };
    void visit(ActuatorMutexGroup& thisRef) final { this->process(thisRef); };
    void visit(ActuatorNop& thisRef) final { this->process(thisRef); };
    void visit(ActuatorOneWire& thisRef) final { this->process(thisRef); };
    void visit(ActuatorPwm& thisRef) final { this->process(thisRef); };
    void visit(ActuatorSetPoint& thisRef) final { this->process(thisRef); };
    void visit(ActuatorTimeLimited& thisRef) final { this->process(thisRef); };
    void visit(ActuatorValue& thisRef) final { this->process(thisRef); };
    void visit(Pid& thisRef) final { this->process(thisRef); };
    void visit(SetPointConstant& thisRef) final { this->process(thisRef); };
    void visit(SetPointMinMax& thisRef) final { this->process(thisRef); };
    void visit(SetPointSimple& thisRef) final { this->process(thisRef); };
    void visit(TempSensorDisconnected& thisRef) final { this->process(thisRef); };
    void visit(TempSensorExternal& thisRef) final { this->process(thisRef); };
    void visit(TempSensorFallback& thisRef) final { this->process(thisRef); };
    void visit(TempSensorMock& thisRef) final { this->process(thisRef); };
    void visit(OneWireTempSensor& thisRef) final { this->process(thisRef); };
    void visit(ValveController& thisRef) final { this->process(thisRef); };
    void visit(TempSensorDelegate& thisRef) final { this->process(thisRef); };
    void visit(ActuatorDigitalDelegate& thisRef) final { this->process(thisRef); };
    void visit(ActuatorAnalogDelegate& thisRef) final { this->process(thisRef); };
    void visit(SetPointDelegate& thisRef) final { this->process(thisRef); };
    void visit(ProcessValueDelegate& thisRef) final { this->process(thisRef); };
    void visit(SensorSetPointPair& thisRef) final { this->process(thisRef); };
#if WIRING
    void visit(ActuatorPin& thisRef) final { this->process(thisRef); };
#endif

public:
    T * getCastResult(){
        return lastProcessed;
    }

private:
	T * lastProcessed;
	void process(T & ref){ lastProcessed = &ref; };
	void process(Interface & ref){ lastProcessed = nullptr; };
};

template<class T>
inline T* asInterface(Interface * i){
   VisitorCast<T> v;
   i->accept(v);
   return v.getCastResult();
}
