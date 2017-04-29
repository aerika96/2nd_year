cmake_minimum_required(VERSION 3.6)
project(thread1)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(thread1 ${SOURCE_FILES})