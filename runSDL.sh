#!/bin/sh
if (cd build && cmake .. && make); then
  ./bin/srSDL
fi
