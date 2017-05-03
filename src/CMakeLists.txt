cmake_minimum_required (VERSION 2.8)
project (s3m)

set (CMAKE_BINARY_DIR ../../bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#if you don't want the full compiler output, remove the following line
set(CMAKE_VERBOSE_MAKEFILE ON)

#add_definitions(-Wall)
if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif (WIN32)

find_file(SDL2_INCLUDE_DIR NAME SDL.h HINTS SDL2)
find_library(SDL2_LIBRARY NAME SDL2)

add_subdirectory (libs3m)
add_subdirectory (s3mplay)
