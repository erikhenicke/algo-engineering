cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(tests_fizzbuzz LANGUAGES CXX)

add_subdirectory(fizzbuzz)

add_subdirectory(catch)

add_executable(fizzbuzz_tests catch_main.cpp catch_tests_fizzbuzz.cpp)

target_link_libraries(fizzbuzz_tests PRIVATE fizzbuzz PRIVATE catch)

enable_testing()
add_test(NAME FIZZBUZZ_TESTS COMMAND fizzbuzz_tests)
