//
//  Unimod.cpp
//  Унимодальная последовательность.
//
//  Дан массив целых чисел А[0..n-1]. Известно, что на интервале [0, m] значения массива
//  строго возрастают, а на интервале [m, n-1] строго убывают. Найти m за O( log m ).
//  2 ≤ n ≤ 10000.
//
//  Created by Oleg Kabanov on 06.03.18.
//  Copyright © 2018 Oleg Kabanov. All rights reserved.
//
#include <iostream>
int FindMax(int const *arr, int count, int first, int last) {
  if (count < last) {
    last = count;
  }
  while (first + 1 < last) {
    if (last - first <= 2) {
      if (arr[first] < arr[first + 1]) {
        return first + 1;
      } else {
        return first;
      }
    }
    int mid = (first + last) / 2;
    if (arr[mid - 1] < arr[mid]) {
      if (arr[mid] < arr[mid + 1]) {
        first = mid + 1; // Искомый элемент находится справа.
      } else {
        return mid;
      }
    } else {
      last = mid; // Искомый элемент находится слева.
    }
  }
  return first;
}
int FindInterval(int const *arr, int count) {
  int result = 0;
  if (arr[result] < arr[result + 1]) {
    result = 1;
  } else {
    return result;
  }
  // находим индексы степени 2, между которыми находится искомый элемент
  while (result * 2 < count) {
    if (arr[result] < arr[result * 2]) {
      result *= 2;
    } else {
      return result;
    }
  }
  return result;
}
int main() {
  int n = 0;
  std::cin >> n;
  int *mas = new int[n];
  for (int i = 0; i < n; i++) {
    std::cin >> mas[i];
  }
  int index = FindInterval(mas, n);
  std::cout << FindMax(mas, n, index, index * 2) << std::endl;
  delete[] mas;
  return 0;
}

