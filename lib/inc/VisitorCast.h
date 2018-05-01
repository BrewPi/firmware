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
#include "Platform.h"
#include "ActuatorMocks.h"
#include "ActuatorInterfaces.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorMutexGroup.h"
#include "ActuatorMutexGroupDelegate.h"
#include "ActuatorMutexGroupDisabled.h"
#include "ActuatorPwm.h"
#include "SetPoint.h"
#include "TempSensorDisconnected.h"
#include "TempSensorExternal.h"
#include "TempSensorFallback.h"
#include "TempSensorMock.h"
#include "Pid.h"
#include "ActuatorTimeLimited.h"
#include "ActuatorOffset.h"
#include "OneWireTempSensor.h"
#include "TempSensorDelegate.h"
#include "VisitorBase.h"
#include "TempSensorDelegate.h"
#include "ActuatorDigitalDelegate.h"
#include "ProcessValueDelegate.h"
#include "SetPointDelegate.h"
#include "SensorSetPointPair.h"

#if WIRING
#include "ActuatorPin.h"
#endif

#if BREWPI_DS2413
#include "ActuatorOneWire.h"
#endif

#if BREWPI_DS2408
#include "ValveController.h"
#endif

template< class T >
class VisitorCast : public VisitorBase {
public:
    VisitorCast() : lastProcessed(nullptr) {};
    ~VisitorCast() = default;
public:
    virtual void visit(ActuatorBool& thisRef) override final { this->process(thisRef); };
    virtual void visit(ActuatorInvalid& thisRef) override final { this->process(thisRef); };
    virtual void visit(ActuatorMutexDriver& thisRef) override final { this->process(thisRef); };
    virtual void visit(ActuatorMutexGroup& thisRef) override final { this->process(thisRef); };
    virtual void visit(ActuatorMutexGroupDelegate& thisRef) override final { this->process(thisRef); };
    virtual void visit(ActuatorMutexGroupDisabled& thisRef) override final { this->process(thisRef); };
    virtual void visit(ActuatorNop& thisRef) override final { this->process(thisRef); };
#if BREWPI_DS2413
    virtual void visit(ActuatorOneWire& thisRef) override final { this->process(thisRef); };
#endif
    virtual void visit(ActuatorPwm& thisRef) override final { this->process(thisRef); };
    virtual void visit(ActuatorOffset& thisRef) override final { this->process(thisRef); };
    virtual void visit(ActuatorTimeLimited& thisRef) override final { this->process(thisRef); };
    virtual void visit(ActuatorValue& thisRef) override final { this->process(thisRef); };
    virtual void visit(Pid& thisRef) override final { this->process(thisRef); };
    virtual void visit(SetPointConstant& thisRef) override final { this->process(thisRef); };
    virtual void visit(SetPointMinMax& thisRef) override final { this->process(thisRef); };
    virtual void visit(SetPointSimple& thisRef) override final { this->process(thisRef); };
    virtual void visit(TempSensorDisconnected& thisRef) override final { this->process(thisRef); };
    virtual void visit(TempSensorExternal& thisRef) override final { this->process(thisRef); };
    virtual void visit(TempSensorFallback& thisRef) override final { this->process(thisRef); };
    virtual void visit(TempSensorMock& thisRef) override final { this->process(thisRef); };
    virtual void visit(OneWireTempSensor& thisRef) override final { this->process(thisRef); };
#if BREWPI_DS2408
    virtual void visit(ValveController& thisRef) override final { this->process(thisRef); };
#endif
    virtual void visit(TempSensorDelegate& thisRef) override final { this->process(thisRef); };
    virtual void visit(ActuatorDigitalDelegate& thisRef) override final { this->process(thisRef); };
    virtual void visit(SetPointDelegate& thisRef) override final { this->process(thisRef); };
    virtual void visit(ProcessValueDelegate& thisRef) override final { this->process(thisRef); };
    virtual void visit(SensorSetPointPair& thisRef) override final { this->process(thisRef); };
#if WIRING
    virtual void visit(ActuatorPin& thisRef) override final { this->process(thisRef); };
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
