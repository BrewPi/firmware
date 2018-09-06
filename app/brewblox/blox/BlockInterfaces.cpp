#include "ProcessValue.h"
#include "SetPoint.h"
#include "TempSensor.h"
#include "cbox/ObjectBase.h"

namespace cbox {

template <>
const obj_type_t
interfaceIdImpl<TempSensor>()
{
    return 265;
}

template <>
const obj_type_t
interfaceIdImpl<SetPoint>()
{
    return 266;
}

template <>
const obj_type_t
interfaceIdImpl<ProcessValue>()
{
    return 267;
}

} // end namespace cbox
