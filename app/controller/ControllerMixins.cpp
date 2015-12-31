#include "ControllerMixins.h"
#include "Pid.h"
#include "TempSensor.h"
#include "TempSensorBasic.h"

void PidMixin::serialize(JSON::Adapter& adapter) {
    Pid* pid = (Pid*)this;
    JSON::Class root(adapter, "Pid");
    std::string name(getName()); // get name as std string for json_writer
    JSON_E(adapter, name);
    JSON_E(adapter, pid->enabled);
    JSON_E(adapter, pid->setPoint);
    JSON_E(adapter, pid->inputSensor);

    JSON_E(adapter, pid->inputError);
    JSON_E(adapter, pid->Kp);
    JSON_E(adapter, pid->Ti);
    JSON_E(adapter, pid->Td);
    JSON_E(adapter, pid->p);
    JSON_E(adapter, pid->i);
    JSON_E(adapter, pid->d);

    JSON_E(adapter, pid->actuatorIsNegative);
    JSON_T(adapter, pid->outputActuator);
}


void TempSensorMixin::serialize(JSON::Adapter& adapter) {
    TempSensor* sensor = (TempSensor*)this;
    JSON::Class root(adapter, "TempSensor");
    std::string name(sensor->getName()); // get name as std string for json_writer
    JSON_E(adapter, name);
    JSON_T(adapter, sensor->sensor);
}


