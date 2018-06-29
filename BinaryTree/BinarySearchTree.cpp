//
//  BinarySearchTree.cpp
//  Обход дерева в порядке level-order
//
//  Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
//  Требуется построить бинарное дерево поиска, заданное наивным порядком вставки. Т.е.,
//  при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел
//  K добавляется в правое поддерево root; иначе в левое поддерево root.
//  Выведите элементы в порядке level-order (по слоям, "в ширину").
//
//  Формат ввода
//  В первой строке записано число N.
//  Во последующих N строках — целые числа исходной последовательности.
//
//  Формат вывода
//  N целых чисел, разделенных пробелом.
//
//  Copyright © 2018 lostmap. All rights reserved.
//
#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include <string>
using std::vector;
using std::queue;
using std::stack;
template <class T>
struct Node {
  T data;
  Node* left;
  Node* right;
  Node(T data = T{}, Node* left = nullptr, Node* right = nullptr)
      : data(data),
        left(left),
        right(right) {}
  ~Node() {}
};
template <class T>
class BinarySearchTree {
 public:
  explicit BinarySearchTree(Node<T>* root = nullptr);
  ~BinarySearchTree();
  void Add(const T& data);
  vector<T> TraverseBFS();
 private:
  Node<T>* root;
};
template <class T>
BinarySearchTree<T>::BinarySearchTree(Node<T>* root)
      : root(root) {}
template <class T>
BinarySearchTree<T>::~BinarySearchTree() {  // post-order delete
  if (root == nullptr) {
    return;
  }
  stack<Node<T>*> stack1;
  stack<Node<T>*> stack2;
  stack1.push(root);
  while (!stack1.empty()) {
    root = stack1.top();
    stack2.push(root);
    stack1.pop();
    if (root->left != nullptr) {
      stack1.push(root->left);
    }
    if (root->right != nullptr) {
      stack1.push(root->right);
    }
  }
  while (!stack2.empty()) {
    delete stack2.top();
    stack2.pop();
  }
}
template <class T>
void BinarySearchTree<T>::Add(const T& data) {
  if (root == nullptr) {  // first entry
    root = new Node<T>;
    root->data = data;
    return;
  }
  Node<T>* init = root;
  Node<T>* insert = new Node<T>;
  insert->data = data;
  while (root != nullptr) {
    if (insert->data >= root->data) {
      if (root->right != nullptr) {
        root = root->right;
      } else {
        root->right = insert;
        break;
      }
    } else {
      if (root->left != nullptr) {
        root = root->left;
      } else {
        root->left = insert;
        break;
      }
    }
  }
  root = init;  // set back pointer to the root
}
template <class T>
vector<T> BinarySearchTree<T>::TraverseBFS() {
  vector<T> output;  // output
  if (root == nullptr) {
    return output;
  }
  queue<Node<T>*> queue;
  queue.push(root);
  while (!queue.empty()) {
    Node<T>* buff = queue.front();
    output.push_back(buff->data);
    //std::cout << buff->data << " ";
    queue.pop();
    if (buff->left != nullptr) {
      queue.push(buff->left);
    }
    if (buff->right != nullptr) {
      queue.push(buff->right);
    }
  }
  return output;
}
int main() {
  BinarySearchTree<int> test;
  int num = 0;
  int data = 0;
  std::cin >> num;
  for (int i = 0; i < num; i++) {
    std::cin >> data;
    test.Add(data);
  }
  vector<int> output = test.TraverseBFS();
  for (int i = 0; i < output.size(); i++) {
    std::cout << output[i] << " ";
  }
  return 0;
}
