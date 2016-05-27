Controlbox
##########

Some key goals of the controller:

* flexible configuration of various types of controller entities - sensors, actuators, controllers, etc..
* flexible configuration of non-controller elements - input/output devices such as encoders, displays etc.
* dynamic discovery of connected devices (I2C/Onewire)
* small footprint


High-Level View
---------------

The high-level view of a controlbox device is this:

* a microcontroller, with connections to various types of hardware (sensors, actuators, I/O devices)
* the control loop:
    * read devices
    * compute values and actions (control response)
    * update devices
    * log values
* a communciations interface that allows programmatic access to the controller
* self-standing: once configured, the device can continue providing service without external influences
* persistence: object definitions and some key values persist and are maintained after power cycle

Comms Interface
---------------

The Comms interface is an abstraction of a communications channel that allows the controller to be interacted with by
an external source.
The Comms interface is a channel through which data can be sent and received to/from the controller.

In practice, the comms interface will take these forms:

* arduino uno/leonardo: virtual Serial data at 57600 baud over the USB port
* spark core (wired): virtual Serial data at 57600 baud over the USB port
* spark core (wireless): TCP server running on the spark core
* desktop simulator: stdin/stdout to process


Objects
-------

An Object defines a building block in the controller - it defines something that we want to work with in the controller.
Everything that is manipulated in the controller is represented as an object. Examples are:

 * Hardware Sensors (Temp, switch)
 * hardware actuators (digital pin on/off, digital pwm (fast), analog pin, onewire),
 * functional actuators (predictive, pwm (slow), heat-cool-composite),
 * functional temp sensors (filtering), controllers
 * controllers (pid, on/off, thermo-model, etc..)
 * UX input devices: rotary encoder (keypad, etc...)
 * UX output devices: LCD display, buzzer
 * Process Values

This list isn't exhaustive - just to illustrate that there are many types of objects.

A key goal of the controller and the comms interface is to allow these objects to be instantiated, and configured,
and for each object to be assigned a unique ID.
Each object that is externally visible has a unique ID.
The ID is specified when interacting with that object. More on IDs later.

Containers
----------
A container is itself an object, and maintains logical grouping of (sub)objects.
All objects in the controller exist in a container - it's not possible for an object to exist outside a container.

Objects are stored in the container at a particular index, starting with 0, up to some value N-1, where N is the
size of the container.
Note that a container of size N does not have to have N objects - some or all of the slots may have no object assigned
to them.

Containers may be open or closed. An open container allows new objects created in it or objects deleted from it.
An open container supports the Fetch Next Slot command which finds the ID of an available slot in the container.
Closed containers do not allow objects to be created or removed, only existing objects can be retrieved.

Once an object is added to a container, it maintains the same index until that object is removed. Even if objects at lower
indexes are removed, this has no bearing on the indexes of other objects in the container.


The Root Container
------------------
We have a chicken and egg problem: the system mandates each object is hosted in a container, but then you can't create
that container without already having a container to put it in!

The system provides the root container to bootstrap object creation.
This is a generic container object that contains all top-level objects.
Since these top-level objects can also be containers, the system can be used to build nested containment,
i.e. a hierarchy.

The root container is created as part of a system profile - each system profile has it's own root container.


Object IDs
----------
The objects created in the controller exist in the memory space of the controller.

To allow external access to these objects via the Comms interface, each object needs an ID.
Since most object definitions are persistent (surviving power cycles) the ID of an object should also survive
power-cycles.

The ID scheme used is based on the container hierarchy.
The ID of an object is the object's index in it's host container, appended after the host container's ID.
The root container is always implicit, and is not included in the ID.

For example
    * ``5``: a top-level object stored at index 5 (in the root container)
    * ``2.3`` : an object stored at index 3 in a container at index '2' in the root.

Hence, each object in the system has an ID, and these IDs are persistent - the same objects will be available using the
same ID after power-cycle. The object exists until it is explicitly deleted (see Delete Object command.)


Values
------
Values are a central concept in a controller.
Values represent sensor readings, configuration details, set points, controller state, etc..

Values are a type of object (so they have an ID).
This means values may represent a simple variable (such as a configuration value/process variable) or a value derived
externally, such as a sensor reading.
Values may be readable/writable or both. Values are abstracted to provide independence from how the value is computed,
and how the system reacts when that value is assigned.
Values are readable and optionally writable.

All items of interest in the controller that can be logged or manipulated are represented as values, and all values
are optionally logged on each cycle.


System Profile
--------------
The system allows multiple configurations to be stored in eeprom. Each configuration corresponds logically to a distinct
root container.

There are commands to retrieve the active profile and the list of available profiles, as well as commands to create
and delete profiles.

The last activated profile is persisted, so that on reboot the same profile is activated.

While a profile is "open" new objects can be created in it. A profile is open from the time it is created until
another profile is created.

The create profile command is used to create a new profile. The profile is not activated until the
When a new profile is created, any currently active profile (the root container) is then marked as closed.
This means additional objects cannot be created in this profile.  (This limitation may be lifted later if there is
reason.)


