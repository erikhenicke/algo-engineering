#include <iostream>
#include "coding-warmup/coding_warmup.h"
#include "exam-exercises/exam_exercises.h"
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

int main() {
  cout << string(80, '#') << endl;
  cout << "Coding warmup:" << endl;
  cout << string(80, '#') << endl;
  run_merge_sort();
  run_merge();

  cout << '\n' << string(80, '#') << endl;
  cout << "Benchmarking:" << endl;
  cout << string(80, '#') << endl;
  run_benchmark();
}
