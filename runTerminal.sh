#!/bin/sh
# This will tell the example to scale to match the terminal size
(mkdir build; cd build; cmake ..; make)
./bin/srTerm 640 480
