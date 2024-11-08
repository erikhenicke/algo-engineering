//
// Created by erik on 08.11.24.
//

#include <omp.h>
#include <iostream>
#include <random>
#include "exam_exercises_cpp.h"

using namespace std;

double rnd_book(unsigned int &seed) {
  seed = (1140671485 * (seed) + 12820163) % (1 << 24);
  return (double)seed / (1 << 24);
}

double rnd(unsigned int &seed) {
  seed = (1103515245 * (seed) + 12345) % (1 << 31);
  return (double)seed / (1 << 31);
}

void pi_monte_carlo_old(long n) {

  int counter = 0;
  auto start_time = omp_get_wtime();

#pragma omp parallel
  {
    int thread_id = omp_get_thread_num();
    int num_threads = omp_get_num_threads();
    default_random_engine re{(unsigned int)thread_id};
    uniform_real_distribution<double> zero_to_one{0.0, 1.0};

    int local_counter = 0;

    for (int i = thread_id; i < n; i += num_threads) {
      auto x = zero_to_one(re);
      auto y = zero_to_one(re);
      if (x * x + y * y <= 1.0) ++local_counter;

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


void pi_monte_carlo_fast_rnd (long n) {
  unsigned int seed = time(nullptr);
  const int max_num_threads = omp_get_max_threads();
  std::vector<unsigned int> seeds(max_num_threads);

  for (int thread = 0; thread < max_num_threads; ++thread) {
    seeds[thread] = seed + thread;
  }

  int counter = 0;
  const double start_time =
      omp_get_wtime();

#pragma omp parallel for reduction(+:counter)
    for (int i = 0; i < n; i++) {
      int thread_id = omp_get_thread_num();
      auto x = rnd(seeds[thread_id]);
      auto y = rnd(seeds[thread_id]);
      if (x * x + y * y <= 1.0) ++counter;
    }

  double run_time = omp_get_wtime() - start_time;
  double pi = 4 * (double(counter) / n);

  cout << "Monte Carlo approximation in parallel:" << endl;
  cout << "pi: " << pi << endl;
  cout << "run_time: " << run_time << " s" << endl;
  cout << "n: " << n << endl << endl;
}

void pi_monte_carlo_no_false_sharing (long n) {
  const unsigned int seed = time(nullptr);

  int counter = 0;
  const double start_time =
      omp_get_wtime();

#pragma omp parallel
  {
    unsigned int thread_seed = seed + omp_get_thread_num();
#pragma omp for reduction(+:counter)
    for (int i = 0; i < n; i++) {
      auto x = rnd(thread_seed);
      auto y = rnd(thread_seed);
      if (x * x + y * y <= 1.0) ++counter;
    }
  }

  double run_time = omp_get_wtime() - start_time;
  double log_pi = log(4) + log(counter) - log(n);
  double pi = exp(log_pi);

  cout << "Monte Carlo approximation in parallel:" << endl;
  cout << "pi: " << pi << endl;
  cout << "run_time: " << run_time << " s" << endl;
  cout << "n: " << n << endl << endl;
}


