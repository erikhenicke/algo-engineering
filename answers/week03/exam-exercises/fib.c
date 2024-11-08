//
//Createdbyerikon03.11.24.
//

#include <stdio.h>
#include <omp.h>

long fib(int n) {
  return (n < 2 ? 1 : fib(n - 1) + fib(n - 2));
}

void fibs_from(int n) {
#pragma omp parallel num_threads(4)
  {
    int t = omp_get_thread_num();
    printf("%d: %ld\n", t, fib(n + t));
  }
}