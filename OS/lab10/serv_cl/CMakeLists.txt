cmake_minimum_required(VERSION 3.6)
project(serv_cl)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c server.c client.c)
add_executable(serv_cl ${SOURCE_FILES})