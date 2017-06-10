
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

    for(int i = 0; i < n; i++) {
        if(color[i] == 0) {
            if(find_cycle_dfs(i, -1, color, cycle))
                return cycle;
        }
        cycle.clear();
    }
}

tuple<int,int> Graph::get_edge_ends(int edge_id) const {
    return edges[edge_id];
}

void Graph::biconnected_components_dfs(stack<int> &S, int u, vector<int> &parent, vector<bool> &visited, int count,
                                       vector<int> &d, vector<int> &low, vector<Graph> &comps) const {
    visited[u] = true;
    count++;
    d[u] = count;
    low[u] = d[u];

    for(int edge : adj_list[u]) {
        int v = get_neighbour(u, edge);
        if(!visited[v]) {
            S.push(edge);
            parent[v] = u;
            biconnected_components_dfs(S, v, parent, visited, count, d, low, comps);
            if(low[v] >= d[u]) {
                Graph comp(n); // TODO : compress
                while(S.size() > 0) {
                    int e = S.top();
                    S.pop();
                    comp.add_edge(edges[e].first, edges[e].second);
                    if(e == edge) break;
                }
                comps.push_back(comp);
            }
            low[u] = min(low[u], low[v]);
        } else if(parent[u] != v && d[v] < d[u]){
            S.push(edge);
            low[u] = min(low[u], d[v]);
        }
    }
}

vector<Graph> Graph::get_biconnected_components() const {
    int count = 0;
    stack<int> S;
    vector<bool> visited(n);
    vector<int> parent(n, -1), d(n), low(n);
    vector<Graph> comps;

    for(int i = 0; i < n; i++) {
        if(!visited[i]) {
            biconnected_components_dfs(S, i, parent, visited, count, d, low, comps);
        }
    }
    return comps;
}


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



