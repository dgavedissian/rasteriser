#!/bin/sh
# This will tell the example to scale to match the terminal size
if (cd build && cmake .. && make); then
  ./bin/srTerm
fi
