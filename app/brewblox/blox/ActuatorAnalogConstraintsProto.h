#pragma once

#include "ActuatorAnalogConstrained.h"
#include "proto/cpp/AnalogConstraints.pb.h"

void
setAnalogConstraints(const blox_AnalogConstraints& msg, ActuatorAnalogConstrained& act);

void
getAnalogConstraints(blox_AnalogConstraints& msg, const ActuatorAnalogConstrained& act);
