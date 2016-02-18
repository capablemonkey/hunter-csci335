/*
  HackerRank Challenge: Minimum Average Waiting Time

  https://www.hackerrank.com/challenges/minimum-average-waiting-time
 */

#include <cmath>
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
  void removeNodeAt(int i);

public:
  void insert(T item);
  T popMin();
  bool empty();
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
bool Heap<T>::empty() {
  return list.size() == 0;
}

template<typename T>
void Heap<T>::removeNodeAt(int i) {
  int indexOfLastItem = list.size() - 1;

  swapNodes(indexOfLastItem, i);

  // delete the last item:
  list.pop_back();

  // fix the heap. swap node with max child until heap is valid
  fixNodeAt(i);
  return;
}

// Given the index of a node whose position in the heap is invalid, fix the 
// heap by moving the node to its correct position.  If a child node is greater 
// than the node in question and the other child node, swap them.  Reapeat this 
// recursively until the node in question is greater than both of its children.
template<typename T>
void Heap<T>::fixNodeAt(int i) {
  // if leaf node, nothing to do
  if (indexOfLeftChild(i) >= list.size()) {
    return;
  }

  // find index of max child
  bool rightChildWithinBounds = indexOfRightChild(i) < list.size();
  bool rightChildGreater = rightChildWithinBounds &&
                           list[indexOfRightChild(i)] > list[indexOfLeftChild(i)];
  int indexOfMaxChild = rightChildGreater ? indexOfRightChild(i) : indexOfLeftChild(i);

  // if max child is greater than node, swap them
  if (list[indexOfMaxChild] > list[i]) {
    swapNodes(i, indexOfMaxChild);
  }

  return fixNodeAt(indexOfMaxChild);
}

template<typename T>
void Heap<T>::insert(T item) {
  list.push_back(item);

  int i = list.size() - 1;

  while(i > 0 && *item > *list[indexOfParent(i)]) {
    swapNodes(i, indexOfParent(i));
    i = indexOfParent(i);
  }

  return;
};

// linear search through leaf nodes for smallest.
template<typename T>
T Heap<T>::popMin() {
  // calculate beginning index of leaf node sequence
  // (last node).parent + 1
  int indexLastNode = list.size() - 1;
  int indexLeavesBegin = indexOfParent(indexLastNode) + 1;

  // find lowest element by iterating from first leaf to last
  T lowest = list[indexLeavesBegin];
  int indexLowest = indexLeavesBegin;

  for (int i = indexLeavesBegin; i < list.size(); i++) {
    if (*list[i] < *lowest) {
      lowest = list[i];
      indexLowest = i;
    }
  }

  removeNodeAt(indexLowest);
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

class Order {
private:
  int timeCreated;
  int timeCost;
  int timeCookProgress = 0;
  int timeStartedCooking;
  int timeFinishedCooking;

public:
  Order(int created, int cost) {
    timeCreated = created;
    timeCost = cost;
  }

  void startCooking(int timeStart) {
    timeStartedCooking = timeStart;
  }

  void finishCooking(int timeEnd) {
    timeFinishedCooking = timeEnd;
  }

  int getTimeCreated() { return timeCreated; }
  int getTimeCost() { return timeCost; }

  bool isCooked() { return timeCookProgress == timeCost; }
  void cookOneTurn() { timeCookProgress++; }

  int getWaitTime() {
    return timeFinishedCooking - timeCreated;
  }

  friend bool operator <(const Order &left, const Order &right) {
    return left.timeCost < right.timeCost;
  }

  friend bool operator >(const Order &left, const Order &right) {
    return left.timeCost > right.timeCost;
  }
};

int main() {
  Heap<Order*> orderQueue = Heap<Order*>();
  vector<Order*> newOrders, finishedOrders;
  string inputLine;
  int timeCreated, timeCost;

  // skip first line; we rely on failbit instead of line count
  getline(cin, inputLine);

  while(getline(cin, inputLine)) {
    stringstream ss(inputLine);

    ss >> timeCreated;
    ss >> timeCost;

    Order *o = new Order(timeCreated, timeCost);
    newOrders.push_back(o);
  }

  // reverse so we can later pop_back to get the earliest order
  reverse(newOrders.begin(), newOrders.end());

  // begin simulation
  int currentTime = 0;
  int totalOrders = newOrders.size();

  Order *orderBeingCooked = NULL;

  while(finishedOrders.size() < totalOrders) {
    cout << "Current time: " << currentTime << endl;
    cout << "New orders: " << newOrders.size() << endl;

    // check for an order created at this time and add to heap
    if (newOrders.empty() == false) {
      Order *earliestOrder = newOrders[newOrders.size() - 1];
      if (earliestOrder->getTimeCreated() == currentTime) {
        cout << "Received new order with cost" << earliestOrder->getTimeCost() << endl;
        orderQueue.insert(earliestOrder);
        newOrders.pop_back();
      }
    }

    if (currentTime == 0) {
      orderBeingCooked = orderQueue.popMin();
      orderBeingCooked->startCooking(currentTime);
    }

    if (orderBeingCooked->isCooked() == true) {
      orderBeingCooked->finishCooking(currentTime);
      finishedOrders.push_back(orderBeingCooked);

      // start cooking the next order
      if (orderQueue.empty() == false) {
        orderBeingCooked = orderQueue.popMin();
        orderBeingCooked->startCooking(currentTime);
        cout << "Now cooking pizza with cost" << orderBeingCooked->getTimeCost() << endl;
      }
    }

    cout << "Finished orders: " << finishedOrders.size() << endl << endl;

    orderBeingCooked->cookOneTurn();
    currentTime++;
  }

  cout << finishedOrders[0]->getWaitTime() << endl;
  cout << finishedOrders[1]->getWaitTime() << endl;
  cout << finishedOrders[2]->getWaitTime() << endl;

  // calculate average wait time
  int totalWaitTime = 0;
  for (int i = 0; i < finishedOrders.size(); i++) {
    totalWaitTime += finishedOrders[i]->getWaitTime();
  }

  cout << floor((totalWaitTime * 1.0) / finishedOrders.size()) << endl;

  return 0;
}