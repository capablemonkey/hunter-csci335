/*
  HackerRank Challenge: QHEAP1

  https://www.hackerrank.com/challenges/qheap1
 */

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

template<typename T>
class Heap {
private:
  vector<T> list;

  int indexOfLeftChild(int i);
  int indexOfRightChild(int i);
  int indexOfParent(int i);
  void swapNodes(int a, int b);

public:
  void insert(T item);
  // void remove(T item);
  T getMinimum();
  void print();
};

template<typename T>
void Heap<T>::insert(T item) {
  list.push_back(item);

  int i = list.size() - 1;

  while(i > 0 && item > indexOfParent(i)) {
    swapNodes(i, indexOfParent(i));
    i = indexOfParent(i);
  }

  return;
};

template<typename T>
void Heap<T>::print() {
  typedef typename vector<T>::iterator iterator;
  for (iterator it = list.begin(); it != list.end(); ++it) {
    cout << " " << *it;
  }

  cout << endl;
}

template<typename T>
T Heap<T>::getMinimum() {
  // calculate beginning index of last row given the list's size
  int height = floor(log2(list.size()));
  int lastRowIndexBegin = pow(2, height);

  // find lowest element by iterating from last row begin to end of list
  T lowest = list[lastRowIndexBegin];

  for (int i = lastRowIndexBegin; i < list.size(); i++) {
    if (list[i] < lowest) {
      lowest = list[i];
    }
  }

  return lowest;
}

// a and b are zero-based indices
template<typename T>
void Heap<T>::swapNodes(int a, int b) {
  T buffer = list[a];
  list[a] = list[b];
  list[b] = buffer;
  return;
}

template<typename T>
int Heap<T>::indexOfParent(int i) {
  return floor(i / 2.0);
}

template<typename T>
int Heap<T>::indexOfRightChild(int i) {
  return (i * 2) + 1;
}

template<typename T>
int Heap<T>::indexOfLeftChild(int i) {
  return (i * 2);
}

int main() {
  /* Enter your code here. Read input from STDIN. Print output to STDOUT */

  Heap<int> heap = Heap<int>();

  heap.insert(4);
  heap.insert(2);
  heap.insert(1);
  heap.insert(3);
  heap.insert(8);
  heap.insert(10);

  cout << heap.getMinimum() << endl;
  heap.print();

  return 0;
}
