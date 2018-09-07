#include "ProcessValue.h"
#include "SetPoint.h"
#include "TempSensor.h"
#include "brewblox.pb.h"
#include "cbox/ObjectBase.h"

namespace cbox {

template <>
const obj_type_t
interfaceIdImpl<ProcessValue>()
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
interfaceIdImpl<SetPoint>()
{
    return BrewbloxFieldOptions_LinkType_Setpoint;
}

} // end namespace cbox
