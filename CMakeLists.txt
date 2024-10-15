cmake_minimum_required(VERSION 3.29)
project(algo_engineering)

find_package(OpenMP REQUIRED)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        answers/pi_approximation.cpp
        answers/main.cpp
        answers/pi_approximation.h
)

if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
endif()