Safe Mode Boot
``````````````
On startup the system waits X [TODO] seconds before loading the last activated profile.
The external code can stop this from happening by sending a activate profile command, specifying profile id -1.
This prevents the profile from being loaded.


System Objects
^^^^^^^^^^^^^^
Regular user-space objects are stored with a profile and created/removed by the user. In contrast, system objects
are not bound to any profile but to the controller itself. These system objects are created by the controller.
Most are typically read only with only a few being writable in some cases.

Like user objects, system objects are readable and writable. However, system objects cannot be created or destroyed.

The commands associated with system objects are:
 - read system object
 - write system object
 - list system objects ?? (Not sure if this is needed.)

These commands operate similarly to their corresponding user object commands, but they use the system
object container as their starting point.

The system objects are provided outside of the container hierarchy.
Despite the name (which will probably change!) they are provided by the application.
System objects cannot be created nor deleted via the command interface.

Comms Interface Format
----------------------
The comms interface format has been chosen to be both readable (to developers) and easy to parse:

* text-based format so it can be easily printed and entered using a terminal
* data bytes are hex-encoded, for example, the byte 255 is streamed as the text FF
* whitespace can be freely inserted (they are ignored)
* annotations delimiters [ ] can be freely inserted (they and the contents between are ignored)
* chunk-based newlines terminate the command.

The core data format used by the embedded device internally is binary, but it is transmitted as ASCII to avoid encoding
issues and to make it representable on a terminal. Furthermore, text strings can be liberally inserted by using
annotation delimiters.

A newline is used to terminate a block (a command). Not only does this help with readability, but also ensures that
the system can recover from a dropped byte or synchronization problem from the start of the next command.

Requests and Responses
^^^^^^^^^^^^^^^^^^^^^^
Command Requests are sent to the controller via the inbound comms interface stream. The format for the request is

    request-id  1   hex-encoded byte              This is the identifier for the request
    request-data [variable length] hex-encoded bytes
    newline

Responses always echo the original request data - this provides enough information to allow the originating request
to be identified by the caller.
For simple commands that don't take any parameters, the identifying details are just the command id.
For more complex commands, the response includes the command id plus the parameters to the command.
After the originating request details comes the result of the request.


These commands are supported::

   	CMD_READ_VALUE = 1,			// read a value
   	CMD_WRITE_VALUE = 2,		// write a value
   	CMD_CREATE_OBJECT = 3,		// add object in a container
   	CMD_DELETE_OBJECT = 4,		// delete the object at the specified location
   	CMD_LIST_OBJECTS = 5,		// list objects in a container
    CMD_FREE_SLOT = 6,          // retrieves the next free slot in a container
    CMD_CREATE_PROFILE = 7,     // create a new profile
    CMD_DELETE_PROFILE = 8,     // delete a profile
    CMD_ACTIVATE_PROFILE = 9,   // activate a profile
    CMD_LOG_VALUES = 10,        // log values from the selected container
    CMD_RESET = 11,             // reset the device
    CMD_FREE_SLOT_ROOT = 12,    // find the next free slot in the root container
    CMD_UNUSED = 13,            // unused
    CMD_LIST_PROFILES = 14,     // list the define profile IDs and the active profile
    CMD_READ_SYSTEM_VALUE = 15, // read the value of a system object
    CMD_SET_SYSTEM_VALUE = 16,  // write the value of a system object
    CMD_SET_MASK_VALUE = 17,    // write the value of a user object with a mask to preserve some of the original data

Each command is described in more detail below.


Representation of IDs
^^^^^^^^^^^^^^^^^^^^^
Most comms interface requests and responses make use of object IDs.
Since the IDs are variable length, the system needs a mechanism to determine how long the ID chain is.
Give the IDs are stored in eeprom, it's essential the scheme is space-efficient.

A container can have at most 127 contained objects (and may in fact have fewer).
The MSB of each id in the chain is set to indicate there are more values to come, or cleared if this is the last number
in the chain.

For example
    * ``5`` - since this is the last one, this is encoded simply as ``0x05``
    * ``2.3`` - this is encoded as ``0x82 0x03``

Some rejected approaches were to specify the length at the start, or use a special sentinel (e.g. -1) at the end to mark
the end.
Both of these take more space when the length of the ID chain is less than 8.



Read Value Command
^^^^^^^^^^^^^^^^^^
A read command can request multiple values at once. The last id is followed by newline, like in all other commands.

Command request::

    01          read value command
    id          variable-length ID to read
    type-id		the type id of the object.
    size        size of result data block expected - can be 0 if size is not known.

Command response::

    01          read value command
    id          variable length ID
    type-id		the type-id of the object being read. 0 if not known.
    expectedsize        the size of the data block expected
    actualsize          length of the next data block. Will be 0 for if id does not identify a valid readable value, or
        the expected size was non-zero and not equal to the actual data block size.
    data[size]  the value

If the type-id doesn't match the actual object type-id, or no object exists at the
given id location, a reponse length of 0 is given. 


