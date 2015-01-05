#!/bin/bash
if (cd build && make); then
  build/example
fi
