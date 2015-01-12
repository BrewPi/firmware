#!/bin/sh
# 
# File:   newscript.sh
# Author: Elco
#
# Created on 12-jan-2015, 17:07:35
#
set -m
st-util -p 9025 2>&1 &

sleep 5

arm-none-eabi-gdb ../platform/spark/target/brewpi.elf -x gdbprogram.txt

