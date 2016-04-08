
Example integration for controlbox

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


