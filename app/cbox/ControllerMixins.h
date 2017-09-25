/*
 * Copyright 2017 BrewPi
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


class InterfaceMixin {};
class ActuatorDigitalMixin {};
class ActuatorAnalogMixin {};
class ActuatorThresholdMixin {};
class TempSensorMixin {};
class TempSensorMockMixin {};
class TempSensorFallbackMixin {};
class PidMixin {};
class OneWireTempSensorMixin {};
class TempSensorDisconnectedMixin {};
class TempSensorExternalMixin {};
class ActuatorTimeLimitedMixin {};
class ActuatorOffsetMixin {};
class ActuatorPwmMixin {};
class ActuatorMutexGroupMixin {};
class ActuatorMutexDriverMixin {};
class ActuatorValueMixin {};
class ActuatorBoolMixin {};
class ActuatorNopMixin {};
class ActuatorInvalidMixin {};
class SetPointMixin {};
class SetPointSimpleMixin {
protected:
    ~SetPointSimpleMixin() = default;
    void* settingsPtr();
friend class SetPointSimpleCBox;
};
class SetPointConstantMixin {};
class SetPointMinMaxMixin {};
class ActuatorPinMixin {};
class ValveControllerMixin {};
class ActuatorOneWireMixin {};
class TempSensorDelegateMixin {};
class ActuatorDigitalDelegateMixin {};
class ActuatorAnalogDelegateMixin {};
class SetPointDelegateMixin {};
class ProcessValueMixin {};
class SensorSetPointPairMixin {};
class ProcessValueDelegateMixin {};
