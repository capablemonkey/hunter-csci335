#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>

using namespace std;

// class node {
//   string filename;
// };

// class Edge {

// };

// class Graph {
// private:

// public:
//   getNeighborsFor(node v)
// };

class Graph {
public:
  void addEdge(string u, string v) {
    adjacency_list[u].push_back(v);
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

private:
  set<string> nodes;
  unordered_map<string, vector<string> > adjacency_list;
  unordered_map<string, int > pre;
  unordered_map<string, int > post;
  unordered_map<string, bool> visited;
  int clock = 1;

  vector<string> neighborsFor(string node) {
    return adjacency_list[node];
  };

  void explore(string node) {
    visited[node] = true;

    // pre-visit
    pre[node] = clock;
    clock++;

    cout << "visited " << node << endl;

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

  cout << "foo" << endl;
  return 0;
}