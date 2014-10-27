#!/bin/sh
# This will tell the example to scale to match the terminal size
(mkdir build; cd build; cmake ..; make)
./bin/srTerm $(expr $(tput cols) / 2) $(expr $(tput lines) - 1)
