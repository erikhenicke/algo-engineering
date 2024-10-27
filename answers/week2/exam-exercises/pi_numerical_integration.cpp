//
// Created by erik on 27.10.24.
//

#include "pi_numerical_integration.h"
#include <omp.h>
#include <iomanip>
#include <iostream>

using namespace std;

void pi_numerical_integration(int num_steps) {
  double start_time = omp_get_wtime();

  double width = 1.0 / double(num_steps);
  double sum = 0.0;
#pragma omp parallel
  {
    double sum_local = 0.0;
#pragma omp for schedule(dynamic, 10000)
    // dynamic and static schedules fetching chucks of size 10000 are performing
    // equally well, even though my system carries performance and efficient
    // cores
    for (int i = 0; i < num_steps; ++i) {
      double x = (i + 0.5) * width;
      sum_local = sum_local + (1.0 / (1.0 + x * x));
    }
#pragma omp atomic
    sum += sum_local;
  }
  double pi = sum * 4 * width;
  double run_time = omp_get_wtime() - start_time;

  cout << "Numerical integration with #pragma omp for:" << endl;
  cout << "pi: " << setprecision(17) << pi << endl;
  cout << "run_time: " << setprecision(6) << run_time << " s" << endl;
  cout << "number of steps: " << num_steps << endl << endl;
}
