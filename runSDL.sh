#!/bin/sh
(mkdir build; cd build; cmake ..; make)
./bin/srSDL 640 480
