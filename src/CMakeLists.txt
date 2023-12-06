cmake_minimum_required (VERSION 3.5)
project (s3m)

set (CMAKE_BINARY_DIR ../../bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_VERBOSE_MAKEFILE ON)

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif (WIN32)

find_package(PkgConfig QUIET)
pkg_check_modules(PKG_SDL2 QUIET sdl2)

find_path(SDL2_INCLUDE_DIR NAME SDL.h HINTS ${PKG_SDL2_INCLUDE_DIRS} ${PKG_SDL2_INCLUDEDIR} PATH_SUFFIXES SDL2)
find_library(SDL2_LIBRARY NAME SDL2 HINTS ${PKG_SDL2_LIBRARY_DIRS} ${PKG_SDL2_LIBDIR} PATH_SUFFIXES x64 x86)

add_subdirectory (libs3m)
add_subdirectory (s3mplay)