Write Value Command
^^^^^^^^^^^^^^^^^^^
A write command can write multiple values at once. The last value is followed by newline, like in all other commands.

Command request::

    02          write value command id
    id          object to write to
    type-id		the type of the object being written to. 0 if not known.
    size        the size of the data to follow
    data[size]  the value to write

Command response::

    02          write value command id
    id          object requested to write to
    type			the type of the object being written to. Can be 0 if unknown
    size        requested size of data to write
    data[size]  requested data to write
    type			the actual type of the object. Will be 0 if the object doesn't exist. 
    size        actual data size
    data[size]  actual data


The response describes the value of the object after writing the value.
In most cases this will be the same as the value in the request.
However, objects are free to veto a requested change, either
in part or totally.


Create Object Command
^^^^^^^^^^^^^^^^^^^^^
Creates a new object and places it at a specific location, specified by the id chain. Note that the system does not
automatically delete any object that is at that slot, and so the caller should ensure the slot is empty or the system
will have a memory leak - the old object will be still in memory, but no longer reclaimable since it's pointer has been
overwritten. Slots returned by the "next free slot" command are guaranteed empty, so this is only a concern when
deliberately overwriting an existing slot.

Command request::

    0x03        create object command id
    id+         variable length id chain of the object to be created.
    type        the type of object
    size        size of construction parameters of the object
    data[size]  construction data

Note that unlike read/write value command, the command request may contain only one object creation.


Command Response::

    0x03        create object command id
    id+         id chain specified in the request
    type        the type of the object
    size        size of construction parameters of the object
    data[size]  construction data
    status      zero or positive on success indicating the object was successfully deleted.
                A negative value on error. (These values may later be defined error codes.)

The types of objects that can be created and the format of their definition blocks is defined by the application.


Delete Object Command
^^^^^^^^^^^^^^^^^^^^^

Deletes a previously created object.

Notes

* when deleting a container, all contained objects MUST be deleted first. The system does not check.
* Only objects previously created with these commands (create object, create top-level object, place object)
  should be deleted.
  Attempts to delete any other objects can crash the system.
  In particular, it is not possible to destroy the root container.
  It is immortal and cannot be destroyed (Other than by pulling power to the Arduino).
* attempting to delete an object that has already been deleted has no effect.


Command request::

    0x04    delete object command id
    id+     variable length id chain that specifies the id of the object to delete

Command response::

    0x04    delete object command id
    id+     variable length id chain that specifies the id of the object to delete
    status  zero or greater on success indicating the object was successfully deleted.
            A negative value on error. (These values may later be defined error codes.)


List Profile Command
^^^^^^^^^^^^^^^^^^^^

The list objects command lists all created objects in the system.

Command request::

    0x05        list profile command id
    profile_id  the profile to list

Command response::

    0x05    list profile command id
    repeat
        0x03    create object command id
        id+     variable length id chain
        type		the object type
        len     length of object params
        [len]   object params

(Note that the repeated part of the response is the same data passed to the Create Object command.)

Free Slot Command
^^^^^^^^^^^^^^^^^
Retrieves the next available slot in a container.

Command request::

    0x06    next free slot command id
    id+     id of container

Command response::

   0x06
   id+      the container id
   slot     >=0 if a free slot is available. <0 if not available, or object identified by id isn't an open container.


Create Profile
``````````````
Creates a new profile.  All other profiles are closed, meaning they cannot be added to.
The newly created profile is not activated.

Command request::

    0x07    create profile command id

Command Response::

    0x07    create profile command id
    profile the id of the profile created >=0. negative on error.


Delete Profile
``````````````
Deletes a profile and all objects defined in that profile.
If the profile is the active profile, it is deactivated first, so that any instantiated objects are
cleaned up.


Command request::

    0x08    delete profile command id
    id      profile id to delete

Command Response::

    0x08    delete profile command id
    id      profile id to delete
    status  >=0 if the profile was deleted. <0 on error.


Activate Profile Command
^^^^^^^^^^^^^^^^^^^^^^^^
Unloads the current profile and optionally activates a profile.

Command Request::

    0x09    activate profile command
    id      profile id to activate, or -1 to not activate any profile

Command response::

    0x09    activate profile command
    id      profile id to activate, or -1 to not activate any profile
    status  >=0 on success <0 on error

If the current profile is the most recently one created, objects in the profile are compacted.
Thus, to force the system to compact a profile, deactivate the profile (send command activate profile -1) then active
the profile again. This should be done after deleting many objects to free up the persistent storage.


Log Values Command
``````````````````
Writes out the values for all objects in a container.

Command Request::

    0x0A    log values command id
    flags   0 to log all values, 1 to restrict values to the following id
    [id*]   optional id chain to restrict

Command Response::

    0x0A    log values command id
    flags   from request
    [id*]   optional id from request
    repeat
        id      variable length ID chain
        type-id	the type of the object
        size    length of the next datablock. Will be 0 for if id does not identify a valid readable value.
        data[size]  the value

The response data is the same as the read values command.


