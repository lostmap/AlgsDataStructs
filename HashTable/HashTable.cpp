//
//  HashTable.cpp
//  Хеш-таблица
//
//  Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
//  Хранимые строки непустые и состоят из строчных латинских букв. Хеш-функция строки должна быть реализована с
//  помощью вычисления значения многочлена методом Горнера. Начальный размер таблицы должен быть равным 8-ми.
//  Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
//  Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества
//  и проверки принадлежности данной строки множеству. Для разрешения коллизий используйте двойное хеширование.
//
//  Формат ввода
//  Каждая строка входных данных задает одну операцию над множеством. Запись операции состоит из типа операции и
//  следующей за ним через пробел строки, над которой проводится операция. Тип операции – один из трех символов:
//  + означает добавление данной строки в множество; - означает удаление строки из множества; ? означает проверку
//  принадлежности данной строки множеству. При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует
//  в этом множестве. При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.
//
//  Формат вывода
//  Программа должна вывести для каждой операции одну из двух строк OK или FAIL, в зависимости от того, встречается
//  ли данное слово в нашем множестве.
//
//  Copyright © 2018 lostmap. All rights reserved.
//
#include <iostream>
#include <vector>
#include <string>
using std::string;
using std::size_t;
using std::vector;

const double Alpha = 0.75;
const size_t Prime1 = 511;
const size_t Prime2 = 1023;
const size_t DefaultSize = 8;

template<class T>
class HashTable {
 public:
  explicit HashTable(T deleted, T empty);
  ~HashTable();
  bool Has(const T& key);
  bool Add(const T& key);
  bool Del(const T& key);
 private:
  size_t Hash1(const T& key);
  size_t Hash2(const T& key);
  vector<T> table;
  void rehash();
  T deleted;
  T empty;
  size_t keysCount;
  size_t tableSize;
};
template <class T>
HashTable<T>::HashTable(T _deleted, T _empty)
    : deleted(_deleted),
      empty(_empty),
      keysCount(0),
      tableSize(DefaultSize) {
        table.resize(tableSize, empty);
}
template <class T>
HashTable<T>::~HashTable() {}

template <>
size_t HashTable<string>::Hash1(const string& key) {  // Hash1 for strings
  size_t hash = 0;
  for (size_t i = 0; i < key.size(); i++) {
    hash = (hash * Prime1 + key[i]) % tableSize;
  }
  return hash;
}
template <>
size_t HashTable<int>::Hash1(const int& key) {  // Hash1 for ints
  return key % tableSize;
}
template <>
size_t HashTable<string>::Hash2(const string& key) {  // Hash2 for strings
  size_t hash = 0;
  for (size_t i = 0; i < key.size(); i++) {
    hash = (hash * Prime2 + key[i]) % tableSize;
  }
  return hash * 2 + 1;
}
template <>
size_t HashTable<int>::Hash2(const int& key) {  // Hash2 for ints
  return 2 * (key % tableSize) + 1;
}
template <class T>
void HashTable<T>::rehash() {
  vector<T> clone = table;
  table.clear();
  table.resize(2 * tableSize, empty);
  tableSize = 2 * tableSize;
  keysCount = 0;
  for (size_t i = 0; i < clone.size(); i++) {
    if (clone[i] != empty) {
      Add(clone[i]);
    }
  }
}
template <class T>
bool HashTable<T>::Has(const T& key) {
  size_t probe = 0;
  size_t hash1 = Hash1(key);
  size_t hash2 = Hash2(key);
  while (probe != tableSize) {
    if (table[hash1] == key) {
      return true;  // found
    }
    if (table[hash1] == empty) {
      return false;  // not found
    }
    probe++;
    hash1 = (hash1 + probe * hash2) % tableSize;
  }
  return false;
}
template <class T>
bool HashTable<T>::Add(const T& key) {
  if (keysCount >= Alpha * tableSize) {
    rehash();
  }
  size_t probe = 0;
  size_t hash1 = Hash1(key);
  size_t hash2 = Hash2(key);
  size_t onDeleted = 0;
  bool remember = false;
  while (probe != tableSize) {
    if (table[hash1] == key) {
      return false;  // found dublicate
    }
    if (table[hash1] == empty ||
        table[hash1] == deleted) {
      if (table[hash1] == empty) {
        if (remember) {  // if no dublicates adding to hash on deleted
          hash1 = onDeleted;
        }
      }
      if (table[hash1] == deleted &&
          remember == false) {  // remembering hash on deleted
          onDeleted = hash1;
          remember = true;
      } else {
        table[hash1] = key;
        keysCount++;
        return true;
      }
    }
    probe++;
    hash1 = (hash1 + probe * hash2) % tableSize;
  }
  return false;
}
template <class T>
bool HashTable<T>::Del(const T& key) {
  size_t probe = 0;
  size_t hash1 = Hash1(key);
  size_t hash2 = Hash2(key);
  while (probe != tableSize) {
    if (table[hash1] == empty) {
      return false;  // not found
    }
    if (table[hash1] == key) {
      table[hash1] = deleted;
      keysCount--;
      return true;
    }
    probe++;
    hash1 = (hash1 + probe * hash2) % tableSize;
  }
  return false;
}
int main() {
  HashTable<string> table("DEL", "");  // setting special symbols
  char operation;
  string word;
  while (std::cin >> operation >> word) {
    if (operation == '+')
      std::cout << (table.Add(word) ? "OK" : "FAIL") << std::endl;
    if (operation == '-')
      std::cout << (table.Del(word) ? "OK" : "FAIL") << std::endl;
    if (operation == '?')
      std::cout << (table.Has(word) ? "OK" : "FAIL") << std::endl;
  }
  return 0;
}
