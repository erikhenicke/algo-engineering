#include "pi_approximation.h"
#include <omp.h>
#include <iomanip>
#include <iostream>
#include <random>

using namespace std;

void pi_numerical_integration(int num_steps) {
  double width = 1.0 / double(num_steps);  // width of a rectangle
  double sum = 0.0;  // for summing up all heights of rectangles

  double start_time = omp_get_wtime();  // wall clock time in seconds
  for (int i = 0; i < num_steps; i++) {
    double x = (i + 0.5) * width;       // midpoint
    sum = sum + (1.0 / (1.0 + x * x));  // add new height of a rectangle
  }
  double pi = sum * 4 * width;  // compute pi
  double run_time = omp_get_wtime() - start_time;

  cout << "Numerical integration:" << endl;
  cout << "pi: " << setprecision(17) << pi << endl;
  cout << "run_time: " << setprecision(6) << run_time << " s" << endl;
  cout << "number of steps: " << num_steps << endl << endl;
}

void pi_numerical_integration_parallel(int num_steps) {
  double start_time = omp_get_wtime();  // wall clock time in seconds

  double width = 1.0 / double(num_steps);  // width of a rectangle
  double sum = 0.0;  // for summing up all heights of rectangles
#pragma omp parallel
  {
    int num_threads = omp_get_num_threads();  // used as increment in for loop
    int thread_id = omp_get_thread_num();
    double sum_local = 0.0;  // for summing up heights locally
    for (int i = thread_id; i < num_steps; i += num_threads) {
      double x = (i + 0.5) * width;                   // midpoint
      sum_local = sum_local + (1.0 / (1.0 + x * x));  // add new height
    }
#pragma omp atomic  // only one thread performs the update of the sum
    sum += sum_local;
  }
  double pi = sum * 4 * width;  // compute pi
  double run_time = omp_get_wtime() - start_time;

  cout << "Numerical integration in parallel:" << endl;
  cout << "pi: " << setprecision(17) << pi << endl;
  cout << "run_time: " << setprecision(6) << run_time << " s" << endl;
  cout << "number of steps: " << num_steps << endl << endl;
}

void pi_monte_carlo(int n) {
  unsigned int seed = 0;
  default_random_engine re{seed};
  uniform_real_distribution<double> zero_to_one{0.0, 1.0};

  int counter =
      0;  // counter for points lying in the first quadrant of a unit circle
  auto start_time =
      omp_get_wtime();  // omp_get_wtime() is an OpenMP library routine

  // compute n points and test if they lie within the first quadrant of a unit circle
  for (int i = 0; i < n; ++i) {
    auto x = zero_to_one(re);  // generate random number between 0.0 and 1.0
    auto y = zero_to_one(re);  // generate random number between 0.0 and 1.0
    if (x * x + y * y <=
        1.0) {  // if the point lies in the first quadrant of a unit circle
      ++counter;
    }
  }

  auto run_time = omp_get_wtime() - start_time;
  auto pi = 4 * (double(counter) / n);

  cout << "Monte Carlo approximation:" << endl;
  cout << "pi: " << pi << endl;
  cout << "run_time: " << run_time << " s" << endl;
  cout << "n: " << n << endl << endl;
}

void pi_monte_carlo_parallel(int n) {

  // counter for points lying in the first quadrant of a unit circle
  int counter = 0;
  auto start_time =
      omp_get_wtime();  // omp_get_wtime() is an OpenMP library routine

#pragma omp parallel
  {
    int thread_id = omp_get_thread_num();
    int num_threads = omp_get_num_threads();
    default_random_engine re{(unsigned int)thread_id};
    uniform_real_distribution<double> zero_to_one{0.0, 1.0};

    int local_counter = 0;

    // compute n / num_threads points and test if they lie within the first
    // quadrant of a unit circle
    for (int i = thread_id; i < n; i += num_threads) {
      auto x = zero_to_one(re);  // generate random number between 0.0 and 1.0
      auto y = zero_to_one(re);  // generate random number between 0.0 and 1.0
      if (x * x + y * y <=
          1.0) {  // if the point lies in the first quadrant of a unit circle
        ++local_counter;
      }
    }
#pragma omp atomic
    counter += local_counter;
  }

  auto run_time = omp_get_wtime() - start_time;
  auto pi = 4 * (double(counter) / n);

  cout << "Monte Carlo approximation in parallel:" << endl;
  cout << "pi: " << pi << endl;
  cout << "run_time: " << run_time << " s" << endl;
  cout << "n: " << n << endl << endl;
}
