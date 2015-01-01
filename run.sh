#!/bin/sh
if (cd build && make); then
  build/example
fi
