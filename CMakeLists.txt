cmake_minimum_required(VERSION 3.12)
project(set)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lpthread")

add_executable(set set.cpp set.hpp)