Reset
^^^^^
Forces the device to reset.

Command request::

    0x0B    reset command
    flags   if 0, the eeprom is left untouched. if 1, the eeprom is erased.
            add 2 to perform a hard-reset after the command has executed. Hence, sending 0 for flags does nothing.

Response::
    0x0B    reset command
    flags
    0x00    confirmation of command execution.

The eeprom is erased and the response returned before the device is reset.

Free Slot Root Command
^^^^^^^^^^^^^^^^^^^^^^
Retrieves the next available slot in the root container.

Command request::

    0x0C    next free slot command id

Command response::

   0x0C
   slot     >=0 if a free slot is available. <0 if not available.


List Profiles
-------------
Lists the activate profile id, and the profile ids that are defined.

Command Request::

    0x0E    list profiles command

Command Response::

    0x0E        list profiles command
    active      The active profile id, >=0x80 if no active profile
    [id]        a list of profile ids that are defined

(NB: note that this format isn't currently self-describing for length.
For that, it is necessary to indicate the number of profile IDs.)


Read System Value
-----------------
Reads the value of a system object in the system root container. System objects are stored outside of the normal profile
space. For example, the unique ID of a device is stored in the system root container. System objects are instantiated
automatically by the system at startup.

Apart from the changed command ID, the syntax for this command is identical to the read value command (0x01).

Command request::

    0x0F        read system value command
    id          variable-length ID to read
    type			the system object type, or 0 if don't know/care
    size        size of result data block expected - can be 0 if size is not known.

Command response::

    0x0F        read system value command
    id          variable length ID
	type			expected type
    expectedsize        the size of the data block expected
	type			actual type
    actualsize          length of the next data block. Will be 0 for if id does not identify a valid readable value, or
        the expected size was non-zero and not equal to the actual data block size.
    data[size]  the value



Write System Value
------------------
Writes the value of a system object. The syntax for this command is identical to the write value command, apart from a
change in command ID.

Command request::

    0x10        write system value command id
    id          object to write to
    type			the type of the object being written to
    size        the size of the data to follow
    data[size]  the value to write

Command response::

    0x10        write system value command id
    id          object requested to write to
    type			the system object type
    size        requested size of data to write
    data[size]  requested data to write
    type			the actual system object type
    size        actual data size
    data[size]  actual data

The write fails if type is non-zero and doesn't match the actual system
object type. 


