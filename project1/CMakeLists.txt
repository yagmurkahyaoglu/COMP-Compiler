cmake_minimum_required(VERSION 2.8.2.12)
project(project1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(project1 ${SOURCE_FILES})