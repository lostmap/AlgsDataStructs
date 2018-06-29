//
//  DequeOnArray.cpp
//  Реализовать дек с динамическим зацикленным буфером.
//
//  Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
//  Формат входных данных.
//  В первой строке количество команд n. n ≤ 1000000.
//  Каждая команда задаётся как 2 целых числа: a b.
//  a = 1 - push front
//  a = 2 - pop front
//  a = 3 - push back
//  a = 4 - pop back
//  Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
//  Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
//  Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается "-1".
//  Формат выходных данных.
//  Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
//
//  Copyright © 2018 lostmap. All rights reserved.
//
//  TODO: add templates
#include <assert.h>
#include <iostream>

class Deque {
public:
  Deque();
  ~Deque();
  void PushFront(int value);
  void PushBack(int value);
  int PopFront();
  int PopBack();
  bool IsEmpty() const;
  void Print() const;
private:
  int* buffer_;
  int buffer_size_;
  int size_;  // реальный размер дека
  int head_;
  int tail_;
  void Extend();
};
Deque::Deque() : buffer_size_(1), size_(0), head_(0), tail_(0) {
  buffer_ = new int[buffer_size_];
}
Deque::~Deque() {
  delete [] buffer_;
}
void Deque::Extend() {
  int new_buffer_size = buffer_size_ * 2;
  int* new_buffer = new int[new_buffer_size];
  int old_tail_ = tail_;
  for (int i = 0; i < size_; i++) {
    new_buffer[i] = buffer_[old_tail_];
    old_tail_ = (old_tail_ + 1) % buffer_size_;
  }
  delete[] buffer_;
  buffer_ = new_buffer;
  buffer_size_ = new_buffer_size;
  head_ = size_ - 1;
  tail_ = 0;
}
void Deque::PushFront(int value) {  // 1 -->
  if (size_ == buffer_size_) {
    Extend();
  }
  if (buffer_size_ > 1) {
    head_ = (head_ + 1) % buffer_size_;
  }
  buffer_[head_] = value;
  size_++;
}
void Deque::PushBack(int value) {  // 3 <--
  if (size_ == buffer_size_) {
    Extend();
  }
  if (buffer_size_ > 1) {
    tail_ = (tail_ - 1 + buffer_size_) % buffer_size_;
  }
  buffer_[tail_] = value;
  size_++;
}
int Deque::PopFront() {  // 2 <--
  assert(size_ != 0);  // нельзя что-то вытащить из пустого дека
  int temp_value = buffer_[head_];
  head_ = (head_ - 1 + buffer_size_) % buffer_size_;
  size_--;
  return temp_value;
}
int Deque::PopBack() {  // 4 -->
  assert(size_ != 0);  // нельзя что-то вытащить из пустого дека
  int temp_value = buffer_[tail_];
  tail_= (tail_ + 1) % buffer_size_;
  size_--;
  return temp_value;
}
bool Deque::IsEmpty() const {
  if (size_ == 0) {
    return true;
  } else {
    return false;
  }
}
void Deque::Print() const {  // "< " - tail_, " >" - head_
  for (int i = 0; i < buffer_size_; i++) {
    if (i == tail_) {
      std::cout << "< ";
    }
    std::cout << buffer_[i] << " ";
    if (i == head_) {
      std::cout << "> ";
    }
  }
  std::cout << std::endl;
}
int main() {
  int num = 0;
  std::cin >> num;
  Deque deque;
  for (int i = 0; i < num; i++) {
    int operation = 0;
    int value = 0;
    std::cin >> operation >> value;
    switch (operation) {
      case 3:
        deque.PushBack(value);
        break;
      case 1:
        deque.PushFront(value);
        break;
      case 2:
      {
        if ((deque.IsEmpty() && value != -1) ||
            (!deque.IsEmpty() && deque.PopFront() != value)) {
          std::cout << "NO" << std::endl;
          return 0;
        }
        break;
      }
      case 4:
      {
        if ((deque.IsEmpty() && value != -1) ||
            (!deque.IsEmpty() && deque.PopBack() != value)) {
          std::cout << "NO" << std::endl;
          return 0;
        }
        break;
      }
      default:
        break;
    }
  }
  std::cout << "YES" << std::endl;
}

