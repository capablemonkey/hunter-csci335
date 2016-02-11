/*
  HackerRank Challenge: QHEAP1

  https://www.hackerrank.com/challenges/qheap1
 */

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;

template<typename T>
class Heap {
private:
  vector<T> list;

  int indexOfLeftChild(int i);
  int indexOfRightChild(int i);
  int indexOfParent(int i);
  int findIndex(T item);
  void swapNodes(int a, int b);
  void fixNodeAt(int i);

public:
  void insert(T item);
  void remove(T item);
  T getMinimum();
  void print();
};

// Sequentially seek for the element.  O(n) time.
template<typename T>
int Heap<T>::findIndex(T item) {
  for (int i = 0; i < list.size(); i++) {
    if (list[i] == item) return i;
  }

  return -1; // sentinel value if item not found
}

template<typename T>
void Heap<T>::remove(T item) {
  int indexOfItemToRemove = findIndex(item);
  int indexOfLastItem = list.size() - 1;

  swapNodes(indexOfLastItem, indexOfItemToRemove);

  // delete the last item:
  list.pop_back();

  // fix the heap.  compare the item with children; if a child is larger than it, swap.
  fixNodeAt(indexOfItemToRemove);
  return;
}

// Given the index of a node whose position in the heap is invalid, fix the 
// heap by moving the node to its correct position.  If a child node is greater 
// than the node in question and the other child node, swap them.  Reapeat this 
// recursively until the node in question is greater than both of its children.
template<typename T>
void Heap<T>::fixNodeAt(int i) {
  bool leftChildWithinBounds = indexOfLeftChild(i) < list.size();
  bool rightChildWithinBounds = indexOfRightChild(i) < list.size();
  bool leftChildIsMaxChild = (leftChildWithinBounds && rightChildWithinBounds) ? list[indexOfLeftChild(i)] > list[indexOfRightChild(i)] : true;
  bool rightChildIsMaxChild = (rightChildWithinBounds) ? list[indexOfRightChild(i)] > list[indexOfLeftChild(i)] : true;
  bool leftChildGreaterThanNode = leftChildWithinBounds ? list[indexOfLeftChild(i)] > list[i] : false;
  bool rightChildGreaterThanNode = rightChildWithinBounds ? list[indexOfRightChild(i)] > list[i] : false;

  // if left child is within bounds and left child is larger than current and the left child is greater than the right child
  if (leftChildGreaterThanNode && leftChildIsMaxChild) {
    swapNodes(i, indexOfLeftChild(i));
    return fixNodeAt(indexOfLeftChild(i));
  }

  if (rightChildGreaterThanNode && rightChildIsMaxChild) {
    swapNodes(i, indexOfRightChild(i));
    return fixNodeAt(indexOfRightChild(i));
  }

  return;
}

template<typename T>
void Heap<T>::print() {
  typedef typename vector<T>::iterator iterator;
  for (iterator it = list.begin(); it != list.end(); ++it) {
    cout << " " << *it;
  }

  cout << endl;
}

template<typename T>
void Heap<T>::insert(T item) {
  list.push_back(item);

  int i = list.size() - 1;

  while(i > 0 && item > list[indexOfParent(i)]) {
    swapNodes(i, indexOfParent(i));
    i = indexOfParent(i);
  }

  return;
};

// linear search through leaf nodes for smallest.
template<typename T>
T Heap<T>::getMinimum() {
  // calculate beginning index of leaf node sequence
  // (last node).parent + 1
  int indexLastNode = list.size() - 1;
  int indexLeavesBegin = indexOfParent(indexLastNode) + 1;

  // find lowest element by iterating from first leaf to last
  T lowest = list[indexLeavesBegin];

  for (int i = indexLeavesBegin; i < list.size(); i++) {
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
  return floor((i % 2 == 0 ? i - 1 : i) / 2.0);
}

template<typename T>
int Heap<T>::indexOfLeftChild(int i) {
  if (i == 0) { return 1; }

  return (i * 2) + 1;
}

template<typename T>
int Heap<T>::indexOfRightChild(int i) {
  if (i == 0) { return 2; }

  return (i * 2) + 2; 
}

int main() {
  Heap<int> heap = Heap<int>();

  // interface for HackerRank submission:
  int operation;
  int element;
  string input;

  // skip first line; we don't need to know length of input since we rely
  // on failbit
  getline(cin, input);

  while(getline(cin, input)) {
    stringstream ss(input);
    ss >> operation;
    ss >> element;

    cout << "=== heap print ";
    heap.print();

    if (operation == 1) {
      cout << "[ADD] " << element << endl;
      heap.insert(element);
    } else if (operation == 2) {
      cout << "[REMOVE] " << element << endl;
      heap.remove(element);
    } else {
      cout << "[MIN] = " << heap.getMinimum() << endl;
    }

  }

  return 0;
}
