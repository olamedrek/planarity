
#include <bits/stdc++.h>
#include "Graph.h"

using namespace std;

Graph::Graph() {}

Graph::Graph(int n) : n(n) {
    adj_list.resize(n);
    matrix.resize(n, vector<bool>(n));
}

void Graph::add_edge(int u, int v) {
    int e = edges.size();
    edges.push_back({u, v});
    adj_list[u].push_back(e);
    adj_list[v].push_back(e);
    matrix[u][v] = matrix[v][u] = true;
}

void Graph::add_edge(pair<int,int> p) {
    add_edge(p.first, p.second);
}

int Graph::num_of_edges() const {
    return edges.size();
}

int Graph::get_neighbour(int v, int e) const {
    return edges[e].first == v ? edges[e].second : edges[e].first;
}

vector<int> Graph::get_neighbours(int v) const {
    vector<int> neighbours;
    for(int e : adj_list[v]) {
        neighbours.push_back(get_neighbour(v, e));
    }
    return neighbours;
}

vector<int> Graph::get_cycle() const {
    vector<int> cycle;
    vector<int> color(n, 0);

    for(int i = 0; i < n; i++) {
        if(color[i] == 0) {
            if(find_cycle_dfs(i, -1, color, cycle)) {
                return cycle;
            }
        }
    }
}

vector<int> Graph::get_edge_ends(int e) const {
    return {edges[e].first, edges[e].second};
}

vector<Graph> Graph::get_biconnected_components() const {
    stack<int> edges_stack;
    vector<bool> visited(n);
    vector<int> parent(n, -1), depth(n), low(n), compress(n, -1);
    vector<Graph> components;

    for(int i = 0; i < n; i++) {
        if(!visited[i]) {
            biconnected_components_dfs(i, parent, visited, edges_stack, 0, depth, low, components, compress);
        }
    }
    return components;
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

void Graph::biconnected_components_dfs(int u, vector<int> &parent, vector<bool> &visited, stack<int> &edges_stack,
                                       int count, vector<int> &depth, vector<int> &low, vector<Graph> &components,
                                       vector<int> &compress) const {
    visited[u] = true;
    count++;
    depth[u] = count;
    low[u] = depth[u];

    for(int e : adj_list[u]) {
        int v = get_neighbour(u, e);
        if(!visited[v]) {
            edges_stack.push(e);
            parent[v] = u;
            biconnected_components_dfs(v, parent, visited, edges_stack, count, depth, low, components, compress);

            if(low[v] >= depth[u]) {
                vector<int> component_edges;
                int edge;
                do {
                    edge = edges_stack.top();
                    component_edges.push_back(edge);
                    edges_stack.pop();
                } while(edge != e);

                Graph C = get_compressed_graph(component_edges, compress);
                components.push_back(C);
            }
            low[u] = min(low[u], low[v]);
        } else if(parent[u] != v && depth[v] < depth[u]){
            edges_stack.push(e);
            low[u] = min(low[u], depth[v]);
        }
    }
}

Graph Graph::get_compressed_graph(vector<int> &component_edges, vector<int> &compress) const {
    int id = 0;
    for(int e : component_edges) {
        for(int v : get_edge_ends(e)) {
            if (compress[v] == -1) {
                compress[v] = id;
                id++;
            }
        }
    }

    Graph C(id);
    for(int e : component_edges) {
        C.add_edge(compress[edges[e].first], compress[edges[e].second]);
    }

    for(int e : component_edges) {
        for(int v : get_edge_ends(e)) {
            compress[v] = -1;
        }
    }

    return C;
}