# Task 1
**When run with one thread per logical core, threads from Listing 3.1 display their numbers randomly, while those from 
Listing 3.2 show them in the same order. Explain why.**

The compute time for each thread in Listing 3.1 is equally minimal. They are likely to reach the `printf` statement at
the same time. Since the order of execution is not synchronized, the output of the thread ID's is random.

Even though the order of thread execution is also not controlled in Listing 3.2, the results always appear in the same
order. This is due to the fact that the computation of increasing Fibonacci numbers requires additional time per 
increment, which determines the output order if the program starts with the 45th Fibonacci number.

# Task 2
**What do you think, does the code snippet in Listing 3.10 use the cores efficiently, why yes, why no? Consider also the
variable size in your argumentation.**

The code snippet for Conway’s Game of Life iterates over through the generations using a while loop. While the loop can
be rewritten as a for loop, the different iterations depend on each other making it difficult to parallelize. 
A disadvantage of the implementation is that each time a new generation is computed, the old threads are terminated and 
new threads have to be created. I would guess that reusing the threads improves the performance.
The code snippet parallelizes each generation, since the computation of each pixel is independent of the other 
pixels. This is beneficial when the computed panel is larger than 200 in each dimension. The parallelization overhead
however outweighs the gains for smaller sizes. 
The code can further be improved, if the collapse statements is removed from the pragma, reducing the amount of 
parallelization management. 

# Task 3
**From Lecture 1, take your parallelized version of pi_monte_carlo.cpp and modify it based on the insights from 
Example 3.5 on computing $\pi$ using random shooting, particularly the random number generation method. Report the 
changes you do and their impact on performance.**

Exchanging the default random engine from c++ through the `rnd` function from chapter 3, speeds up the program by factor
of almost 10. The `rnd` function implements a simple linear congruential generator with a period for all seed values of 
$2^24 = 16777216$ (see [here](https://en.wikipedia.org/wiki/Linear_congruential_generator)). It involves only
a few simple operations like one bitshift, one addition as well as one multiplication.
Because the period size of $16777216$ this is not sufficient for larger loops, one can use different configurations to 
enlarge the period, e.g. $m = 2^31, a = 1103515245, c = 12345$ used in glibc by the GCC. This configuration also comes 
with the performance gains.
Another change is the introduction of private seed variables for each thread, to compute the random numbers. 
Implementing private seeds as thread local variables avoids false sharing, even though the performance differs not 
significantly. 
Last but not least, computing the final result using `std::log` and `std::exp` to avoid the division by a large number,
but also doesn't increase accuracy noticeably. 

