
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
The resulting executable is  found at `target/cbox-gcc/cbox`.

The cross compiled app is build on Particle's cross compile support for their firmware. To see options, run `cbox -h`. 

Since the cbox app doesn't presently connect to the cloud, most of the options aren't relevant here. The executable still requires device keys, but these can be simply empty files. The device ID is needed. This is either passed as a command line parameter, or stored in a config file:

```
touch device_key.der
touch server_key.der
./cbox -i 000000000000000000000001
```

You should then see:
```
["a":"brewpi","v":"0.3.0"]
```

### Example interaction

For demo/testing - the system container provides the device ID and system time. 

```
./cbox -i 112233445566778899AABBCC
["a":"brewpi","v":"0.3.0"]
0f 00
0F 00 00 0C 11 22 33 44 55 66 77 88 99 AA BB CC 
```

The command `0f 00` means read from the system container, object id 00. This is where the ID is stored. The response is the original command `0f 00` followed by `00` (success), then `0C` (12 bytes of data) and finally the 12 id bytes. 

The system time can also be retrieved:

```
0f 01
0F 01 [cmd] 00 [ok response] 06 [6 bytes] 01 09 00 00 [timestamp in ms - little endian] 01 00 [time factor - 0x01 - normal.] 
```


## Protocol Docs

https://raw.githubusercontent.com/BrewPi/brewpi-developerdocs/develop/source/brewpi-embedded.rst



## Example integration for controlbox

On a fresh device, run these commands:

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


