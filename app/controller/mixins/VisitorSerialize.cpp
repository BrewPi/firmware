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

#include "VisitorSerialize.h"

#include "Platform.h"
#include "Brewpi.h"

#include "ActuatorMocks.h"
#include "ActuatorInterfaces.h"
#include "ActuatorMutexDriver.h"
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
#include "ActuatorDigitalDelegate.h"
#include "SetPointDelegate.h"
#include "SensorSetPointPair.h"
#include "ProcessValueDelegate.h"

#if WIRING
#include "ActuatorPin.h"
#endif

#if BREWPI_DS2408
#include "ValveController.h"
#endif

#if BREWPI_DS2413
#include "ActuatorOneWire.h"
#endif

void VisitorSerialize::visit(ActuatorBool& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(ActuatorInvalid& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(ActuatorMutexDriver& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(ActuatorMutexGroup& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(ActuatorNop& thisRef) {
    thisRef.serializeImpl(adapter);
}
#if BREWPI_DS2413
void VisitorSerialize::visit(ActuatorOneWire& thisRef) {
    thisRef.serializeImpl(adapter);
}
#endif
void VisitorSerialize::visit(ActuatorPwm& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(ActuatorOffset& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(ActuatorTimeLimited& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(ActuatorValue& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(Pid& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(SetPointConstant& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(SetPointMinMax& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(SetPointSimple& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(TempSensorDisconnected& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(TempSensorExternal& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(TempSensorFallback& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(TempSensorMock& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(OneWireTempSensor& thisRef) {
    thisRef.serializeImpl(adapter);
}
#if BREWPI_DS2408
void VisitorSerialize::visit(ValveController& thisRef) {
    thisRef.serializeImpl(adapter);
}
#endif
void VisitorSerialize::visit(TempSensorDelegate& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(ActuatorDigitalDelegate& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(ProcessValueDelegate& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(SetPointDelegate& thisRef) {
    thisRef.serializeImpl(adapter);
}
void VisitorSerialize::visit(SensorSetPointPair& thisRef) {
    thisRef.serializeImpl(adapter);
}

#if WIRING
void VisitorSerialize::visit(ActuatorPin& thisRef) {
    thisRef.serializeImpl(adapter);
}
#endif
