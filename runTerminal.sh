#!/bin/sh
# This will tell the example to scale to match the terminal size
./bin/srTerm $(expr $(tput cols) / 2) $(expr $(tput lines) - 1)