//
//  Huffman.cpp
//  Алгоритм сжатия данных Хаффмана.
//
//  В архиве сохраняйте дерево Хаффмана и код Хаффмана от исходных данных.
//  Дерево Хаффмана требуется хранить эффективно - не более 10 бит на каждый 8-битный символ.
//
//  Метод Encode архивирует данные из потока original
//  Метод Decode восстанавливает оригинальные данные из потока compressed
//
//  Copyright © 2018 lostmap. All rights reserved.
//  TODO: Improve image compression & write proper interface
//
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <queue>
#include <map>
#include <stack>
#include <memory>

using std::stack;
using std::priority_queue;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;
size_t const ALPHABET = 256;

class IInputStream {
public:
  IInputStream(ifstream& input);
  bool Read(char &value);
private:
  ifstream& handler;
};
IInputStream::IInputStream(ifstream& input): handler(input)
{}
bool IInputStream::Read(char &value) {
  if (handler.peek() == EOF)
    return false;
  handler.read(&value, 1);
  return true;
}

class IOutputStream {
public:
  IOutputStream(ofstream& output);
  void Write(char value);
private:
  ofstream& handler;
};
IOutputStream::IOutputStream(ofstream& output): handler(output)
{}
void IOutputStream::Write(const char value) {
  handler.write(&value, 1);
}

struct Node {
  Node(size_t _prior = 0, bool _leaf = true , unsigned char _value = '\0', Node* _left = nullptr, Node* _right = nullptr);
  ~Node();
  size_t prior;
  bool leaf;
  unsigned char value;
  Node* left;
  Node* right;
  
  bool operator()(const Node& a, const Node& b) const;
};
void deleteTree(Node * node){
  if(node == nullptr){
    return;
  }
  deleteTree(node->left);
  deleteTree(node->right);
}
Node::Node(size_t _prior, bool _leaf, unsigned char _value, Node* _left, Node* _right)
: prior(_prior), leaf(_leaf), value(_value), left(_left), right(_right)
{}
Node::~Node() {
  deleteTree(left);
  delete left;
  deleteTree(right);
  delete right;
}


class Comparator {
public:
  bool operator()(const Node* a, const Node* b) const {
    return a->prior > b->prior;
  }
};


class InBitStream {
public:
  explicit InBitStream(vector<unsigned char> input);
  char ReadBit();
  char ReadByte();
  void ResetCursor();
  size_t GetCursorPos() const;
  size_t GetBytesCount() const;
  size_t GetEndPos() const;
private:
  vector<unsigned char> buffer;
  size_t cursor;
};
size_t InBitStream::GetEndPos() const {
  return buffer.size() * 8 + buffer.back() - 16;  // don't count last byte
}
InBitStream::InBitStream(vector<unsigned char> input) :
buffer(std::move(input)), cursor(0)
{}
char InBitStream::ReadBit() {
  assert(cursor < buffer.size() * 8);
  unsigned char value;
  value = (buffer[cursor / 8] << (cursor % 8));
  value = value >> 7;
  cursor++;
  if (value != '\0') {
    return '1';  // bit 1 as byte
  }
  return '0';  // bit 0 as byte
}
char InBitStream::ReadByte() {
  unsigned char value;
  if (cursor % 8 == 0) {
    value = buffer[cursor / 8];
  } else {
    value = buffer[cursor / 8] << (cursor % 8);
    value |= buffer[(cursor / 8) + 1] >> (8 - (cursor % 8));
  }
  cursor += 8;
  return value;
}
size_t InBitStream::GetBytesCount() const {
  return buffer.size();
}
size_t InBitStream::GetCursorPos() const {
  return cursor;
}
void InBitStream::ResetCursor() {
  cursor = 0;
}

