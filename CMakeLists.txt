cmake_minimum_required(VERSION 3.29)
project(algo_engineering C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF) # disable compiler specific extensions
set(CMAKE_CXX_STANDARD_REQUIRED ON) # require C++ standard 17

find_package(OpenMP REQUIRED)
link_libraries(OpenMP::OpenMP_CXX)
link_libraries(OpenMP::OpenMP_C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -ffast-math -march=native")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -ffast-math -march=native")

set(Python3_EXECUTABLE /usr/bin/python3)
find_package(Python3 3.12 EXACT COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python3_NumPy_INCLUDE_DIRS})

set(MATPLOTLIB_CPP_INCLUDE_DIRS /usr/local/vcpkg/buildtrees/matplotlib-cpp/src/a4e571e4d1-7c78af1a8c.clean)

add_subdirectory("answers/week01")
add_subdirectory("answers/week02")
add_subdirectory("answers/week03")
add_subdirectory("answers/week04")

target_include_directories(week04 PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS}
        ${MATPLOTLIB_CPP_INCLUDE_DIRS}
)
target_link_libraries(week04 PRIVATE ${Python3_LIBRARIES})

add_subdirectory("answers/week05")
