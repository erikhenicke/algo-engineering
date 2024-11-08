//
// Created by erik on 27.10.24.
//

#include <iostream>
#include "coding-warmup/coding_warmup.h"
#include "exam-exercises/pi_numerical_integration.h"

using namespace std;

void coding_warmup() {
  cout << "Fixed race condition executed with 10000 threads:" << endl;
  race_condition();
  cout << "Balanced workload using dynamic scheduling:" << endl;
  unbalanced_workload();
  cout << "Bad schedule is produced by schedule(static, 22):" << endl;
  bad_schedule();
}

int main() {
  coding_warmup();
  pi_numerical_integration(100000000);
}