
#ifndef PLANARITY_GRAPH_H
#define PLANARITY_GRAPH_H

#include <bits/stdc++.h>

using namespace std;

class Graph {
public:
    int n;
    vector<vector<int>> adj_list;
    vector<pair<int,int>> edges;
    vector<vector<bool>> matrix;

    Graph();

    Graph(int n);

    void add_edge(int u, int v);

    int num_of_edges() const;

    int get_neighbour(int v, int edge_id) const;

    vector<int> get_neighbours(int v) const;

    vector<int> get_cycle() const;

    tuple<int,int> get_edge_ends(int edge_id) const;

    vector<Graph> get_biconnected_components() const;

private:
    bool find_cycle_dfs(int v, int parent, vector<int> &color, vector<int> &cycle) const;

    void biconnected_components_dfs(stack<int> &S, int u, vector<int> &parent, vector<bool> &visited, int count, vector<int> &d, vector<int> &low, vector<Graph> &comps) const;
};


#endif //PLANARITY_GRAPH_H
