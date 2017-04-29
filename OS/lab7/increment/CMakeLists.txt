cmake_minimum_required(VERSION 3.6)
project(increment)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(increment ${SOURCE_FILES})