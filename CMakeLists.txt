cmake_minimum_required(VERSION 3.29)
project(algo_engineering)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")#-O3 -ffast-math -std=c++14 -fopenmp -march=native")

add_subdirectory("answers/week1")
add_subdirectory("answers/week2")


