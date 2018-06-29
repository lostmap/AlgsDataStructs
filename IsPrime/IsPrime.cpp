//
//  IsPrime.cpp
//  Проверить, является ли заданное натуральное число n простым.
//
//  Оценка
//  Время работы T(n) = O(√n)
//  Объем доп.памяти M(n) = O(1)
//
//  Copyright © 2018 lostmap. All rights reserved.
//
#include <iostream>
bool IsPrime(int number) {
  if (number == 1) {
    return false;
  }
  for (int factor = 2; factor * factor <= number; factor++) {
    if (number % factor == 0) {
      return false;
    }
  }
  return true;
}

int main() {
  int number = 0;
  std::cin >> number;
  std::cout << IsPrime(number) << std::endl;
  return 0;
}
