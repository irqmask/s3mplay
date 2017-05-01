cmake_minimum_required (VERSION 2.8)
project (s3m)

set (CMAKE_BINARY_DIR ../../bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#if you don't want the full compiler output, remove the following line
set(CMAKE_VERBOSE_MAKEFILE ON)

set (CMAKE_MODULE_PATH ../src)

INCLUDE(FindPkgConfig)
find_package(SDL2TTF)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

add_subdirectory (libs3m)

add_subdirectory (s3mplay)
