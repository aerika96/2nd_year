cmake_minimum_required(VERSION 3.6)
project(execute_command)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(execute_command ${SOURCE_FILES})