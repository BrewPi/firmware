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

#include "InterfaceTypeVisitor.h"
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

void InterfaceTypeVisitor::visit(ActuatorBool& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(ActuatorDigitalReference& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(ActuatorInvalid& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(ActuatorMutexDriver& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(ActuatorMutexGroup& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(ActuatorNop& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(ActuatorOneWire& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(ActuatorPwm& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(ActuatorRangeReference& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(ActuatorSetPoint& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(ActuatorTimeLimited& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(ActuatorValue& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(Pid& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(SetPointConstant& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(SetPointMinMax& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(SetPointSimple& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(TempSensor& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(TempSensorDisconnected& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(TempSensorExternal& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(TempSensorFallback& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(TempSensorMock& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(OneWireTempSensor& thisRef) {
    process(thisRef);
}
void InterfaceTypeVisitor::visit(ValveController& thisRef) {
    process(thisRef);
}
#if WIRING
void InterfaceTypeVisitor::visit(ActuatorPin& thisRef) {
    process(thisRef);
}
#endif
