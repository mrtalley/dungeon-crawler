cmake_minimum_required(VERSION 3.6)
project(1_04)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES dungeon.c heap.c path.c utils.c dims.h macros.h rlg327.c endian.h)
add_executable(1_04 ${SOURCE_FILES})