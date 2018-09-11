#include "ProcessValue.h"
#include "Setpoint.h"
#include "TempSensor.h"
#include "brewblox.pb.h"
#include "cbox/ObjectBase.h"

namespace cbox {

template <>
const obj_type_t
interfaceIdImpl<ProcessValue<temp_t>>()
{
    return BrewbloxFieldOptions_LinkType_ProcessValue;
}

template <>
const obj_type_t
interfaceIdImpl<TempSensor>()
{
    return BrewbloxFieldOptions_LinkType_TempSensor;
}

template <>
const obj_type_t
interfaceIdImpl<Setpoint>()
{
    return BrewbloxFieldOptions_LinkType_Setpoint;
}

} // end namespace cbox
