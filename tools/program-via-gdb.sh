#!/bin/sh

set -m
st-util -p 9025 2>&1 &

sleep 5

arm-none-eabi-gdb ../platform/spark/target/brewpi.elf -x gdb-program.txt