Write Masked Value
------------------
This command allows a partial update of an object's value by interleaving a mask along with the data bytes.
When a bit is set in the max, then the corresponding bit in the data is also written to the object. When the mask
has a 0 bit, then that bit is left alone (retains it's existing value, regardless of the value of the datastream.)

The format is similar to the write command, but the data block is interpreted as an interleaving of data byts and
mask bytes.

Command request::

    0x11        write masked value command id
    id          object to write to
    type			the object type
    size        the number of data and mask byte pairs
    data[size*2]  the value and mask to write. the byte at 2N is applied with mask 2N+1 and then written to the object's
                byte at N, preserving existing data where the mask has a 0 bit.

Command response::

    0x11        write masked value command id
    id          object to write to
    type			the object type, can be 0 if not known
    size        the number of data and mask byte pairs
    data[size*2]  the value and mask to write. the byte at 2N is applied with mask 2N+1 and then written to the object's
	type			the actual object type
    size        actual data size. Will be 0 if the id does not reference a writable value object.
    data[size]  current value of the data written



Persistence
-----------
As each object is created, the same command used to create it is stored in eeprom.
This makes it possible to reinstantiate all objects when the controller powers up by simply re-running
all the object creation commands again from eeprom.

After an object has been created, but before it is used, the address it's creation details in eeprom are passed to it.
This allows the eeprom definition block to be used as storage by the object. The first byte of the definition block
lists the length.


Update Cycle
------------
To maintain consistency values are refreshed/computed once per update cycle.
Once a value has been first computed/updated during a given update cycle, subsequent requests to read/update the value
during the same update cycle will return the same value.
Values may also track if they have changed since the last update.

As an implementation detail, this is done using flags to indicate if a value has already been computed for this update cycle.
Alternative schemes using timeouts are too fragile to be used here, since they will then be tied to knowledge of how
long the update cycle is.
Delays in the update cycle will cause the value to be recomputed, providing an inconsistent set of results.
Ensuring all values are stable for the duration of the update cycle is a key tenet to ensuring consistency and repeatability.

Having a two-stage, prepare/update cycle is beneficial to some devices that require some time to compute or sense their value.

Example Sessions
----------------

A fresh clean state.

List objects::

    05

Response:

    05

There are no objects to list, so just the response command id is returned.




Designing New Objects
---------------------

Some guidelines:

- if an object's property/value is immutable and specified in the creation arguments, it may not be necessary to make the
  object a container and expose the property as a sub-value. Instead, clients can retrieve the original value by listing
  the object definition.



System Root Container
---------------------
The system root container contains system objects is automatically provided by the system and instantiated on startup.
Objects in the system root can be read and some written. The system root is not an open container, so new objects
cannot be added, nor existing ones deleted.

System objects are read and written with the "read system object" and "write system object" commands.
The type id for system objects is distinct from the type-id for application objects.
The available type-ids and the corresponding read/write formats are described by the application. 

The system objects and types are freely defined by the application. Here are some guidelines for
the system objects to include:

Slot 0 - Device ID
^^^^^^^^^^^^^^^^^^
On the Arduino, this is 1-byte readable and writable value. The default value with a fresh eeprom is 0xFF. The local
connector assigns a new, unique id to Arduinos when the id has not been assigned (read as the default value).


Slot 1 - ScaledTicksValue
^^^^^^^^^^^^^^^^^^^^^^^^^
This provides the current system time. Normally, this runs in real time. For testing, simulation or amusement the
time can be paused/resumed and scaled faster or slower than actual time.

The read data is 6 bytes:

    0x00    current scaled time as milliseconds since power on (32-bit unsigned, little-endian)
    0x04    scale - time compression factor. 0 stops time. 1 is normal time, 2+ is accelerated time. (16-bit signed.
            Negative values are reserved.)
    0x06    (end)

The write data is 6 bytes:

    0x00    start point - the number of actual milliseconds to consider time 0. (32-bit unsigned, little-endian)
    0x04    scale - (same as for read)
    0x06    (end)

The scale can be used to stop time (scale=0), to run normally (scale=1) or to accelerate time (scale>=2).
(At present, time slowdown is not implemented. A future extension could allow time slowdown by using the MSB of the
scale to indicate that it should be treated as a fraction from [0,1) - the fractional value for scale -x being
x/2^15.)

The offset can be used to advance time to a specific point. It's subtracted from the real time before applying the scale.
This allows manual stepping through time by successively updating the offset.

Neither the current time, offset or scale are persisted to eeprom. On restart, time runs as normal.




Refactor docs after this point.
===============================
This is what remains from the original doc sketching out ideas, or are new ideas in progress.


System Profiles
---------------
What we have been calling the root container so far, is in fact a Profile container, and all the objects configured
in the root container are stored in a Profile.

Multiple profiles can be managed, but at most one profile can be active at any time. On startup, the last active profile
is loaded after 10 seconds. This gives external agents the chance to select a different profile, or request that no profile is loaded.

The profile manager stores the last used profile in eeprom. This profile is automatically selected if no other profile is
requested within 10 seconds after power on.

The profile manager maintains the number of profiles in eeprom. A profile is defined by its start address.

The beauty of profiles is that they can be switched at runtime, without requiring a power cycle.

The last profile in eeprom is always open and can be added to. Closed profiles cannot have new objects added to them,
although objects can be removed and object definitions changed in place.

New profile commands?
- since the root container is provided by the profile, this could always have an available object at the first index.
- exposes a value for the current profile which can be set.

Create profile/Delete profile
- as new commands?
- as create object?

Profile Container, part of all root containers.
- maximum of 4 profiles (for arduino.)
- new object type for profile
- create Profile object, target is profile container (0 in the root). result is new profile id (stored in the profile itself so it's persistent.)
- value for current profile, setting this changes the profile.

0 - root container
0.0 - active profile
0.1 - profiles?
0.2 -

alternative

0 - root container
0.0 - active profile
0.1 - profiles
0.1.0 - profile 0, a read-only Value that lists the creation statements for this profile.
0.1.1 - profile 1,
etc..

profile as new object. container is at position 0 in the root.

create object will fail if there is already a profile active at that location.

create object command
03 10 80 00 00

03 - create object command
10 - profile object type id
80 00 - place in container 00 (80, because not last in chain) at index 00.




blank system
startup: doesn't find magic number in eeprom so erases eeprom
creates profile 0 and sets this as active
serial comms ready to receive object creation commands

later
client issues either delete profile or create profile command
create profile command closes current profile and creates a new profile

delete profile erases the profile





Last used profile stored in eeprom. Outside of object system.

Profile is create object statement in eeprom. Fixed size. includes location in eeprom where profile objects begin.





Profile object
- current profile exposed as a value (stored in eeprom to avoid runtime overhead.)
-



Logging
-------
At present logging is based on log IDs and parameters.
Some logs should be considered events, e.g. temp sensor connection changes. (Maybe all logs are events - logs and events being the same thing - an event ID with parameters.)

Keep existing logging scheme but write out parameters in binary.
Debug logs can be written out as annotations. These have the benefit of being completely ignored, rather than as before,
breaking the protocol if they weren't inserted in the correct place, which can be difficult to code when the log
is generated while producing serial output.

Removing the need for string arguments avoids conversion. E.g. temp sensor disconnect used to have to convert the address,
 now it can be written as data.











Listing the Devices
-------------------
The serial protocol will provide a command to list the construction of a device.
This essentially replays the construction commands previously submitted.

If a client knows the device ID it wants to retrieve construction details for, then it can go ahead and issue the
command to list the device.

Otherwise, clients can start by browsing the global container, with reserved ID '0'.
This is the root in which all devices are contained.





EEPROM Management
-----------------

The eeprom stores the create commands that are required in order to instantiate and configure various objects in the system.
The commands are:

* create (item id, variable args block)
* set (item id) - configures an item/sets a value

Deleted Items
-------------
When an item is deleted all it's configuration is removed from eeprom.
In practice this is done by setting a flag against each configuration to indicate that it is deleted and can be ignored.
At certain times, such as when there is little free space, the eeprom is compacted to remove the deleted items.


Hardware Detection
------------------
A special container type can be instantiated by the client that detects installed hardware.

For instance, instantiating a OneWireDetector container, configured with the OneWire bus object.
(At present, OneWire bus instances are hard-coded.
These will now be regular configurable objects, and not have special treatment.)

When devices are added or removed, the container sets a flag, which causes an event to be posted to serial.
The Arduino connector then requests the new list of detected devices, and determines connected/disconnected events based
on the previous detected hardware.


Logging and Arbitrary Hardware
------------------------------
One of the goals of the new controller design is that users can freely plugin sensors and have these monitored (after
installing the hardware, i.e. issuing appropriate Create Object commands.)

Sensors are logged by their object IDs, since the IDs are persisted and last for the duration of the object. (The corner-case
 of one object replacing another has to be handled in the upper layers so that the database/service is aware that a potentially different
 device is now producing the data.)







Detected and Installed Hardware
-------------------------------


Display
-------
To avoid a hard-coded menu system, the display text is written as a template, which includes placeholders for values.

For example, the current display could be written as::

    Mode   %15r_mode_mdstr
    Beer   %4r_beer_pv   %4r_beer_sv  %_tempunit
    Fridge %4r_fridge_pv %4r_fridge_sv %_tempunit
    %15l_state_statestr %7r_statetime_hms

The conceptual format for a place holder is::

    '%' - (distinguishes start of placeholder)
    width  - number of chars to output on the lcd
    alignment - l,r for left, right
    value - an identifier for a readable value in the system
    format - an identifier for a formatter to use to format the value

Note that this is pseudo-code - the internal representation would most likely be more compact, e.g. using the MSB of a
byte as an excape code or some similar scheme to efficiently encode the placeholders.

The display checks values for changes and updates only those parts that have changed.

With the use of a template (stored either in code, or fetched from eeprom) allows flexibility to configure the display
according to the runtime application.
This might be as trivial as changing "beer" to "wine" for a 2 stage controller in a winery, or reworking the display
completely for a different application.

Rotary Encoder/Menu
-------------------
The menu system can use the same display template to drive the menu.

When the user hits the rotary encoder, the first template value that is a WritableValue (indicating that the value can
be assigned) is found.
The portion of the display is then flashed (mode setting in the example above).
Rotating the encoder calls increment/decrement on the associated value object to fetch next/previous values, and these
are then formatted as per the display template.
As with the current menu code, a timeout is used to restore the value to the original.

The key message here is that the menu can be driven also from the template, generically, without requiring the display
to be hard-coded.

After setting mode, pressing the encoder again jumps to the next writable value.
For example after setting the mode to "beer constant", the controllers mark fridge_sv as non-writable, so the next
writable value is beer_sv.
After setting this value, pushing the encoder again jumps to the next value, but as there are no more writable values,
the menu exits.


Serial Comms Format
-------------------
The serial comms format will be a mixture of text and hex-encoded binary.

The hex-encoded binary avoids parsing and allows the same data to be stored in memory, transmitted over serial, and
persisted to eeprom.
To assist with testing, the format will have quoted text as comments to allow text strings to be inserted free-form into
the format, which are ignored by the controller.

For example: to define a temp sensor, the format looks like::

    <add>0A<id>00<OneWireTempSensor>01<address>28C80E9A0300009C\n

Here `0A` is the (arbitrary) value for the add command. The syntax for the command expects the subsequent byte to denote
which slot (the id) the object is stored in. When it's 00, the id is assigned by the arduino.
When it's non-zero the device at the existing slot is replaced with the new one.

The next byte defines the type of device to create.
Each object type (class) has a unique type ID which is given here.
In this case, 01 denotes a Onewire temp sensor.
The remainder of the line are parameters specific to the type of the device being configured.
Here, the OneWire temp sensor takes an additional argument, the onewire address.

On receiving the command, the controller does any validation needed, and instantiates the requested object.
Once instantiated, the response is sent::

    <added>0A<code>00<id>22

This indicates that that the add command returned a zero (no-error) status code and the id of the created temp sensor
is 0x22.

Other protocol details
----------------------
The newline after each complete command is mandatory and serves to separate commands.
This allows variable length commands to be supported.
The alternative is to include a byte count as part of the initial command definition - I feel the terminator character
is more straightforward and enhances readability.

To ensure each response can be easily paired with each request, each request includes a unique command ID that is
defined by the client.
When the arduino sends a response, it includes the unique command ID provided by the client.

The arduino may also output <annotated> hex bytes in DEBUG mode, or in production builds, if space permits.
If there is insufficient space in the arduino for the annotated protocol, then it is annotated by the python Arduino
handler.
The text in angled brackets is optional and is there simply to make reading the format simpler.
It's optionally generated by clients (And optionally verified by the Arduino in DEBUG mode).

Composite IDs
-------------
Global IDs are simple values, e.g. ``0x10``.
To allow IDs to also reference objects contained within a container we need composite IDs.
E.g. 0x1001 refers to the object at location 0x01 in object (container) 0x10.

For the serial protocol, these composite IDs need to be encoded.
Because the IDs are no longer fixed length, the parser needs to know how long they are.
A simple scheme is to set the MSB to 1 if the ID has another byte following.
So 0x1001 would be encoded as 0x9001.
(An Alternative scheme would be to prefix each ID with the number of bytes, essentially making them array parameters -
this is also workable.)



Serial Comms and Generic Function Call Mechanism
------------------------------------------------
In essence, the serial comms is a generic function call mechanism, using the same encode/decode logic for all functions
offered by the Arduino.
This will dramatically reduce the size of encode-decode logic used, so there is more room for useful functionality.

There is a global function table that defines the function's external ID (e.g 0x0A for Add Object) and the function
argument types and return value.
This is used to (optionally) verify the arguments passed in, and guides the decode process.
Recall that each object type that can be externally instantiated has an associated typeID so these can also be part of
function call arguments and results - all entities are referenced by their ID.

Some examples of the functions supported:

 * add object - constructs a new object and returns its external ID (or returns an error code if this is not possible.)
 * read values - reads a number of values (each referenced via their ID) and returns each value.
   (the result includes status flags for invalid IDs and values that cannot be read.)
 * write values - write a number of values in one hit
 * reset values?? (I would prefer the default values being exposed so a reset is simply writing the defaults.)
 * read default value?? - fetches the default value for a given value object.
 * read default value?? - fetches the default value for a given value object.
   I'm thinking defaults should be in the arduino connector (python) to save space.

The read values/write values replace much of the existing functionality
 * t - list SV/PV temperatures and state - read values from temp sensors (either the base hardware sensor, or the
   filtered sensor, both are available).
   State is fetched by reading the values of actuators.
 * j - update settings - write values
 * l - read display - now read "buffer" value on a display object.
 * A/a - enable/disable alarm - write value to alarm actuator
 * C/S - reset constants/settings - write values to defaults (either defaults stored in script, or fetched from Arduino)
 * c/s - read constants/settings - read values
 * n   - print version string - we may keep this as is, or create a special value for the version.


 Commands replaced by top-level functions

  * E   - initalize eeprom.
  * h{} - list detected hardware. This can either be a new top-level function, or a special value container
    can be instantiated (by the script) whose job is to provide details of the hardware detected.
  * d{} - list configured devices (from eeprom.)
    This is a new top-level function that outputs the device recipe stored in eeprom.
  *  U{} - define a device - replaced by the general Add object function.


Destroying Objects
------------------

* The eeprom data is variable length (due to different parametesr for different objects)
* destroy disables all commands in eeprom (create/write) that reference the object
* a compacting phase later removes the empty spaces.

Compacting the eeprom
* start from the beginning, read write pointers the same
* work through, copying command chunks. There read pointer skips over disabled commands.
* write 0xFF to the end

Value Reference
---------------
A value reference is a proxy to some other value. It can be used to break cyclic dependencies, or for filling in
the actual value dynamically later.

Containers, Objects and Values, Oh My!
--------------------------------------

An object - some arbitrary identifiable item.
A container - exposes an interface for retrieving known related items.
A value - a type of object that represents a value.
Has a associated log level so that information values are not logged normally.

An asynchornous value - prepare/isReady/read. Prepare returns the minimum delay to wait before calling read.

Example Hierarchy

0 Root container (implicit)
1 System info (static, readonly)
1.0 - version string
1.1 - shield type
1.2 - embedded device type (leonardo/arduino/spark etc...)
1.3 - etc...

Basic temp sensor. Also a container
0 - the temperature value

Filtered temp sensor.
0 - the filter (provides the filtered value)
0.0 - filter value
0.1 - filter coefficient
1 - the temp sensor

# Alternatively

TempSensorWithFilter
0 - the temp sensor (raw value)
1..N - filters
1.0 - the filtered value
1.1 - the co-efficient

PID Controller (v0)
0 - state (heating/cooling waiting etc...)
0.1 - state override (?) allow external control of controller state
1 - beer temp sensor    (hard-coded indices here.)
2 - fridge temp sensor
3 - heat actuator
4 - cool actuator
5 - control parameters
5.0 - mode
5.1 - kp
5.2 - ki
5.3 - kd
5.4 - beerSet
5.5 - fridgeSet
5.6 - tempRange (min.max)
5.7 - idleRange
5.8 - heatingTargetRange
5.9 - coolingTargetRange
5.10 - pidMax
6 - runtime parameters
6.0 - diff
6.1 - p
6.2 - i
6.3 - d
6.4 - peaks, estimates, beer slope, diff integral etc...


* orgnaization depends upon if values are pulled or pushed.

pull: request filtered temp -> filter requests basic temp, and updates to produce new filter temp
push: new basic temp pushed into filters, cache flag set.
maybe this is handled at the level of the container.

how does the filter know to wait for reading from the temp sensor? Has to be handled in the main loop, that async
devices are setup and waited for.
Need to enumerate async devices first.

* can the filtered temp sensor be dynamic and allow additional filters? e.g. fast, slow and slope?
* a slope value. it's output is the difference between successive values.


In code, composition can make use of known types at compile time. Or the simple dynamic typing at runtime.



read - given an object id, if the object is a value, returns the value.
If the object is a container, fetches the object at location 0 and re-applies.
This allows objects to later be changed to contaienrs to expose more values wihtout breaking compatibility with existing code.

write - same for write

create  - create an object and store in the specified container (default is top level container 0). Returns the id.
        - takes a binary array as construction parameter

enumerate object - walk the entire object hierarchy to enumerate each object
the handler will do something, e.g. look for cached values and reset them


Templates/Recipes
-----------------
Rather than having to repeat commands to define many objects to build (say) a 2-stage temp controller over and over for
each instance of the controller required (e.g. each fridge), it would be nice to create recipes containing many objects.
These define input and outputs to the whole recipe and hide the internal objects used and complexity of the wiring.
This reuse may save eeprom space.

It's not clear yet if these recipes should exist on the arduino or externally. Update: they should be external.
Much easier to manage and less code. If they are stored in the arduino, it is implemented as a new objec type that
take's a custom construction definition block, and then creates the required sub-objects and exposes them either
via the container interface or as a composite read value which is decoded in the controller.





Version
-------
The version number, shield type and other compiled in data was previously available as a separate command.
For othogonality, the system will present the read-only system details as values in a special container.



Scenarios
=========

Version
-------

Scenario: fetch the version number. The version number is compiled into the software at build time and is a 8-character
long alphanumeric code.

    Given a started arduino
    Then serial read of value build number is an 8-character string




Editable Variable and LCD display
---------------------------------
Scenario: configure a process value that can be read/written via serial and also via the LCD menu

Feature: Process variable with display
    Background:
        Given a clean eeprom
        And a defined persisted process variable P
    Scenario: process value can be set and read via serial
        When serial write value 42 to P
        Then serial read of value P is 42
    Scenario:
        Given a display configured to display process variable P at the top left as tenths
        The display shows 42.0 at the top left


Design Notes on Profile
-----------------------
Can't decide if the profile should expose the value as part of the read/write interface, or should push the value
to some other object.

if controller uses an object reference
- not immediately aware of changes (which may be relevant to help avoid a large D when SV changes.). To see changes
    it will have to maintain a lastValue. With a value setter, it has both the old value and the new value and can compute
        the difference right there.
    not a biggie - the controller just has to maintain the last value read to compute deltas.

- changing mode?
    change the SV object reference. (requires also a change to eeprom - the id_chain should then be the same length.)
    this is mainly about setting the SV for fridge temp. In fridge mode it's a simple value/profile,
    in beer mode, it's a PID value from the aggregate setpoints of the carboys.
- logging:
    the value is logged as some other value, external code has to keep track of the relationship.

if controller uses it's own internal value (exposed as a container)
- can be aware of changes immediately since it implements the write() method and compute the delta so that this can be
    compensated for
- changing mode
    this is someone else's problem. the controller just cares about the SV value
    e.g. a composite mode object that wraps a simple value (fridge value), a profile (fridge profile) or the contained beers PID value
    easier to change mode since it's just a single byte change for the mode state in eeprom.


A pid controller: as a container:
    the needed external items (actuator/sensors) are made available as contained references. They can be instantiated in place
    or they can be instantiated externally and use a proxy.
    + easy to change sensors/actuators (direct or proxy) without rebuilding the controller
    + separation of config values vs. logged values

    item 0: PWM actuator to reduce value        (default is NULL)
    item 1: PWM actuator to increase value      (default is NULL)
    item 2: parameters:         a EepromValue object not logged, with a local interface to retrieve specific parameter values
        Kp, Ki, Kd values
        min, max values
        pidMax value
        idle, mode (off, on)

    item 3: SV - the set value
               - this can be a contained value since it's updated after the controller. easy to change

    item 4: PV (e.g. temp sensor)
        if this is a contained value then the prepare/update cycle will prepare the controller before the sensor, but
                    update always happens after prepare so the sensor value will be read.
        if decoupling is needed, a proxy value, that wraps the target value can be used (e.g. temp sensor lifetime longer
                                    than controller lifetime.)

Want to have a control loop to run as fast as possible? with sensors providing output
have a 1 second trigger to log values and fetch new temps?

use case: pressure controller, water fill controller etc - may need to respond much quicker
avoids inner loop to process serial commands (cleaner), no side affects between prepare/update.
use

does the loop need a delay?
maybe not. but some operations may need to be done on a timer
 - logging
 - update display
 - etc...


