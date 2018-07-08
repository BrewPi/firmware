#include "ActuatorMutexDriver.h"
#include "ActuatorMutexGroup.h"

ActuatorMutexDriver::ActuatorMutexDriver(ActuatorDigital & target, ActuatorMutexGroupInterface & m) : target(target), mutexGroup(m){
	m.registerActuator(this);
}

// To activate actuator, permission is asked from mutexGroup, false is always allowed
  // when priority not specified, default to highest priority
void ActuatorMutexDriver::setState(State state, int8_t priority){
  if(mutexGroup.request(this, state, priority)){
      target.setState(state);
      if(target.getState() != state){
          // if setting the target failed, cancel the request to prevent blocking other actuators
          mutexGroup.cancelRequest(this);
      }
  }
}
