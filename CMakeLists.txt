# This file is part of the software-rasteriser project
# Licensing information can be found in the LICENSE file
# (C) 2014 David Avedissian. All rights reserved.

cmake_minimum_required(VERSION 2.8)
project(sr)

# Include modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

# Set default build type
if(CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty so default to
  # building debug symbols
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

# Build in gnu99 mode and link to math library
if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
endif()

# Set install location
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/bin")

# Make sure executables are placed in the install directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_PREFIX})
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_INSTALL_PREFIX})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_INSTALL_PREFIX})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_INSTALL_PREFIX})
endforeach()

# kazmath
file(GLOB KAZMATH_SRC "kazmath/*.c")
set(SRC ${SRC} ${KAZMATH_SRC})

# sr
set(SR_SRC
  sr/srColour.c
  sr/srFrameBuffer.c
  sr/srRasteriser.c)
set(SRC ${SRC} ${SR_SRC})

# main
set(MAIN_SRC main.c)
set(SRC ${SRC} ${MAIN_SRC})

# Find SDL
find_package(SDL REQUIRED)

# Executables
add_executable(srSDL ${SRC} sr/srContextSDL.c)
add_executable(srTerm ${SRC} sr/srContextTerminal.c)
if(CMAKE_COMPILER_IS_GNUCC)
  target_link_libraries(srSDL m)
  target_link_libraries(srTerm m)
endif()
