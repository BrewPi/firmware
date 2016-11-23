# controlbox-cpp
Embedded framework for dynamic configuration of things.

## What it does

Controlbox allows different types of objects to be created,
configured and interacted with. The object configuration is persisted so the same objects are recreated
automatically when the device powers on. 

Objects can be anything - LCD display drivers, various sensors, actuators - anything
you can write code for. 

Each object has a standard way to get information in and out, which works with the
communications protocol, so adding new objects is simple, and doens't require writing any
communications code (typically an object would have to know how to parse some text protocol or
generate some text to be sent over the wire.)

Each object also has a standard way to be configured, and so it can be automatically saved to EEPROM
when first configured and re-created after a power-cycle. Much of this happens for free using the
framework.

The system takes care of running one or more control loops where each object is updated. 
The state of the object is written via the comms layer so the system offers full visibility 
into what is happnening. 


## Why do this?

Many microcontroller programs are simple control loops - they 
exist to read values from sensors and either simply output the values, or 
act on it in some way. The project that inspired this - Brewpi - was no different: reading temperature sensors and
feeding this into a control loop that activated headers/coolers as needed to maintain a temperature.

We found we wrote a lot of boilerplate code - similar code that was changed for each type of object
but basically did the same thing - save some configuration to EEPROM, read/write the current
state to Serial. Not only did this involve a lot of extra coding hours to write this code, but it
also bloated the firmware. 

Controlbox aims to remove the unnecessary repetition by providing a framework that uses a consistent
approach to persistently configuring objects and managing their state at runtime. 
