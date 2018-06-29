//
//  Power.cpp
//  Быстрое возведение в степень.
//
//  Дано число a и неотрицательное целое число n. Найти a^n
//
//  Оценка
//  Время работы T(n) = O(log(n))
//  Объем доп.памяти M(n) = O(1)
//
//  Copyright © 2018 lostmap. All rights reserved.
//
#include <assert.h>
#include <iostream>
double Power(double a, int n) {
  assert(n >= 0);
  double result = 1;
  double a_in_power_of_2 = a;
  while (n > 0) {
    if ((n & 1) == 1) {
      result *= a_in_power_of_2;
    }
    a_in_power_of_2 *= a_in_power_of_2;
    n = n >> 1;
  }
  return result;
}
int main() {
  int a = 0, n = 0;
  std::cin >> a >> n;
  std::cout << Power(a, n) << std::endl;
  return 0;
}


