#pragma once

#include "ActuatorDigitalConstrained.h"
#include "proto/cpp/DigitalConstraints.pb.h"

void
setDigitalConstraints(const blox_DigitalConstraints& msg, ActuatorDigitalConstrained& act);

void
getDigitalConstraints(blox_DigitalConstraints& msg, const ActuatorDigitalConstrained& act);
