//
// Created by erik on 15.10.24.
//

#include <iostream>
#include <ostream>
#include "pi_approximation.h"

using namespace std;

int main() {
    pi_numerical_integration(100000000);
    pi_numerical_integration_parallel(100000000);
    pi_monte_carlo(10000000);
    pi_monte_carlo_parallel(10000000);
}
