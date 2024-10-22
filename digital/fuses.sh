#!/bin/bash
set -x

avrdude -p m328p -c arduino -P /dev/ttyACM0 -b 19200 -U lfuse:r:-:b -U hfuse:r:-:b -U efuse:r:-:b

