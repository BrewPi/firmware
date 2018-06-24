## Actuator interfaces
There are 3 different pure virtual interface classes for actuators.
The `Actuator` interface is inherited by all actuators. Next to that, all Actuators are either digital or range.

### Actuator
Interface for all actuators, only supports 2 interface functions:

    virtual uint8_t type() const = 0;
    virtual void update() = 0;

### ActuatorDigital
A digital actuator is simply ON or OFF. ActuatorDigital adds the these two functions:

    virtual void setState(State state) = 0;
    virtual State getState() const = 0;

### ActuatorAnalog
A range actuator accepts a ranged value. Examples are PWM actuators that can be partially ON (0-100%), or SetPoint actuators that manipulate a temperature set point. The value of the actuator is a fixed point temp_t value.
Next to the set/get value, the interface also has a readValue() function. This function reads the actually achieved value, instead of the set value. This could be the average PWM duty cycle or a measured temperature difference (instead of a setpoint difference).

    virtual void setValue(temp_t const& val) = 0;
    virtual temp_t getValue() const = 0; // get set value
    virtual temp_t readValue() const = 0; // read actual achieved value
    virtual temp_t min() const = 0;
    virtual temp_t max() const = 0;

### ActuatorThreshold
ActuatorThreshold is not used yet. The idea is that it is a digital actuator that accepts a range input. It switches the output ON or OFF when a certain threshold is passed.
