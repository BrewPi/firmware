
# Example Sessions

Here's some example sessions. The input is given as is, with comments describing
 the meaning of the commands as `[annotations]`. The output enclosed in square brackets (as an annotation) so it's
 completely ignored if it's pasted as input.


## Exit

```
0b 02 [ reset ]
[ 0B 02 00 reset acknowledged ]
```

## Create a new profile

```
0e [ list profiles ]
[ 0E FF - no profile ]
07 [ create profile ]
[ 07 00 - profile created at index 00 ]
09 00 [ activate profile 00 ]
[ 09 00 00 ]
0e [ list profiles ]
[ 0E 00 00 selected profile 00, available profiles 00 ]
```

 ## Create a ScaledTicksValue instance

 ```
03 [create object] 00 [id 0] 01 [type 01] 00 [config data length 0]
[03 00 01 00 00 - result is 00 object created]
 ```

## Read the value of the ScaledTicksValue instance
```
01 00 00 [read object at index 0, unknown size]
[ 01 00 00 <cmd> 06 <size 6> 87 07 1D 00 <current ticks> 01 00 <scale> ]
```

Values are written LSB first:

- Current millis: 0x001d0787
- Scale: 0x0001



 ## List object definitions in profile

```
05 [list objects in profile]
05 00 [profile 0] 03 [create object] 00 [index 0] 01 [type 1] 00 [no config]
```

## List values of objects in profile
```
0A 00 [list all values]
[0A 00 <cmd> 00 <id 0> 06 <len 6> A5 D4 00 00 <millis> 01 00 <scale>]
```

