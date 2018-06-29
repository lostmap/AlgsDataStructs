//
//  Polygon.cpp
//  Площадь n-угольника.
//
//  Вычислить площадь выпуклого n-угольника, заданного координатами своих вершин.
//  Вначале вводится количество вершин, затем последовательно целочисленные координаты
//  всех вершин в порядке обхода против часовой стрелки.
//  n < 1000, координаты < 10000.
//  Указание. Для вычисления площади n-угольника можно посчитать сумму ориентированных
//  площадей трапеций под каждой стороной многоугольника.
//
//  Copyright © 2018 lostmap. All rights reserved.
//
#include <iostream>
#include <cmath>

struct Point {
  double x;
  double y;
  Point(): x(0), y(0) {}
  Point(double x_cord, double y_cord): x(x_cord), y(y_cord) {}
};
double CalculateAreaOfNPoints(Point* mas, int size) {
  double calculated_area = 0;
  for (int i = 0; i < size; i++) {
    Point p1 = mas[i % size];
    Point p2 = mas[(i + 1) % size]; // замыкаем массив т.е. N пара будет считаться с 0 парой
    calculated_area += 0.5 * (p1.y + p2.y) * (p2.x - p1.x);  // 0.5 * (a + b) * h
  }
  return std::abs(calculated_area);
}
int main() {
  int n = 0;
  std::cin >> n;
  Point* mas = new Point[n];
  for (int i = 0; i < n; i++) {
    std::cin >> mas[i].x >> mas[i].y;
  }
  std::cout << CalculateAreaOfNPoints(mas, n) << std::endl;
  delete[] mas;
  return 0;
}
