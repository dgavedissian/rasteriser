#!/bin/sh
# This will tell the example to scale to match the terminal size
./bin/sr $(expr $(tput cols) / 2) $(expr $(tput lines) - 1)