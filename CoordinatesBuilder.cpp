
#include "CoordinatesBuilder.h"

CoordinatesBuilder::CoordinatesBuilder(Embedding embedding) : embedding(embedding) {}

pair<int,int> intersection(pair<int,int> &a, pair<int,int> &b) {
    return {(a.first - a.second + b.first + b.second) / 2, (-a.first + a.second + b.first + b.second) / 2};
};

vector<pair<int,int>> CoordinatesBuilder::get_coordinates() {
    Graph& G = embedding.G;

    if(G.n == 1 && G.num_of_edges() == 0) {
        return {make_pair(0,0)};
    }
    if(G.n == 2 && G.num_of_edges() == 1) {
        return {make_pair(0,0), make_pair(1,0)};
    }

    embedding.triangulate();

    vector<int> order = get_canonical_order();

    vector<vector<int>> dependent(G.n);
    vector<pair<int,int>> coordinates(G.n);
    vector<int> contour;

    vector<pair<int,int>> init_pos = {{0, 0}, {2, 0}, {1, 1}};
    for(int i = 0; i < 3; i++) {
        coordinates[order[i]] = init_pos[i];
        dependent[order[i]].push_back(order[i]);
    }

    contour.push_back(order[0]);
    contour.push_back(order[2]);
    contour.push_back(order[1]);

    for(int i = 3; i < order.size(); i++) {
        int v = order[i];

        int p, q;
        for(int j = 0; j < contour.size(); j++) {
            if (G.matrix[v][contour[j]]) {
                p = j;
                break;
            }
        }
        for(int j = (int)contour.size() - 1; j >= 0; j--) {
            if (G.matrix[v][contour[j]]) {
                q = j;
                break;
            }
        }
        for(int j = p+1; j < contour.size(); j++) {
            for(int u : dependent[contour[j]]) {
                coordinates[u].first += j < q ? 1 : 2;
            }
        }

        coordinates[v] = intersection(coordinates[contour[p]], coordinates[contour[q]]);
        dependent[v].push_back(v);

        for(int j = p+1; j < q; j++) {
            for(int u : dependent[contour[j]]) {
                dependent[v].push_back(u);
            }
        }

        vector<int> tmp;
        for(int j = 0; j < contour.size(); j++) {
            if(j <= p) tmp.push_back(contour[j]);
            if(j == p+1) tmp.push_back(v);
            if(j >= q) tmp.push_back(contour[j]);
        }
        contour = tmp;
    }

    return coordinates;
};

vector<int> CoordinatesBuilder::get_canonical_order() {
    Graph& G = embedding.G;
    vector<vector<int>>& faces = embedding.faces;
    int n = embedding.G.n;

    vector<int> order;
    vector<bool> inc_chord(n), out(n), in_order(n);

    for(int v : faces[0]) {
        out[v] = true;
    }

    int v1 = faces[0][0], v2 = faces[0][1];

    while(order.size() < n-2) {
        int v;
        for(int i = 0; i < n; i++) {
            if(i != v1 && i != v2 && !in_order[i] && out[i] && inc_chord[i] == 0) {
                v = i;
                break;
            }
        }

        order.push_back(v);
        in_order[v] = true;
        out[v] = false;

        for(int u : G.get_neighbours(v)) {
            if(!in_order[u]) {
                out[u] = true;
            }
        }

        vector<int> nbrs_on_outer(n, 0);
        for(int u = 0; u < n; u++) {
            for(int t : G.get_neighbours(u)) {
                if(out[t]) {
                    nbrs_on_outer[u]++;
                }
            }
        }
        for(int u = 0; u < n; u++) {
            if(out[u]) {
                inc_chord[u] = nbrs_on_outer[u] > 2;
            }
        }
    }

    reverse(order.begin(), order.end());
    order.insert(order.begin(), {v1, v2});

    return order;
}