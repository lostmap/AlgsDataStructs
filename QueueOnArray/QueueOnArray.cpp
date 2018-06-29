//
//  QueueOnArray.cpp
//  Реализовать очередь с динамическим зацикленным буфером.
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
class Queue {
public:
  explicit Queue(int size);
  ~Queue();
  void PushBack(int value);
  int PopFront();
  bool IsEmpty() const;
private:
  int* buffer_;
  int buffer_size_;
  int head_; // первый элемент очереди
  int tail_; // следующий элемент после последнего
};
Queue::Queue(int size) : buffer_size_(size), head_(0), tail_(0) {
  buffer_ = new int[buffer_size_];
}
Queue::~Queue() {
  delete [] buffer_;
}
void Queue::PushBack(int value) {
  assert(tail_ != -1);  // нельзя впихнуть элемент, если буфер переполнен
  buffer_[tail_] = value;
  if ((tail_ + 1) % buffer_size_ == head_) { // буфер переполнен
    tail_ = -1;
  } else { //
    tail_ = (tail_ + 1) % buffer_size_;
  }
}
int Queue::PopFront() {
  if (tail_ == -1) { // освобождаем место в буфере
    tail_ = head_;
  }
  int temp_data = buffer_[head_];
  head_ = (head_ + 1) % buffer_size_;
  return temp_data;
}
bool Queue::IsEmpty() const {
  return (head_ == tail_);
}
int main() {
  int num = 0;
  std::cin >> num;
  Queue queue(num);
  for (int i = 0; i < num; i++) {
    int operation = 0;
    int value = 0;
    std::cin >> operation >> value;
    switch (operation) {
      case 3:
        queue.PushBack(value);
        break;
      case 2:
      {
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
}
