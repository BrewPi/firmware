# Hardware debugging in NetBeans
On-target hardware debugging requires a hardware debugger (ST-Link V2) and a 
small board to connect the ST-Link to the Spark Core.
You can find details on how to setup NetBeans for debugging here:
https://community.spark.io/t/local-development-and-debugging-a-step-by-step-guide/7829


##
To recompile the project with SWD enabled append this to the make command in Netbeans:

```
USE_SWD=y
```

and do a clean build.

Upload SWD enabled build result via DFU. Put core in DFU mode, run:

```
make program-dfu
```

When starting st-util, you should now see this line:

```
Chip ID is 00000410, Core ID is 1ba01477.
```
If the chip ID comes back as 00000000 you do not have SWD enabled.

# Uploading with with 1 button click

A script is available to do all steps needed.
Run ```program-via-gdb.sh``` from a shell (terminal window in Netbeans).

This will start a gdb server with st-util, connect to it with arm gdb and then load the .elf file.
Make sure st-util is in the PATH.

To be able to upload via the Run button in Netbeans, set the Run command to:
```
./program-via-gdb.sh
```
And the run directory to
```
tools
```
