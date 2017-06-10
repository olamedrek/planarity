
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

void Graph::add_edge(pair<int,int> p) {
    add_edge(p.first, p.second);
}

int Graph::num_of_edges() const {
    return edges.size();
}

int Graph::get_neighbour(int v, int edge_id) const {
    return edges[edge_id].first == v ? edges[edge_id].second : edges[edge_id].first;
}

vector<int> Graph::get_neighbours(int v) const {
    vector<int> neighbours;
    for(int edge_id : adj_list[v]) {
        neighbours.push_back(get_neighbour(v, edge_id));
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
        cycle.clear();
    }
}

vector<int> Graph::get_edge_ends(int edge_id) const {
    return {edges[edge_id].first, edges[edge_id].second};
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

    for(int edge : adj_list[u]) {
        int v = get_neighbour(u, edge);
        if(!visited[v]) {
            edges_stack.push(edge);
            parent[v] = u;
            biconnected_components_dfs(v, parent, visited, edges_stack, count, depth, low, components, compress);

            if(low[v] >= depth[u]) {
                vector<int> component_edges;
                int e;
                do {
                    e = edges_stack.top();
                    component_edges.push_back(e);
                    edges_stack.pop();
                } while(e != edge);

                Graph C = get_compressed_graph(component_edges, compress);
                components.push_back(C);
            }
            low[u] = min(low[u], low[v]);
        } else if(parent[u] != v && depth[v] < depth[u]){
            edges_stack.push(edge);
            low[u] = min(low[u], depth[v]);
        }
    }
}

Graph Graph::get_compressed_graph(vector<int> &component_edges, vector<int> &compress) const {
    int cnt = 0;
    for(int edge_id : component_edges) {
        for(int v : get_edge_ends(edge_id)) {
            if (compress[v] == -1) {
                compress[v] = cnt;
                cnt++;
            }
        }
    }

    Graph C(cnt);
    for(int edge_id : component_edges) {
        C.add_edge(compress[edges[edge_id].first], compress[edges[edge_id].second]);
    }

    for(int edge_id : component_edges) {
        for(int edge_end : get_edge_ends(edge_id)) {
            compress[edge_end] = -1;
        }
    }

    return C;
}