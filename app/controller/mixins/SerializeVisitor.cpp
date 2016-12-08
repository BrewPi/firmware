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

#include "SerializeVisitor.h"
#include "Platform.h"

#include "ActuatorMocks.h"
#include "ActuatorInterfaces.h"
#include "ActuatorReferences.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorPwm.h"
#include "ActuatorOneWire.h"
#include "SetPoint.h"
#include "TempSensor.h"
#include "TempSensorDisconnected.h"
#include "TempSensorExternal.h"
#include "TempSensorFallback.h"
#include "TempSensorMock.h"
#include "Pid.h"
#include "OneWireSwitch.h"
#include "ActuatorTimeLimited.h"
#include "ActuatorSetPoint.h"
#include "OneWireTempSensor.h"
#include "ValveController.h"

#if WIRING
#include "ActuatorPin.h"
#endif

void SerializeVisitor::visit(ActuatorBool& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(ActuatorDigitalReference& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(ActuatorInvalid& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(ActuatorMutexDriver& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(ActuatorMutexGroup& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(ActuatorNop& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(ActuatorOneWire& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(ActuatorPwm& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(ActuatorRangeReference& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(ActuatorSetPoint& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(ActuatorTimeLimited& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(ActuatorValue& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(Pid& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(SetPointConstant& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(SetPointMinMax& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(SetPointSimple& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(TempSensor& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(TempSensorDisconnected& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(TempSensorExternal& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(TempSensorFallback& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(TempSensorMock& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(OneWireTempSensor& thisRef) {
    thisRef.serializeImpl(adapter);
}
void SerializeVisitor::visit(ValveController& thisRef) {
    thisRef.serializeImpl(adapter);
}
#if WIRING
void SerializeVisitor::visit(ActuatorPin& thisRef) {
    thisRef.serializeImpl(adapter);
}
#endif
