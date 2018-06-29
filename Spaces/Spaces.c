//
//  Spaces.c
//  Удаление лишних пробелов
//
//  Time limit:  14 s
//  Memory limit:  64 M
//  Один из вариантов ускорения загрузки web-страниц - это уменьшить их размер. Например, за счет удаления
//  незначащих пробелов. Составить программу построчной обработки текста, в результате которой каждая группа
//  повторяющихся пробелов заменяется на один пробел. Текстовые строки подаются на стандартный ввод программы,
//  результат программы должен подаваться на стандартный вывод. Процедура обработки должна быть оформлена в
//  виде отдельной функции, которой подается на вход массив строк, выделенных в динамической памяти, и его
//  длина. На выход функция должна возвращать массив обработанных строк. Программа должна уметь обрабатывать
//  возникающие ошибки (например, ошибки выделения памяти). В случае возникновения ошибки нужно вывести в поток
//  стандартного вывода сообщение "[error]" и завершить выполнение программы.
//
//  ВАЖНО! Программа в любом случае должна возвращать 0. Не пишите return -1, exit(1) и т.п.
//  Даже если обнаружилась какая-то ошибка, все равно необходимо вернуть 0! (и напечатать [error] в stdout).
//
//  Copyright © 2018 lostmap. All rights reserved.
// TODO: попробуй с goto

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

char* work(char *mas) {
  if (mas == NULL) {
    return NULL;
  }
  size_t i = 0;
  size_t j = 0;
  bool found = false;
  while (mas[j] != '\0') {
    if (mas[j] != ' ') {
      mas[i++] = mas[j++];
      found = false;
    } else if (mas[j++] == ' ') {
      if (!found) {
        mas[i++] = ' ';
        found = true;
      }
    }
  }
  char* new_mas = realloc(mas, (i + 1) * sizeof(char));
  if (new_mas == NULL) {  // if realloc fails
    return NULL;
  }
  mas = new_mas;
  mas[i] = '\0';
  return mas;
}

char* GetString(char *buffer, bool* is_eof) {
  if (buffer != NULL) {
    return NULL;
  }
  if (is_eof == NULL) {
    return NULL;
  }
  size_t size = 0;
  size_t capacity = 1;
  char c = '\0';
  int32_t result = 0;
  do {
    result = scanf("%c", &c);
    if (result == EOF) {  // EOF
      *is_eof = true;
      return buffer;
    } else {
      if (result != 1) {  // symbol input error
        return NULL;
      }
    }
    if (size + 1 >= capacity) {
      size_t new_capacity = capacity * 2;
      char* new_buffer = realloc(buffer, (new_capacity + 1) * sizeof(char));
      if (new_buffer == NULL) {  // if realloc fails
        return NULL;
      }
      buffer = new_buffer;
      capacity = new_capacity;
    }
    buffer[size] = c;
    buffer[size + 1] = '\0';
    ++size;
  } while (c != '\n');
  return buffer;
}
void freer (char **mas, size_t rows) {
  if (mas == NULL) {
    return;
  }
  for (size_t i = 0; i < rows; ++i) {
    free(mas[i]);
    mas[i] = NULL;
  }
  free(mas);
  mas = NULL;
}
int main() {
  char **mas = NULL;
  size_t rows = 0;
  size_t row = -1;
  bool is_eof = false;
  while(!is_eof) {
    ++row;
    ++rows;
    char** new_mas = realloc(mas, rows * sizeof(char*));
    if (new_mas == NULL) {  // if realloc fails
      printf("[errno]");
      freer(mas, rows);
      return 0;
    }
    mas = new_mas;
    mas[row] = NULL;
    mas[row] = GetString(mas[row] ,&is_eof);
    if (mas[row] == NULL && is_eof == false) {  // if incorrect input
      printf("[errno]");
      freer(mas, rows);
      return 0;
    }
    mas[row] = work(mas[row]);
    if (mas[row] == NULL) {  // eof case
      break;
    }
  }
  
  for (size_t i = 0; i < rows; ++i) { // print
    size_t j = 0;
    if (mas[i] == NULL)
      break;
    while (mas[i][j] != '\0') {
      printf("%c", mas[i][j]);
      ++j;
    }
  }
  freer(mas, rows);
  return 0;
}
