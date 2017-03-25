cmake_minimum_required(VERSION 3.6)
project(TA)

set(CMAKE_CXX_STANDARD 14)
add_definitions("-DONLINE_JUDGE")

set(SOURCE_FILES problems/67.2.cpp)
add_executable(TA ${SOURCE_FILES})