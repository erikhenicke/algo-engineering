//
// Created by erik on 03.11.24.
//

#include <omp.h>
#include <stdio.h>
#include "exam_exercises_c.h"

void hello_world() {
  printf("Hello, world:");
#pragma omp parallel
  printf(" %d", omp_get_thread_num());
  printf("\n");
}