class OutBitStream {
public:
  OutBitStream();
  void WriteBit(unsigned char bit);
  void WriteByte(unsigned char byte);
  void WriteNewByte(unsigned char byte);
  const unsigned char* GetBuffer() const;
  size_t GetBytesCount() const;
  size_t GetLastBitsCount() const;
  size_t size() const;
private:
  vector<unsigned char> buffer;
  size_t bitsCount;
  size_t lastBitsCount;
};
OutBitStream::OutBitStream() :
bitsCount(0), lastBitsCount(0) {}

size_t OutBitStream::size() const {
  return buffer.size();
}

void OutBitStream::WriteBit(unsigned char bit) {
  if (bitsCount + 1 > buffer.size() * 8) {
    buffer.push_back('\0');
  }
  if (bit != '0') {
    buffer[bitsCount / 8] |= 1 <<  ((7 - bitsCount) % 8);
  }
  lastBitsCount = (lastBitsCount + 1) % 8;
  bitsCount++;
}
void OutBitStream::WriteNewByte(unsigned char byte) {
  buffer.push_back(byte);
  bitsCount += (8 + 8 - lastBitsCount);
}
void OutBitStream::WriteByte(unsigned char byte) {
  if (bitsCount % 8 == 0) {
    buffer.push_back(byte);
  } else {
    size_t offset = bitsCount % 8;
    buffer[bitsCount / 8] |= byte >> offset;
    buffer.push_back(byte << (8 - offset));
  }
  bitsCount += 8;
}
size_t OutBitStream::GetBytesCount() const {
  return buffer.size();
}
size_t OutBitStream::GetLastBitsCount() const {
  return lastBitsCount;
}
const unsigned char* OutBitStream::GetBuffer() const {
  return buffer.data();
}


void TableFromTree(vector<vector<unsigned char>>& table,vector<unsigned char> buff, Node* tree, int value) {
  if (value == 0) {
    buff.push_back('0');
  } else if (value == 1) {
    buff.push_back('1');
  }
  if (tree == nullptr) {
    buff.pop_back();
    return;
  } else if (tree->leaf == true) {
    table[tree->value] = buff;
    buff.pop_back();
    return;
  }
  value = 0;
  TableFromTree(table, buff, tree->left, value);
  value = 1;
  TableFromTree(table, buff, tree->right, value);
  if (tree->leaf == false)
    buff.pop_back();
}

Node* EncodeTree(OutBitStream& output, priority_queue<Node*, vector<Node*>, Comparator>& heap) {
  uint8_t bytes = heap.size();
  output.WriteByte(bytes);  // bytes in tree
  bool flag = true;
  while (heap.size() != 1 || flag) {
    flag = false;
    Node* right = nullptr;
    size_t prior_r = 0;
    if (!heap.empty()) {
      right = heap.top();
      prior_r = right->prior;
      heap.pop();
    }
    Node* left = nullptr;
    size_t prior_l = 0;
    if (!heap.empty()) {
      left = heap.top();
      prior_l = left->prior;
      heap.pop();
    }
    Node* node = nullptr;
    if (bytes == 1) {
      node = new Node(prior_r + prior_l, false, '\0', right, left);
    } else {
      node = new Node(prior_r + prior_l, false, '\0', left, right);
    }
    heap.push(node);
  }
  
  Node* tree = std::move(heap.top());
  heap.pop();
  return tree;
}

Node* DecodeTree(InBitStream& input, stack<Node*>& decoder) {
  uint8_t totalBytes = input.ReadByte();
  size_t currBytes = 0;
  bool flag = true;
  while (decoder.size() != 1 || flag) {
    if (currBytes == totalBytes) {
      flag = false;
    }
    unsigned char value;
    value = input.ReadBit();
    if (value == '1') {
      value = input.ReadByte();
      currBytes++;
      Node* node = new Node(0, true, value, nullptr, nullptr);
      decoder.push(node);
      
    } else if (value == '0') {
      Node* right = nullptr;
      if (!decoder.empty()) {
        right = decoder.top();
        decoder.pop();
      }
      Node* left = nullptr;
      if (!decoder.empty()) {
        left = decoder.top();
        decoder.pop();
      }
      Node* node = nullptr;
      if (totalBytes == 1) {
        node = new Node(0, false, '\0', right, left);
      } else {
        node = new Node(0, false, '\0', left, right);
      }
      decoder.push(node);
    }
  }
  Node* tree = std::move(decoder.top());
  decoder.pop();
  return tree;
}

