cmake_minimum_required(VERSION 3.6)
project(two_threads)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(two_threads ${SOURCE_FILES})