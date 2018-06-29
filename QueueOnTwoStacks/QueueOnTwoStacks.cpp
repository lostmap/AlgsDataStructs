//
//  QueueOnTwoStacks.cpp
//  Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с помощью динамического буфера.
//  Обрабатывать команды push back и pop front.
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
//  TODO: add templates
//
#include <assert.h>
#include <iostream>

class Stack {
 public:
  Stack();
  ~Stack();
  void Push(int value);
  int Pop();
  bool IsEmpty() const;
  void Print() const;
 private:
  int* buffer_;
  int buffer_size_;
  int top_;
  void Extend();
};
Stack::Stack() : buffer_size_(1), top_(-1) {
  buffer_ = new int[buffer_size_];
}
Stack::~Stack() {
  delete [] buffer_;
}
void Stack::Extend() {
  int new_buffer_size = buffer_size_ * 2;
  int* new_buffer = new int[new_buffer_size];
  for (int i = 0; i < buffer_size_; i++) {
    new_buffer[i] = buffer_[i];
  }
  delete[] buffer_;
  buffer_ = new_buffer;
  buffer_size_ = new_buffer_size;
}
void Stack::Push(int value) {
  if (top_ + 1 == buffer_size_) {
    Extend();
  }
  top_++;
  buffer_[top_] = value;
}
int Stack::Pop() {
  assert(top_ != -1);  // нельзя что-то вытащить из пустого стека
  int temp_value = buffer_[top_];
  top_--;
  return temp_value;
}
bool Stack::IsEmpty() const {
  if (top_ == -1) {
    return true;
  } else {
    return false;
  }
}
void Stack::Print() const {
  std::cout << "< ";
  for (int i = 0; i < buffer_size_; i++) {
    std::cout << buffer_[i] << " ";
    if (i == top_) {
      std::cout << "> ";
    }
  }
  std::cout << std::endl;
}
class Queue {
 public:
  Queue();
  ~Queue();
  void PushBack(int value);
  int PopFront();
  bool IsEmpty() const;
  void Print() const;
 private:
  Stack queue_push_;  // часть очереди, в которую происходит вставка
  Stack queue_pop_;  // часть очереди, из которой происходят извлечения
};
Queue::Queue() {}
Queue::~Queue() {}
void Queue::PushBack(int value) {
  queue_push_.Push(value);
}
int Queue::PopFront() {
  assert(!IsEmpty());  //// нельзя что-то вытащить из пустой очереди
  if (queue_pop_.IsEmpty()) {
    while (!queue_push_.IsEmpty()) {
      queue_pop_.Push(queue_push_.Pop());
    }
  }
  return queue_pop_.Pop();
}
bool Queue::IsEmpty() const {
  if (queue_push_.IsEmpty() && queue_pop_.IsEmpty()) {
    return true;
  } else {
    return false;
  }
}


int main() {
  int num = 0;
  //вводим количество операций
  std::cin >> num;
  Queue queue;
  for (int i = 0; i < num; i++) {
    int operation = 0;
    int value = 0;
    //формат: номер_команды значение
    std::cin >> operation >> value;
    switch (operation) {
        //команда push back
      case 3:
        queue.PushBack(value);
        break;
        //команда pop front
      case 2:
      {
        //если очередь пустая, то ожидаем "-1"
        //если очередь не пустая, то сравниваем значение из очереди с ожидаемым
        if ((queue.IsEmpty() && value != -1) ||
            (!queue.IsEmpty() && queue.PopFront() != value)) {
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
  return 0;
}
