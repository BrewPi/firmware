#pragma once

#include "bloc.h"
#include "OneWireTempSensor.pb.h"
#include "OneWireTempSensor.h"
#include "OneWireBusCBox.h"

// since we only have one then might as well reference it directly
// this will change when support for multiple busses is added.
extern OneWireBusCBox oneWireBus;

BLOC_CLASS_SETTINGS_AND_STATE(OneWireTempSensor, &oneWireBus.oneWire());
