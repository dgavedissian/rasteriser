# This file is part of the rasteriser project
# Licensing information can be found in the LICENSE file
# (C) 2014 David Avedissian. All rights reserved.
cmake_minimum_required(VERSION 2.8)
project(rasteriser)

# Include modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

# kazmath
file(GLOB KAZMATH_SRC "src/sr/kazmath/*.c")

# sr
set(SR_SRC
  src/sr/srColour.c
  src/sr/srContextSDL.c
  src/sr/srContextTerm.c
  src/sr/srFrameBuffer.c
  src/sr/srMain.c
  src/sr/srRasteriser.c)

# Dependencies
find_package(SDL REQUIRED)
set(LIBS ${SDL_LIBRARY})

# Ensure we build in gnu99 mode and include the math library
if(${CMAKE_C_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
  set(LIBS ${LIBS} m)
endif()

# sr library
include_directories(${SDL_INCLUDE_DIR})
add_library(sr ${SR_SRC} ${KAZMATH_SRC})
target_link_libraries(sr ${LIBS})

# Example
add_executable(example src/main.c)
target_link_libraries(example sr)

