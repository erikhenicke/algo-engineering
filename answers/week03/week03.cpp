//
// Created by erik on 28.10.24.
//

extern "C" {
#include "exam-exercises/exam_exercises_c.h"
}
#include "exam-exercises/exam_exercises_cpp.h"
#include "coding-warmup/coding_warmup.h"
#include <iostream>

using namespace std;

int main() {

  cout << "Coding Warmup:" << endl;
  cout << "Own parallelization: ";
  compute_mandelbrot_image();
  cout << "Solution: ";
  compute_mandelbrot_image_solution();
  pi_numerical_integration(1000000000);


  cout << "Exam assignments:" << endl;
  cout << "Random thread order vs ordered computations:" << endl;
  hello_world();
  fibs_from(45);


  cout << "Game of Life:" << endl;
  // On smaller sizes, the parallel overhead outweighs the gains
  game_of_life_sp(100, 10000, 1, false);
  game_of_life_sp(100, 10000, 20, false);

  game_of_life_sp(250, 1000, 1, false);
  game_of_life_sp(250, 1000, 20, false);


  // On larger sizes parallelization increases the performance
  game_of_life_sp(1000, 1000, 1, false);
  game_of_life_sp(1000, 1000, 20, false);

  // double pointers seem to be beneficial
  game_of_life_sp(1000, 10000, 20, false);
  game_of_life_dp(1000, 10000, 20);

  cout << "Monte Carlo:" << endl;
  long n = 1000000000;
  pi_monte_carlo_old(n);
  pi_monte_carlo_fast_rnd(n);
  pi_monte_carlo_no_false_sharing(n);
}
