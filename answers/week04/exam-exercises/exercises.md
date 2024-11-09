# Task 1
**Provide details of your benchmarking environment, including the Operating System, RAM, CPU, and Compiler.**

The system which is used for benchmarking runs Arch Linux and is provided with 2 * 8 GB of RAM and 20 logical cores 
(6 performance cores with Hyperthreading enabled and 8 efficiency cores). For compiling the GCC 14.2.1 compiler is used.

Compilation flags: `-fopenmp -Ofast -ffast-math -std=c++17 -march=native`

# Task 2
**Explain any patterns or trends observed in the graphs, and offer potential reasons for these findings.**

**Varying the number of threads:** The relative speedup of the custom `min_max_quicksort` implementation increases 
steadily with the number of threads used, but with a decreasing slope. In contrast, the `__gnu_parallel::sort` 
implementation increases it's speedup almost linearly. The speedup rises with a steep linear slope until five threads 
are used, followed by a short stagnation. Using seven threads and more leads to a further linear increase in speedup, 
with only a decrease when the last logical core is used. 
In comparison, the custom implementation with a maximum speedup of five is clearly inferior to the `__gnu_parallel` 
implementation, which has a maximal speedup of over ten compared to `std::sort`.
// adding interpretation

**Varying sizes:**   
// description

