cmake_minimum_required(VERSION 3.12)
project(set)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lpthread")

add_executable(set_test gtest/gtest.h gtest/gtest-all.cc gtest/gtest_main.cc
        set.cpp set.hpp counted.cpp counted.hpp fault_injection.cpp fault_injection.hpp test.inl)

add_executable(set_me set.hpp set.cpp main.cpp)

target_link_libraries(set_test pthread)