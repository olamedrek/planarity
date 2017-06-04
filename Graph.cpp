
#include <bits/stdc++.h>
#include "Graph.h"

using namespace std;

Graph::Graph() {}

Graph::Graph(int n) : n(n) {
    adj_list.resize(n);
    matrix.resize(n, vector<bool>(n));
}

void Graph::add_edge(int u, int v) {
    int edge_id = edges.size();
    edges.push_back({u, v});
    adj_list[u].push_back(edge_id);
    adj_list[v].push_back(edge_id);
    matrix[u][v] = matrix[v][u] = true;
}

int Graph::num_of_edges() const {
    return edges.size();
}

int Graph::get_neighbour(int v, int edge_id) const {
    return edges[edge_id].first == v ? edges[edge_id].second : edges[edge_id].first;
}

vector<int> Graph::get_neighbours(int v) const {
    vector<int> neighbours;
    for(int edgeNum : adj_list[v]) {
        neighbours.push_back(get_neighbour(v, edgeNum));
    }
    return neighbours;
}

vector<int> Graph::get_cycle() const {
    vector<int> cycle;
    vector<int> color(n, 0);
    find_cycle_dfs(0, -1, color, cycle);
    return cycle;
}

tuple<int,int> Graph::get_edge_ends(int edge_id) const {
    return edges[edge_id];
}

bool Graph::two_connected() const {}

bool Graph::find_cycle_dfs(int v, int parent, vector<int> &color, vector<int> &cycle) const {
    color[v] = 1;

    for(int u : get_neighbours(v)) if(u != parent) {
            if (color[u] == 1) {
                cycle.push_back(u);
                cycle.push_back(v);
                color[v] = 2;
                return true;
            } else if(color[u] == 0) {
                if (find_cycle_dfs(u, v, color, cycle)) {
                    if (color[cycle[0]] == 1) {
                        cycle.push_back(v);
                    }
                    color[v] = 2;
                    return true;
                }
            }
        }

    color[v] = 2;
    return false;
}

bool Graph::two_connected_dfs(int v, int parent, vector<bool> &visited, vector<int> &rank, vector<int> &depth) const {}


