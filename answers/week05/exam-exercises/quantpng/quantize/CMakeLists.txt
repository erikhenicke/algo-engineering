cmake_minimum_required(VERSION 3.29)
project(quantize LANGUAGES C)

file(GLOB_RECURSE SOURCES "src/*.c" "include/*.h")

add_library(quantize STATIC ${SOURCES})

target_include_directories(quantize PUBLIC include)