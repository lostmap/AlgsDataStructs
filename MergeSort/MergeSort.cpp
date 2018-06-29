//
//  MergeSort.cpp
//  Закраска прямой в один слой, сортировка слиянием.
//
//  На числовой прямой окрасили N отрезков.
//  Известны координаты левого и правого концов каждого отрезка [Li, Ri].
//  Найти сумму длин частей числовой прямой, окрашенных ровно в один слой.
//  N ≤ 10000. Li, Ri — целые числа в диапазоне [0, 10^9].
//
//  Формат ввода
//  В первой строке записано количество отрезков.
//  В каждой последующей строке через пробел записаны координаты левого и правого концов отрезка.
//
//  Формат вывода
//  Выведите целое число — длину окрашенной в один слой части.
//
//  Copyright © 2018 lostmap. All rights reserved.
//
#include <iostream>
struct Line {
  int cord;  // координата
  int paint;  // начало или конец покраски
};
// Функторы
template <class T>
class IsLess {  // по убыванию
 public:
  bool operator()(const T& left, const T& right) {
    return left < right;
  }
};
template <class T>
class IsGreater {  // по возрастанию
public:
  bool operator()(const T& left, const T& right) {
    return left > right;
  }
};
class IsLessLine {  // по убыванию для структуры Line
 public:
  bool operator()(const Line& left, const Line& right) {
    return left.cord < right.cord;
  }
};
class IsGreaterLine {  // по возрастанию для структуры Line
 public:
  bool operator()(const Line& left, const Line& right) {
    return left.cord > right.cord;
  }
};
template <class T, class Compare>
void Merge(T* arr, int start, int part, int end, Compare cmp) {
  int n = part - start + 1;
  int m = end - part;
  T* left = new T[n];
  T* right = new T[m];
  for (int i = 0; i < n; i++) {
    left[i] = arr[start + i];
  }
  for (int j = 0; j < m; j++) {
    right[j] = arr[part + 1 + j];
  }
  int i = 0;  // итератор по левому массиву
  int j = 0;  // итератор по правому массиву
  int k = start;  // итератор по слитому массиву
  while (i < n && j < m) {
    if (cmp(right[j], left[i])) {
      
      arr[k] = right[j++];
    } else {
      arr[k] = left[i++];
    }
    k++;
  }
  for( ; i < n; i++) {  // правый массив пуст
    arr[k] = left[i];
    k++;
  }
  for( ; j < m; j++) {  // левый массив пуст
    arr[k] = right[j];
    k++;
  }
  delete [] left;
  delete [] right;
}
/*
template <class T>
void RecursiveMergeSort(T* arr, int start, int end) {
  if (start < end) {
    int part = (start + end) / 2;
    MergeSort(arr, start, part);
    MergeSort(arr, part + 1, end);
    Merge(arr, start, part, end);
  }
}
*/
template <class T, class Compare>
void MergeSort(T* arr, int start, int end, Compare cmp) {
  int size = end - start + 1;
  // выбираем по степени двойки размер подмассива
  for (int power_of_2 = 1; power_of_2 < size ; power_of_2 = power_of_2 * 2) {
    // находим начальные индексы подмассивов текущего размера
    for (int index = start; index <= end - power_of_2; index += power_of_2 * 2) {
      int min = index + power_of_2 * 2 - 1;
      // выбираем конечный индекс, чтобы он не вышел за пределы массива
      if (end < min) {
        min = end;
      }
      Merge(arr, index, index + power_of_2 - 1, min, cmp);  // сливаем подмассивы
    }
  }
}
template <class T, class Compare>
int CountLength(T* mas, int n, Compare cmp) {
  MergeSort(mas, 0, n - 1, cmp);
  int width = 0;
  int length = 0;
  int temp = mas[0].cord;
  for (int i = 0; i < n; i++) {
    if (width == 1) {
      length += mas[i].cord - temp;
    }
    width += mas[i].paint;
    temp = mas[i].cord;
  }
  return length;
}
int main() {
  int n = 0;
  std::cin >> n;
  n = n * 2;
  Line* mas = new Line[n];
  for (int i = 0; i < n; i += 2) {
    std::cin >> mas[i].cord >> mas[i + 1].cord;
    mas[i].paint = 1;
    mas[i + 1].paint = -1;
  }
  std::cout << CountLength(mas, n, IsLessLine()) << std::endl;
  delete [] mas;
  return 0;
}