void WriteTree(OutBitStream& encoded, Node* tree) {
  if (tree == nullptr)
    return;
  if (tree->leaf == true) {
    encoded.WriteBit('1');
    encoded.WriteByte(tree->value);
  } else {
    WriteTree(encoded, tree->left);
    WriteTree(encoded, tree->right);
    encoded.WriteBit('0');
  }
}

void Encode(IInputStream& original, IOutputStream& compressed) {
  vector<size_t> count(ALPHABET, 0);
  vector<unsigned char> buff;
  char value;
  bool empty = true;  // if nothing to encode
  while (original.Read(value)) {
    empty = false;
    count[(unsigned char)value]++;
    buff.push_back((unsigned char)value);
  }
  if (empty)
    return;
  priority_queue<Node*, vector<Node*>, Comparator> heap;
  for (size_t i = 0; i < ALPHABET; i++) {
    if (count[i] != 0) {
      Node* node = new Node(count[i], true, (unsigned char)i, nullptr, nullptr);
      heap.push(node);
    }
  }
  
  OutBitStream output;
  Node* tree = EncodeTree(output, heap);
  
  vector<vector<unsigned char>> table(ALPHABET);  // huff table
  vector<unsigned char> code;
  TableFromTree(table, code, tree, -1);
  
  WriteTree(output, tree);
  delete tree;
  
  for (size_t byte = 0; byte < buff.size(); byte++) {
    for (size_t bit = 0; bit < table[buff[byte]].size(); bit++) {
      output.WriteBit(table[buff[byte]][bit]);
    }
  }
  
  output.WriteNewByte(output.GetLastBitsCount());  // to distinguish bits
  
  const unsigned char* pack = output.GetBuffer();
  for (size_t byte = 0; byte < output.size(); byte++) {
    compressed.Write((char)pack[byte]);
  }
}

void Decode(IInputStream& compressed, IOutputStream& original) {
  vector<unsigned char> buff;
  char value;
  bool empty = true;  // if nothing to encode
  while (compressed.Read(value)) {
    empty = false;
    buff.push_back((unsigned char)value);
  }
  if (empty)
    return;
  
  InBitStream input(buff);
  stack<Node*> decoder;
  Node* tree = DecodeTree(input, decoder);
  
  vector<unsigned char> output;
  for (size_t cursor = input.GetCursorPos(); cursor < input.GetEndPos(); cursor++) {
    output.push_back(input.ReadBit());
  }
  Node* point = tree;
  for (size_t i = 0; i < output.size(); i++) {
    if (output[i] == '0') {
      point = point->left;
    } else {
      point = point->right;
    }
    if (point == nullptr) {
      point = tree;
    } else if (point->leaf == true) {
      original.Write((char)point->value);
      point = tree;
    }
  }
  delete tree;
}

int main() {
  // encode
  ifstream input;
  input.open("input.txt", std::ios::binary);
  
  ofstream output;
  output.open("output.txt", std::ios::binary);
  
  IInputStream original(input);
  IOutputStream compressed(output);
  Encode(original, compressed);
  
  input.close();
  output.close();
  
  // decode
  /*
  ofstream input;
  input.open("input.txt", std::ios::binary);

  ifstream output;
  output.open("output.txt", std::ios::binary);

  IInputStream compressed(output);
  IOutputStream original(input);
  Decode(compressed, original);

  input.close();
  output.close();
  */
  return 0;
}


