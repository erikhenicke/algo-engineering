cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(fizzbuzz LANGUAGES CXX)

add_library(fizzbuzz STATIC include/fizzbuzz.h src/fizzbuzz.cpp)

target_include_directories(fizzbuzz PUBLIC include)