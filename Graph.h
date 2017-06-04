
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

    bool two_connected() const;

private:
    bool find_cycle_dfs(int v, int parent, vector<int> &color, vector<int> &cycle) const;

    bool two_connected_dfs(int v, int parent, vector<bool> &visited, vector<int> &rank, vector<int> &depth) const;
};


#endif //PLANARITY_GRAPH_H
