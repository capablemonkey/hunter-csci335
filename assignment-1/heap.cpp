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

  // int indexOfLeftChild(int i);
  // int indexOfRightChild(int i);
  // int indexOfParent(int i);

public:
  Heap();
  void insert(T item);
  // void remove(T item);
  T getMinimum();
};

template<typename T>
Heap<T>::Heap() {

};

template<typename T>
void Heap<T>::insert(T item) {
  list.push_back(item);

  return;
};

template<typename T>
T Heap<T>::getMinimum() {
  return list.back();
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */

  cout << "hello" << endl;

  Heap<int> heap = Heap<int>();

  heap.insert(4);
  heap.insert(2);
  heap.insert(1);

  cout << heap.getMinimum() << endl;

  return 0;
}
