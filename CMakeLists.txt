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

# Set install location
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")

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
file(GLOB KM_SOURCES "kazmath/*.c")
set(SOURCES ${SOURCES} ${KM_SOURCES})

# sr
file(GLOB SR_SOURCES "sr/*.c")
set(SOURCES ${SOURCES} ${SR_SOURCES})

# main
file(GLOB MAIN_SOURCES "*.c")
set(SOURCES ${SOURCES} ${MAIN_SOURCES})

# Executable
add_executable(sr ${SOURCES})
