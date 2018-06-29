//
//  FindKStatistics.cpp
//
//  Порядковая статистика.
//
//  Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
//  Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с
//  индексом k (0..n-1) в отсортированном массиве. Напишите нерекурсивный алгоритм.
//  Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).
//  Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
//  В качестве опорного выбирается "случайный элемент".
//
//  Copyright © 2018 lostmap. All rights reserved.
//
#include <iostream>
#include <random>
int Partition(int* arr, int start, int end) {
  std::random_device rd;
  auto seed = rd();  // число для инициализации генератора
  std::mt19937 gen(seed);  // гененратор псевдослучайных чисел
  std::uniform_int_distribution<int> range(start,end);  // генеририрует случайный int в диапазоне
  int random_pos = range(gen);  // случайную позицию опорного элемента
  int pivot = arr[random_pos];  // опорный "случайный элемент"
  std::swap(arr[random_pos], arr[end]);  // меняем опорный элемент с последним элементом массива
  int left_border = start;  //  граница разделения меньших и больших элементов
  int right_border = start;  //  граница разделения больших и не рассмотренных элементов
  while (right_border < end) {  // ходим по циклу, пока не дойдем до опорного элемента
    if (arr[right_border] <= pivot) {
      std::swap(arr[right_border], arr[left_border]);  // переносим меньший элемент на позицию левой границы
      left_border++;  // расширяем границу для меньших элементов
    }
    right_border++;  // расширяем границу для больших элементов
  }
  std::swap(arr[left_border], arr[end]);  // меняем опорный элемент с местом вставки
  return left_border;  // возвращаем новую позицию опорного элемента
}
int FindKStatistics(int* arr, int start, int end, int k) {
  int pivot_pos = Partition(arr, start, end);
  while (true) {
    if (pivot_pos == k) {  // если позиции совпали, то выводим ответ
      return  arr[pivot_pos];
    }
    if (pivot_pos < k) {  // сравниваем k-ю статистику с позицией опорного элемента
      start = pivot_pos + 1;
      pivot_pos = Partition(arr, start, end);  // правая часть
    } else {
      end = pivot_pos - 1;
      pivot_pos = Partition(arr, start, end);  // левая часть
    }
  }
}
int main() {
  int n = 0;
  int k = 0;
  std::cin >> n >> k;
  int* mas = new int[n];
  for (int i = 0; i < n; i++) {
    std::cin >> mas[i];
  }
  std::cout << FindKStatistics(mas, 0, n - 1, k) << std::endl;
  delete [] mas;
  return 0;
}
