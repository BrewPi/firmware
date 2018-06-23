#include "ActuatorMutexDriver.h"
#include "ActuatorMutexGroup.h"

ActuatorMutexDriver::ActuatorMutexDriver(ActuatorDigital & target, ActuatorMutexGroup * m) : target(target), mutexGroup(m){
	m->registerActuator(this);
}

void ActuatorMutexDriver::setMutex(ActuatorMutexGroup * mutex){
	if(mutexGroup != nullptr){
		mutexGroup->unRegisterActuator(this); // unregister at old group
	}
	if(mutex != nullptr){
		mutex->registerActuator(this); // register at new group
	}
	mutexGroup = mutex;
}

// To activate actuator, permission is asked from mutexGroup, false is always allowed
  // when priority not specified, default to highest priority
void ActuatorMutexDriver::setState(State state, int8_t priority){
  if(mutexGroup != nullptr){
	  if(mutexGroup->request(this, state, priority)){
		  target.setState(state);
		  if(target.getState() != state){
			  // if setting the target failed, cancel the request to prevent blocking other actuators
			  mutexGroup->cancelRequest(this);
		  }
	  }
  }
  else{
	  target.setState(state); // if mutex group is not set, just pass on the call
  }
}
