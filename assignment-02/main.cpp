/*
  HackerRank Challenge: Minimum Average Waiting Time

  https://www.hackerrank.com/challenges/minimum-average-waiting-time
 */

#include <cmath>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
using namespace std;

/*
 * Heap implementation
 */

template<typename T>
class Heap {
private:
  vector<T> list;

  int indexOfLeftChild(int i);
  int indexOfRightChild(int i);
  int indexOfParent(int i);
  void swapNodes(int a, int b);
  void fixNodeAt(int i);
  void removeNodeAt(int i);

public:
  void insert(T item);
  T popMin();
  bool empty();
};

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

/*
 * Order implementation
 */

class Order {
private:
  long int timeCreated;
  long int timeCost;
  long int timeCookProgress = 0;
  long int timeStartedCooking;
  long int timeFinishedCooking;

public:
  Order(long int created, long int cost) {
    timeCreated = created;
    timeCost = cost;
  }

  void startCooking(long int timeStart) {
    timeStartedCooking = timeStart;
  }

  void finishCooking(long int timeEnd) {
    timeFinishedCooking = timeEnd;
  }

  long int getTimeCreated() { return timeCreated; }
  long int getTimeCost() { return timeCost; }

  bool isCooked() { return timeCookProgress == timeCost; }
  void cookOneTurn() { timeCookProgress++; }

  long int getWaitTime() {
    return timeFinishedCooking - timeCreated;
  }

  friend bool operator <(const Order &left, const Order &right) {
    return left.timeCost < right.timeCost;
  }

  friend bool operator >(const Order &left, const Order &right) {
    return left.timeCost > right.timeCost;
  }
};

struct {
  bool operator()(Order *a, Order *b) {
    return a->getTimeCreated() > b->getTimeCreated();
  }
} descendingTimeCreated;

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

  // sort so we can later pop_back to get the earliest order
  sort(newOrders.begin(), newOrders.end(), descendingTimeCreated);

  // begin simulation
  int totalOrders = newOrders.size();
  long int currentTime = newOrders[totalOrders - 1]->getTimeCreated();
  Order *orderBeingCooked = NULL;

  while(finishedOrders.size() < totalOrders) {
    // find orders created earlier than now and a) insert them into heap and b) pop them from newOrders
    for(int i = newOrders.size() - 1; i >= 0; i--) {
      if (newOrders[i]->getTimeCreated() <= currentTime) {
        orderQueue.insert(newOrders[i]);
        newOrders.pop_back();
      } else {
        break;
      }
    }

    orderBeingCooked = orderQueue.popMin();
    orderBeingCooked->startCooking(currentTime);

    cout << "At time " << currentTime << " started cooking " << orderBeingCooked->getTimeCost() << endl;

    currentTime += orderBeingCooked->getTimeCost();
    orderBeingCooked->finishCooking(currentTime);
    finishedOrders.push_back(orderBeingCooked);
  }

  // while(finishedOrders.size() < totalOrders) {
  //   // check for an order created at this time and add to heap
  //   if (newOrders.empty() == false) {
  //     Order *earliestOrder = newOrders[newOrders.size() - 1];
  //     if (earliestOrder->getTimeCreated() == currentTime) {
  //       orderQueue.insert(earliestOrder);
  //       newOrders.pop_back();
  //     }
  //   }

  //   if (currentTime == 0) {
  //     orderBeingCooked = orderQueue.popMin();
  //     orderBeingCooked->startCooking(currentTime);
  //   }

  //   if (orderBeingCooked->isCooked() == true) {
  //     orderBeingCooked->finishCooking(currentTime);
  //     finishedOrders.push_back(orderBeingCooked);

  //     // start cooking the next order
  //     if (orderQueue.empty() == false) {
  //       orderBeingCooked = orderQueue.popMin();
  //       orderBeingCooked->startCooking(currentTime);
  //     }
  //   }

  //   orderBeingCooked->cookOneTurn();
  //   currentTime++;
  // }

  cout << finishedOrders.size() << endl;
  cout << finishedOrders[0]->getTimeCreated() << endl;
  cout << finishedOrders[0]->getWaitTime() << endl;

  // calculate average wait time
  long int totalWaitTime = 0;

  for (int i = 0; i < finishedOrders.size(); i++) {
    totalWaitTime += finishedOrders[i]->getWaitTime();
  }

  long double average = (totalWaitTime * 1.0) / finishedOrders.size();

  cout << fixed << setprecision(0) << average << endl;

  return 0;
}