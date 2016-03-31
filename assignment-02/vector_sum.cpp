#include <iostream>
#include <vector>

using namespace std;

int main() {
  vector<int> numbers;
  numbers.push_back(1);
  numbers.push_back(2);
  numbers.push_back(3);

  vector<int>::iterator it;
  int sum = 0;

  for (it = numbers.begin(); it < numbers.end(); it++) {
    sum += *it;
  }

  cout << sum << endl;

  return 0;
}

