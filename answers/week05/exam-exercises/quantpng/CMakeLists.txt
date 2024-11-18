cmake_minimum_required(VERSION 3.29)
project(quantpng LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lodepng)
add_subdirectory(quantize)

add_executable(quantpng main.cpp)

target_link_libraries(quantpng PUBLIC lodepng)
target_link_libraries(quantpng PUBLIC quantize)

