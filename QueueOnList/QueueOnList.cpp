//
//  QueueOnList.cpp
//  Реализовать очередь на односвязном списке.
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

#include <assert.h>
#include <iostream>

struct Node {
  int data;
  Node* next;
  Node(): data(0), next(nullptr) {}
  explicit Node(int value): data(value), next(nullptr) {}
};

class Queue {
 public:
  Queue();
  ~Queue();
  void PushBack(int value);
  int PopFront();
  bool IsEmpty() const;
 private:
  Node* head_;
  Node* tail_;
};

Queue::Queue() : head_(nullptr), tail_(nullptr) {}

Queue::~Queue() {
  while (!IsEmpty()) {
    PopFront();
  }
}

void Queue::PushBack(int value) {
  Node* new_node = new Node(value);
  if (IsEmpty()) {
      head_ = tail_ = new_node;
      return;
  }
  tail_->next = new_node;
  tail_ = new_node;
}

int Queue::PopFront() {
  assert(!IsEmpty());
  Node* temp_node = head_;
  head_ = head_->next;
  int temp_data = temp_node->data;
  delete temp_node;
  if (head_ == nullptr) {
      tail_ = nullptr;
  }
  return temp_data;
}

bool Queue::IsEmpty() const {
  return (tail_ == nullptr) && (head_ == nullptr);
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



