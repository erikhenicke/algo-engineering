//
// Created by erik on 28.10.24.
//

#include <iostream>
#include <iomanip>
#include <omp.h>
#include "coding_warmup.h"

using namespace std;

void pi_numerical_integration(int num_steps) {
  double width = 1.0 / double(num_steps);
  double sum = 0.0;

  double start_time = omp_get_wtime();
# pragma omp parallel for schedule(dynamic, num_steps / 100 / omp_get_num_threads()) reduction(+ : sum) // this one-liner enables parallelism
  for (int i = 0; i < num_steps; i++) {
    double x = (i + 0.5) * width;
    sum = sum + (1.0 / (1.0 + x * x));
  }
  double pi = sum * 4 * width;
  double run_time = omp_get_wtime() - start_time;

  cout << "pi with " << num_steps << " steps is " << setprecision(17)
       << pi << " in " << setprecision(6) << run_time << " seconds\n";
}