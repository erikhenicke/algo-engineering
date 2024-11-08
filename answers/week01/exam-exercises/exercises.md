
## Task 1
**Select one slide from the lecture, research more about the topic, and report on it.**

### Is OpenMP composable?
The programming model OpenMP was introduced first for C/C++ in 1998 and works very effectively for single level parallelism. However, it does not support composable nested parallelism, which is desirable for building more scalable applications. OpenMP allows a number of OpenMP constructs to be composed in a series, but OpenMP nesting can lead to overhead and resource consumption, resulting in program termination. This also prohibits the use of libraries containing parallel code within OpenMP constructs, as nested execution can lead to exhaustion. 
To overcome these problems, the TBB (Threading Building Blocks) programming model, which is also widely known in the C++ community, can be used. TBB dynamically schedules parallel tasks and allows nesting. It is considered more composable than OpenMP. However, full compatibility cannot be achieved by any model, as it would then have to work perfectly with all other parallel programming models.  

Sources: 
- Voss, M., Asenjo, R., & Reinders, J. (2019). Pro TBB. Apress. https://doi.org/10.1007/978-1-4842-4398-5, (Chapter 1: Jumping Right In: "Hello, TBB!" ,Chapter 9: The Pillars of Composability)
- https://en.wikipedia.org/wiki/OpenMP

## Task 2 
**Discuss two things for Chapter 1 you find particularly interesting.**

### Hyperthreading
In order to keep improving performance through hardware, multicore processors have become commonplace. Multicore processors consist of several CPU's each with its own L1 and L2 caches. Hyperthreading makes it possible for one single CPU to execute multiple flows of control. By keep multiple copies of program counters and register files, it can perform context shifts between threads on a cycle-by-cycle basis.
Hyperthreading enables better management of the processing resources. Time otherwise paused for data loading can thus be used to proceed with the execution of a different thread. In practice most commonly two threads reside on one CPU, doubling the thread-level parallelism.

### SIMD
SIMD stands for single-instruction, multiple-data parallelism and refers to the ability of many modern processors to execute several operations in parallel with a single instruction. Intel's AVX-512 SIMD instructions process 512 bits of data at once. These instructions can be used especially for processing images, sound or video data. To reliably apply SIMD instructions special vector data types supported by e.g. GCC must be used. 

