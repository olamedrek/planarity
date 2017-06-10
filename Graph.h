
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

    void add_edge(pair<int,int> p);

    int num_of_edges() const;

    int get_neighbour(int v, int edge_id) const;

    vector<int> get_neighbours(int v) const;

    vector<int> get_cycle() const;

    vector<int> get_edge_ends(int edge_id) const;

    vector<Graph> get_biconnected_components() const;

private:
    bool find_cycle_dfs(int v, int parent, vector<int> &color, vector<int> &cycle) const;

    void biconnected_components_dfs(int u, vector<int> &parent, vector<bool> &visited, stack<int> &edges_stack,
                                    int count, vector<int> &depth, vector<int> &low, vector<Graph> &components,
                                    vector<int> &compress) const;

    Graph get_compressed_graph(vector<int> &component_edges, vector<int> &compress) const;
};

#endif //PLANARITY_GRAPH_H