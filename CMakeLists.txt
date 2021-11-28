cmake_minimum_required(VERSION 3.20)
project(sys C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS -pthread)

add_executable(s server.c)
add_executable(c1 client1.c)
add_executable(c2 client1.c)
