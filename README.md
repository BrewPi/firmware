# FreeRTOS on the Core

FreeRTOS is the RTOS used on the Photon and the Electron. This library
adds FreeRTOS support to the Core.

This opens up the possibility of using threads, queues, timers, pre-emptive and co-operative
multitasking and all the other goodies FreeRTOS provides.

The library adds between 3-9K to the flash binary size. For some projects,this
is not an issue, while others need all the flash space they can get, which is why this isn't
included as standard in the Core firmware, but made available as an optional extra.

The version of FreeRTOS bundled with this library is v8.2.2

# Compatibility

Please note that code that uses FreeRTOS functions directly will not be compatible with the
Photon or the Electron, since these platforms wrap FreeRTOS into higher level libraries.
In time, we will provide high-level support for all FreeRTOS features so that code
can be written to be compatible across all Particle devices, and independent from
the underlying RTOS.

