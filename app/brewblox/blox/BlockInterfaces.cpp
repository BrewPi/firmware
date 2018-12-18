#include "ActuatorAnalogConstrained.h"
#include "ActuatorDigitalConstrained.h"
#include "ActuatorPwm.h"
#include "Balancer.h"
#include "DigitalConstraints.pb.h"
#include "FixedPoint.h"
#include "OneWireDevice.h"
#include "ProcessValue.h"
#include "Setpoint.h"
#include "SetpointSensorPair.h"
#include "TempSensor.h"
#include "brewblox.pb.h"
#include "cbox/ObjectBase.h"

namespace cbox {

template <>
const obj_type_t
interfaceIdImpl<ProcessValue<safe_elastic_fixed_point<11, 12, int32_t>>>()
{
    return BrewbloxFieldOptions_LinkType_ProcessValueLink;
}

template <>
const obj_type_t
interfaceIdImpl<TempSensor>()
{
    return BrewbloxFieldOptions_LinkType_TempSensorLink;
}

template <>
const obj_type_t
interfaceIdImpl<SetpointSensorPair>()
{
    return BrewbloxFieldOptions_LinkType_SetpointSensorPairLink;
}

template <>
const obj_type_t
interfaceIdImpl<Setpoint>()
{
    return BrewbloxFieldOptions_LinkType_SetpointLink;
}

template <>
const obj_type_t
interfaceIdImpl<ActuatorAnalogConstrained>()
{
    return BrewbloxFieldOptions_LinkType_ActuatorAnalogLink;
}

template <>
const obj_type_t
interfaceIdImpl<ActuatorDigitalConstrained>()
{
    return BrewbloxFieldOptions_LinkType_ActuatorDigitalLink;
}

template <>
const obj_type_t
interfaceIdImpl<TimedMutex>()
{
    return BrewbloxFieldOptions_LinkType_MutexLink;
}

template <>
const obj_type_t
interfaceIdImpl<Balancer<blox_DigitalConstraint_mutex_tag>>()
{
    return BrewbloxFieldOptions_LinkType_BalancerLink;
}

template <>
const obj_type_t
interfaceIdImpl<OneWireDevice>()
{
    return BrewbloxFieldOptions_LinkType_OneWireDeviceLink;
}

template <>
const obj_type_t
interfaceIdImpl<ActuatorPwm>()
{
    return BrewbloxFieldOptions_LinkType_ActuatorPwmLink;
}

} // end namespace cbox
