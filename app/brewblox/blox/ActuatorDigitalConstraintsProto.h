#pragma once

#include "ActuatorDigitalConstrained.h"
#include "cbox/ObjectContainer.h"
#include "proto/cpp/DigitalConstraints.pb.h"

void
setDigitalConstraints(const blox_DigitalConstraints& msg, ActuatorDigitalConstrained& act, cbox::ObjectContainer& objects);

void
getDigitalConstraints(blox_DigitalConstraints& msg, const ActuatorDigitalConstrained& act);
