#!/bin/sh

export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

/bin/date -s "2019-01-01 00:00:00"
./gdb nvr_demo