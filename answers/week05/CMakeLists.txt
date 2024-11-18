# Notes for lecture 5 regarding CMake:
# 1) Executables are objects of class executable
# - add_executable() is the constructor for executables
# - add_library() is the constructor for libraries
# 2) Objects have member variables (attributes) which can be accessed and changed using member functions
# - e.g. target_sources() sets the sources of a target
# - specific compile flags can be set using target_compile_options()
# - We can also include subdirectories and link libraries.
add_executable(week05
        week05.cpp
)

add_subdirectory(exam-exercises/quantpng)