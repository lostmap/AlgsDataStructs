//
//  Heap.cpp
//  Жадина, куча.
//
//  Вовочка ест фрукты из бабушкиной корзины.
//  В корзине лежат фрукты разной массы. Вовочка может поднять не более k грамм.
//  Каждый фрукт весит не более K грамм. За раз он выбирает несколько самых тяжелых фруктов,
//  которые может поднять одновременно, откусывает от каждого половину и кладет огрызки обратно
//  в корзину. Если фрукт весит нечетное число грамм, он откусывает большую половину.
//  Фрукт массы 1гр он съедает полностью.
//
//  Определить за сколько подходов Вовочка съест все фрукты в корзине.
//
//  Формат ввода
//  Вначале вводится n - количество фруктов и массив весов фруктов. n ≤ 50000.
//  Затем k - "грузоподъемность". k ≤ 1000.
//
//  Формат вывода
//  Неотрицательное число - количество подходов к корзине.
//
//  Copyright © 2018 lostmap. All rights reserved.
//
#include <assert.h>
#include <iostream>
class Heap {  // невозрастающая двоичная куча (пирамида)
 public:
  Heap();
  explicit Heap(int size);
  explicit Heap(int* array, int size);
  ~Heap();
  void Add(int value);
  int ExtractMax();
  int GetMax() const;
  bool IsEmpty() const;
  int Size() const;
  void Print() const;
 private:
  void SiftUp(int index);
  void SiftDown(int index);
  void Swap(int& a, int& b);
  void Extend();
  int* buffer_;
  int buffer_size_;
  int size_;
};
Heap::Heap() : buffer_size_(1), size_(0) {
  buffer_ = new int[buffer_size_];
}
Heap::Heap(int size) : buffer_size_(size), size_(0) {
  assert(size > 0);
  buffer_ = new int[buffer_size_];
}
Heap::Heap(int* array, int size) : buffer_size_(size), size_(size) {
  assert(size > 0);
  buffer_ = new int[buffer_size_];
  for (int i = 0; i < size; i++) {
    buffer_[i] = array[i];
  }
  for (int i = size_ / 2 - 1; i >= 0; i--) {
    SiftDown(i);
  }
}
Heap::~Heap() {
  delete[] buffer_;
}
void Heap::Add(int value) {
  if (size_ == buffer_size_) {
    Extend();
  }
  buffer_[size_++] = value;
  SiftUp(size_ - 1);
}
int Heap::ExtractMax() {
  assert(!IsEmpty());
  int result = buffer_[0];
  buffer_[0] = buffer_[--size_];
  if (!IsEmpty()) {
    SiftDown(0);
  }
  return result;
}
int Heap::GetMax() const {
  assert(!IsEmpty());
  return buffer_[0];
}
int Heap::Size() const {
  return size_;
}
bool Heap::IsEmpty() const {
  return (size_ == 0);
}

void Heap::Print() const {
  for (int i = 0; i < size_; i++) {
    std::cout << buffer_[i] << " ";
  }
  std::cout << std::endl;
}
void Heap::Extend() {
  int new_buffer_size = buffer_size_ * 2;
  int* new_buffer = new int[new_buffer_size];
  for (int i = 0; i < buffer_size_; i++) {
    new_buffer[i] = buffer_[i];
  }
  delete[] buffer_;
  buffer_ = new_buffer;
  buffer_size_ = new_buffer_size;
}
void Heap::Swap(int& a, int& b) {
  int temp = a;
  a = b;
  b = temp;
}
void Heap::SiftDown(int index) {
  while (true) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int largest = index;
    if (left < size_ && buffer_[left] > buffer_[index]) {
      largest = left;
    }
    if (right < size_ && buffer_[right] > buffer_[largest]) {
      largest = right;
    }
    if (largest != index) {
      Swap(buffer_[index], buffer_[largest]);
      index = largest;
    } else {
      return;
    }
  }
}
void Heap::SiftUp(int index) {
  while (index > 0) {
    int parent = (index - 1) / 2;
    if (buffer_[index] <= buffer_[parent]) {
      return;
    }
    Swap(buffer_[index], buffer_[parent]);
    index = parent;
  }
}
class Stack {  // стек
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
class Queue {  // очередь на двух стеках
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
  assert(!IsEmpty());  // нельзя что-то вытащить из пустой очереди
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
  std::cin >> num;
  int* array = new int[num];
  for (int i = 0; i < num; i++) {
    std::cin >> array[i];
  }
  Heap bucket(array, num);  // инициализация кучи массивом
  delete [] array;
  Queue hands;
  int capacity = 0;
  int payload = 0;
  int amount = 0;
  int total_payload = 0;
  std::cin >> capacity;
  while (!bucket.IsEmpty()) {
    while (!bucket.IsEmpty() && bucket.GetMax() + total_payload <= capacity) {
      payload = bucket.ExtractMax();
      total_payload += payload;
      if (payload != 1) {
        hands.PushBack(payload / 2);  // кладет яблоки в руки
      }
    }
    while (!hands.IsEmpty()) {
      bucket.Add(hands.PopFront());  // бросает огрызки в корзину
    }
    total_payload = 0;
    amount++;
  }
  std::cout << amount << std::endl;
  return 0;
}
