
## Building Brewpi for your Particle Device

From the root of this repo
```
cd platform/spark
make APP=cbox PLATFORM=photon
```
To flash, put the device in DFU mode, and run the same command with `program-dfu` added.


## Building Brewpi as a cross-compiled app
From the root of this repo
```
cd platform/spark
make APP=cbox PLATFORM=gcc
```
The cross-compiled app should be built with gcc 5. gcc 6 does not work.
You can easily do this by installing the packages gcc-5 and g++-5, and then running:
```
make CC=gcc-5 CXX=g++-5 PLATFORM=gcc APP=cbox
```

The resulting executable is  found at `target/cbox-gcc/cbox`.

The cross compiled app is built on Particle's cross compile support for their firmware. To see options, run `cbox -h`. 

Since the cbox app doesn't presently connect to the cloud, most of the options aren't relevant here. The executable still requires device keys, but these can be simply empty files. The device ID is needed. This is either passed as a command line parameter, or stored in a config file:

```
touch device_key.der
touch server_key.der
./cbox -i 112233445566778899AABBCC
```

You should then see:

```
["a":"brewpi","v":"0.3.0"]
```

### Example interaction

For demo/testing - the system container provides the device ID and system time. 

Starting the executable responds with an app name and version string

```
./cbox -i 112233445566778899AABBCC
["a":"brewpi","v":"0.3.0"]
```

You can then issue a command:

```
0F 00
```

which responds with

```
0F 00 [replay command] 00 [type] 00 [success] 0C [length] 11 22 33 44 55 66 77 88 99 AA BB CC 
```

The command `0f 00` means read from the system container, object id 00. (There is an implicit additional '00' for the type, which means don't care.) This is where the device ID is stored. The response is the original (filled out) command `0f 00 00` followed by `00` (success), then `0C` (12 bytes of data) and finally the 12 id bytes. 

The system time can also be retrieved:

```
0f 01
0F 01 00 [cmd replay] 00 [ok response] 06 [6 bytes] 01 09 00 00 [timestamp in ms - little endian] 01 00 [time factor - 0x01 - normal.] 
```
## Enumerating the default onewire bus

To list the addresses of devices on the bus

```
10 [system object write] 02 [id] 00 [type] 01 [data-len] 02 [data]
```

The device responds with the same command, plus `00` to acknowledge the request, and then the 8-bytes for each address discovered on the one wire bus. 

For example, with two temperature sensors connected, the response is

```
10 02 00 01 02 [echo command] 00 [success result] 28 FE D8 73 06 00 00 EE [first address] 28 ED C9 73 06 00 00 5D [second address]
```

# Creating Objects

Before creating new objects, the system needs to have a profile created and activated. 

## Creating and activating a profile

First confirm there are no profiles defined

```
0E [list profiles]
```

which should respond with

```
0E [command echo] FF [no active profile] [empty list of profiles]
```

Then create a profile

```
07 [create profile]
```

which gives the response

```
07 [create profile] 00 [profile created with id 0]
```

The profile is created, but not yet active:

```
0e [list profiles]
```

gives 

```
07 [list profiles] FF [no active profile] 00 [profile id 0]
```

The profile is activated by

```
09 [activate profile] 00 [index]
``

which responds with

```
09 [activate profile] 00 [index] 00 [success]
```

With the profile active, you can now proceed to create objects. 
The created profile and making it active are persistent operations, so the device will be in the same state when power is cycled.

## Temperature sensors

### Installing 

A new temperature sensor is installed via the Create Object command.
Be sure to install it to a new slot (use the find-slot command if needed.)

```
03 [create object] 00 [id-chain] 06 [temp sensor] 08 [size] 28 ED C9 73 06 00 00 5D [address]
```
03 [create object] 01 [id-chain] 06 [temp sensor] 08 [size] 28 FE D8 73 06 00 00 EE

If successful, the system appends `00` to the command, indicating success.

You can check the object is stored in the profile by the list profile command:

```
05 [list profile] 00 [profile 0]
```

responds with

```
05 [list profile] 00 [profile 0] 03 [create object] 00 [slot 0] 06 [type] 08 [8-bytes of data] 28 ED C9 73 06 00 00 5D [onewire address]

```

### Reading the temperature value

```
01 [read] 00 [id] 
```

responds with

```
01 [read] 00 [id] 00 [success] 00 [type] 00 [datalen=unknown] 01 [connected] 80 17 00 00 [temperature]
```

The temperature is in little-endian format, so the value is 0x00001780.
This is a fixed point number, scaled by 256, so divide by 256 to get the value of 23.625. 



## Protocol Docs

https://raw.githubusercontent.com/BrewPi/brewpi-developerdocs/develop/source/brewpi-embedded.rst



## Example integration for controlbox

On a fresh device, run these commands:

```
07 	    # create profile
[ 00 ] # profile 00 created

09 00   # Activate profile 00
[ 00 ] # succesfully activated profile 00

0E   # list profiles
[ 00 00 ] profile 00 active, profiles available 00

03 00 01 00  # create object at index 00, type 01 (Ticks), data len 00
[ 00 ] # object created

05 00  # list objects defined in profile 00
[ 03 00 01 00 ]  # lists the object just created (03 starts each object definition)

0A 00  # dump all values in the current profile
[ 00 04 F1 5F 09 00 ]  # index 00, value F1 5F 09 00, which is little endian notation for 0x00095FF1, the time the device has been online in milliseconds. 
```

## Application objects

### OneWire

The OneWire object provides a one-wire bus. The application instantiates this as part of the board configuration.

