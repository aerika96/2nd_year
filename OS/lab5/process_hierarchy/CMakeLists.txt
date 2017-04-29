cmake_minimum_required(VERSION 3.6)
project(process_hierarchy)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(process_hierarchy ${SOURCE_FILES})