#pragma once

/**
 * This class is used to allow the application to store a pointer to the application object in the
 * library object instance.
 */
template <typename T>
class ApplicationObject
{
    T* handle;
public:
    ApplicationObject() : handle(nullptr) {}
    T* get_handle() const { return handle; }
    void set_handle(T* handle) { this->handle = handle; }
};

// We use a void* in the base class to avoid a compile-time dependency from the lib class to the application class.
// (The app class is already compile-time dependent on the lib class.)

class BaseMixin {};
class ActuatorDigitalMixin {};
class ActuatorBoolMixin {};
class ActuatorInvalidMixin {};
class ActuatorMixin : public ApplicationObject<void> {};
class ActuatorMutexDriverMixin {};
class ActuatorMutexGroupMixin : public ApplicationObject<void> {};
class ActuatorNopMixin {};
class ActuatorPinMixin {};
class ActuatorRangeMixin {};
class ActuatorSetPointMixin {};
class ActuatorPwmMixin {};
class ActuatorTimeLimitedMixin {};
class ActuatorValueMixin {};
class OneWireTempSensorMixin {};
class PidMixin {};
class SetPointMixin : public ApplicationObject<void> {};
class SetPointNamedMixin {};
class SetPointConstantMixin {};
class SetPointMinMaxMixin {};
class SetPointSimpleMixin {};
class TempSensorBasicMixin : public ApplicationObject<void> {};
class TempSensorExternalMixin {};
class TempSensorDisconnectedMixin {};
class TempSensorMixin {};
class TempSensorMockMixin {};
class ValveControllerMixin {};
