# Task 1
**Provide details of your benchmarking environment, including the Operating System, RAM, CPU, and Compiler.**

The system which is used for benchmarking runs Arch Linux and is provided with 2 * 8 GB of RAM and 20 logical cores 
(6 performance cores with Hyperthreading enabled and 8 efficiency cores). For compiling the GCC 14.2.1 compiler is used.

Compilation flags: `-fopenmp -Ofast -ffast-math -std=c++17 -march=native`

# Task 2
**Explain any patterns or trends observed in the graphs, and offer potential reasons for these findings.**

**Varying the number of threads:** The relative speedup of the custom `min_max_quicksort` implementation increases 
steadily with the number of threads used, but with a decreasing slope. In contrast, the `__gnu_parallel::sort` 
implementation increases it's speedup almost linearly. It's speedup rises with a steep linear slope until four threads 
are used. Using four threads and more leads to a linear increase in speedup with a lower slope, with only a decrease 
when the last logical core is used. 
The `min_max_quicksort` implementation doesn't make use of parallelization in it's partitioning steps. This sequential 
part of the algorithm cannot make use of additional threads and thus prevents a good increase in speedup. The almost 
linear increase in speedup until the use of $20$ threads of the `__gnu_parallel::sort` implementation on the other hand
shows that the implementation parallelizes more parts of the algorithm and thus makes better use of the additional 
threads.

**Varying sizes:** The relative speedup of both implementations starts below one and exceeds the sequential `std::sort`
implementation at an array size between $10^3$ and $10^4$. Until that point the parallelization effects are dominated 
by the overhead created by parallelization. The speedup of `min_max_quicksort` increases steadily but with a decreasing 
slope until the last measure, where the slope increases again. The speedup of `__gnu_parallel:sort` increases linearly 
until the end including with two minor setbacks. Both curves show that the system bounds that slow parallelization down
are not yet reached. Inspecting the curve further was not possible due to the limits of the system memory. 

# Task 3
**Discuss two things you find particularly interesting from "What every systems programmer should know about
concurrency".**

## Read-modify-write operations with LL/SC instructions
Arm lacks dedicated RMW instructions, which are build instead through special `load-link` and `store-conditional` 
(LL/SC). The first instruction reads a value from an address and orders the processor to keep track of it. The 
store-operation continues only to write the modified value if no other stores were made to that address in the meantime.
This way atomic operations can be performed under the guarantee that at least one thread is always making forward 
progress in modifying an address, if several are competing.
In praxis processor monitor load-link addresses only on the level of cache lines, to reduce overhead. This makes rooms
for the existence of the `compare_exchange_weak` operation, that no longer distinguishes between real SC failures, where
the actual address was modified, from spurious SC failures, where only the same cache line got invalidated.

## Consume memory order
In a scenario, where data is rarely written, but often loaded, one wants to relax the standard writer and reader 
synchronization of acquire and release a little bit. The writers thread still needs to keep release memory order, 
meaning that all operations prior to write should be actually performed prior. The readers on the other side don't need
the acquire memory order since all operations performed on the data are dependent on initially loading the data.
This allows the removal of memory barries also on weakly-ordered systems. The consume memory order makes this explicit.
Unfortunately detecting dependency isn't very easy. Thus, compilers are currently changing consume operations to 
acquire operations.