//
// Created by erik on 20.11.24.
//

#include "../include/fizzbuzz.h"
#include <string>
#include <iostream>

std::string fizzbuzz(int n) {
  std::string result;
  int fizz = n % 3;
  int buzz = n % 5;
  if (fizz == 0) {
    result.append("fizz");
  }
  if (buzz == 0) {
    result.append("buzz");
  }
  if (result.empty()) {
    return std::to_string(n);
  }
  return result;
}



