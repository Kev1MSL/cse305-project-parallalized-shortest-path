cmake_minimum_required(VERSION 3.21)
project(CSE305_Project)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include_directories(src)

add_executable(CSE305_Project main.cpp )
add_compile_options(-O3)