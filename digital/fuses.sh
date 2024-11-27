#!/bin/bash
set -x

avrdude -p m328p -c arduino -P /dev/ttyACM0 -b 19200 -U lfuse:r:-:b -U hfuse:r:-:b -U efuse:r:-:b

# to write fuses
# avrdude -p m328p -c arduino -P /dev/ttyACM0 -b 19200 -U lfuse:w:0xFF:m -U hfuse:w:0xDA:m -U efuse:w:0xFD:m
# replace hex values with hex values of choice

