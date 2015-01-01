#!/bin/sh
rm -rf build
mkdir build
pushd build
if ["$1" == "debug"]; then
  cmake .. -DCMAKE_BUILD_TYPE=Debug
elif ["$1" == "release"]; then
  cmake .. -DCMAKE_BUILD_TYPE=Release
else
  cmake ..
fi
popd
