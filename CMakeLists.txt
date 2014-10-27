# This file is part of the rasteriser project
# Licensing information can be found in the LICENSE file
# (C) 2014 David Avedissian. All rights reserved.
cmake_minimum_required(VERSION 2.8)
project(rasteriser)

# Include modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

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
file(GLOB KAZMATH_SRC "src/kazmath/*.c")
set(SRC ${SRC} ${KAZMATH_SRC})

# sr
set(SR_SRC
  src/sr/srColour.c
  src/sr/srFrameBuffer.c
  src/sr/srRasteriser.c)
set(SRC ${SRC} ${SR_SRC})

# main
set(MAIN_SRC src/main.c)
set(SRC ${SRC} ${MAIN_SRC})

# Find SDL
find_package(SDL)

# SDL based contex
if(SDL_FOUND)
  include_directories(srSDL ${SDL_INCLUDE_DIR})
  add_executable(srSDL ${SRC} src/sr/srContextSDL.c)
  target_link_libraries(srSDL ${SDL_LIBRARY})
  if(CMAKE_COMPILER_IS_GNUCC)
    target_link_libraries(srSDL m)
  endif()
endif()

# Terminal based context
add_executable(srTerm ${SRC} src/sr/srContextTerminal.c)
if(CMAKE_COMPILER_IS_GNUCC)
  target_link_libraries(srTerm m)
endif()

