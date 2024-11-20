cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(lodepng LANGUAGES CXX)

add_library(lodepng STATIC include/lodepng.h src/lodepng.cpp)

target_include_directories(lodepng PUBLIC include)