@ECHO OFF
ECHO PROGRAMMING!!!
dfu-suffix -d 1d50:607f -a ..\..\spark-firmware\build\target\main\prod-0\brewpi.bin
dfu-util -d 1d50:607f -a 0 -s 0x08005000:leave -D ..\..\spark-firmware\build\target\main\prod-0\brewpi.bin
pause