#pragma once

/* Empty Mixins. Application can include non-empty mixins instead of this file
 * Using protected non-virtual destructors to prevent destruction through base class
 */

class TempSensorBasicMixin {
protected:
    ~TempSensorBasicMixin() = default;
};

class TempSensorMockMixin {
protected:
    ~TempSensorMockMixin() = default;
};

class TempSensorMixin {
protected:
    ~TempSensorMixin() = default;
};

class PidMixin {
protected:
    ~PidMixin() = default;
};

class OneWireTempSensorMixin {
protected:
    ~OneWireTempSensorMixin() = default;
};

class TempSensorDisconnectedMixin {
protected:
    ~TempSensorDisconnectedMixin() = default;
};

class TempSensorExternalMixin {
protected:
    ~TempSensorExternalMixin() = default;
};


class ActuatorMixin {
protected:
    ~ActuatorMixin() = default;
};


class ActuatorTimeLimitedMixin {
protected:
    ~ActuatorTimeLimitedMixin() = default;
};

class ActuatorSetPointMixin {
protected:
    ~ActuatorSetPointMixin() = default;
};

class ActuatorPwmMixin {
protected:
    ~ActuatorPwmMixin() = default;
};

class ActuatorMutexGroupMixin {
protected:
    ~ActuatorMutexGroupMixin() = default;
};

class ActuatorMutexDriverMixin {
protected:
    ~ActuatorMutexDriverMixin() = default;
};

class ActuatorValueMixin {
protected:
    ~ActuatorValueMixin() = default;};

class ActuatorBoolMixin {
protected:
    ~ActuatorBoolMixin() = default;
};

class ActuatorNopMixin {
protected:
    ~ActuatorNopMixin() = default;
};

class ActuatorInvalidMixin {
protected:
    ~ActuatorInvalidMixin() = default;
};

class ActuatorDigitalMixin {
protected:
    ~ActuatorDigitalMixin() = default;
};

class SetPointMixin {
protected:
    ~SetPointMixin() = default;
};

class SetPointSimpleMixin {
protected:
    ~SetPointSimpleMixin() = default;
};

class SetPointConstantMixin {
protected:
    ~SetPointConstantMixin() = default;
};

class SetPointMinMaxMixin {
protected:
    ~SetPointMinMaxMixin() = default;
};

class ActuatorPinMixin {
protected:
    ~ActuatorPinMixin() = default;
};

class ValveControllerMixin {
protected:
    ~ValveControllerMixin() = default;
};

class ActuatorOneWireMixin {
protected:
    ~ActuatorOneWireMixin() = default;
};

class ActuatorForwarderMixin {
protected:
    ~ActuatorForwarderMixin() = default;
};

