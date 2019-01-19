cmake_minimum_required(VERSION 3.12)
project(CPPConsoleCalc VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_executable(calc main.cpp math.cpp)
