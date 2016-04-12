#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Graph {
public:
  void addEdge(string u, string v) {
    adjacencyList[u].push_back(v);
  };

  void addNode(string u) {
    nodes.insert(u);
  }

  void depthFirstSearch(string node) {
    // initialize visited to false for all nodes:
    set<string>::iterator it;
    for (it = nodes.begin(); it != nodes.end(); it++) {
      visited[*it] = false;
    }

    for (it = nodes.begin(); it != nodes.end(); it++) {
      if (visited[*it] == false) { explore(*it); }
    }

    return;
  };

  bool isCyclic() {
    // returns true if there is a back edge.
    set<string>::iterator u;
    for (u = nodes.begin(); u != nodes.end(); u++) {
      visited[*u] = false;

      // look at every edge and see if for edge (u,v) pre[u] > pre[v] && post[u] < post[v]:
      vector<string> neighbors = neighborsFor(*u);
      vector<string>::iterator v;
      for (v = neighbors.begin(); v!= neighbors.end(); v++) {
        if (pre[*u] > pre[*v] && post[*u] < post[*v]) { return true; }
      }
    }

    return false;
  }

  void printTopologicalSort() {
    // build vector of (postNumber, node) pairs
    vector< pair<int, string> > postNums;
    set<string>::iterator node;
    for (node = nodes.begin(); node != nodes.end(); node++) {
      postNums.push_back(make_pair(post[*node], *node));
    }

    // Sort nodes by ascending post number:
    sort(postNums.begin(), postNums.end(),
      [](const pair<int, string> & a, const pair<int, string> & b) -> bool {
        return get<0>(a) < get<0>(b);
      });

    // Print nodes in topological order:
    vector< pair<int, string> >::iterator it;
    for (it = postNums.begin(); it != postNums.end(); it++) {
      cout << get<0>(*it) << " " << get<1>(*it) << endl;
    }
  }

private:
  set<string> nodes;
  unordered_map<string, vector<string> > adjacencyList;
  unordered_map<string, int > pre;
  unordered_map<string, int > post;
  unordered_map<string, bool> visited;
  int clock = 1;

  vector<string> neighborsFor(string node) {
    return adjacencyList[node];
  };

  void explore(string node) {
    visited[node] = true;

    // pre-visit
    pre[node] = clock;
    clock++;

    // visit neighbors:
    vector<string> neighbors = neighborsFor(node);
    vector<string>::iterator it;

    for (it = neighbors.begin(); it != neighbors.end(); it++) {
      if (visited[*it] == false) { explore(*it); }
    }

    // post-visit
    post[node] = clock;
    clock++;
  };
};

int main() {
  Graph g = Graph();

  g.addNode("a");
  g.addNode("b");
  g.addNode("c");

  g.addEdge("a", "b");
  g.addEdge("a", "c");
  g.addEdge("b", "c");

  g.depthFirstSearch("a");

  if(g.isCyclic()) {
    cout << "Circular dependency found!" << endl;
  } else {
    g.printTopologicalSort();
  }

  return 0;
}