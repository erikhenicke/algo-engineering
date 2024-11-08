cmake_minimum_required(VERSION 3.29)
project(algo_engineering C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3 -ffast-math -std=c++14 -march=native")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -O3 -ffast-math -std=c++14 -march=native")

add_subdirectory("answers/week01")
add_subdirectory("answers/week02")
add_subdirectory("answers/week03")
add_subdirectory("answers/week04")
