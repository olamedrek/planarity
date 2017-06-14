
#include <algorithm>
#include "Embedding.h"

Embedding::Embedding() {}

Embedding::Embedding(Graph G) : G(G) {
    int n = G.n;
    embedded.resize(n, vector<bool>(n));
    vertex_faces.resize(n);
    vertex_on_face.resize(n, vector<bool>(G.num_of_edges() - n + 2));
}

void Embedding::embed_initial_cycle(vector<int> &cycle) {
    faces.resize(2);

    for(int i = 0; i < (int)cycle.size()-1; i++) {
        int u = cycle[i], v = cycle[i+1];
        embedded[u][v] = embedded[v][u] = true;
        vertex_on_face[u][0] = vertex_on_face[u][1] = true;
        faces[0].push_back(u);
        faces[1].push_back(u);
        vertex_faces[u].push_back(0);
        vertex_faces[u].push_back(1);
    }
}

void Embedding::embed_path(vector<int> path, int face) {
    int a = path[0], b = path.back();
    vector<int> a_to_b = get_face_fragment(face, a, b);
    vector<int> b_to_a = get_face_fragment(face, b, a);

    for(int i = 0; i < path.size()-1; i++) {
        int u = path[i], v = path[i+1];
        embedded[u][v] = embedded[v][u] = 1;
    }

    int new_face = faces.size();
    faces.push_back(vector<int>());
    faces[face].clear();

    copy(b_to_a.begin(), b_to_a.end(), back_inserter(faces[new_face]));
    copy(path.begin(), path.end(), back_inserter(faces[new_face]));
    reverse(path.begin(), path.end());
    copy(a_to_b.begin(), a_to_b.end(), back_inserter(faces[face]));
    copy(path.begin(), path.end(), back_inserter(faces[face]));

    for(int v : b_to_a) {
        for(int i = 0; i < vertex_faces[v].size(); i++) {
            if(vertex_faces[v][i] == face) {
                swap(vertex_faces[v][i], vertex_faces[v].back());
                vertex_faces[v].pop_back();
                break;
            }
        }
        vertex_on_face[v][face] = false;
        vertex_on_face[v][new_face] = true;
        vertex_faces[v].push_back(new_face);
    }

    for(int v : path) {
        vertex_faces[v].push_back(new_face);
        vertex_on_face[v][new_face] = true;
        if(v != a && v != b) {
            vertex_faces[v].push_back(face);
            vertex_on_face[v][face] = true;
        }
    }
}

bool Embedding::is_vertex_embedded(int v) {
    return vertex_faces[v].size() > 0;
}

bool Embedding::is_edge_embedded(int e) {
    return embedded[G.edges[e].first][G.edges[e].second];
}

void Embedding::triangulate() {
    int face = 0;

    while(face < faces.size()) {
        if(faces[face].size() > 3) {

            bool done = false;
            for(int i = 0; i < faces[face].size(); i++) {
                for(int j = i+1; j < faces[face].size(); j++) {
                    int v = faces[face][i];
                    int u = faces[face][j];

                    if(!G.matrix[v][u]) {
                        G.add_edge(v, u);
                        embed_path(vector<int>({v, u}), face);
                        done = true;
                        break;
                    }
                }
                if(done) break;
            }
        } else {
            face++;
        }
    }
}

bool Embedding::belongs(int v, int face) {
    return vertex_on_face[v][face];
}

vector<int> Embedding::get_face_fragment(int face, int a, int b) {
    vector<int> result;
    int a_pos, b_pos;
    int size = faces[face].size();

    for(int i = 0; i < size; i++) {
        if(faces[face][i] == a) {
            a_pos = i;
        }
        if(faces[face][i] == b) {
            b_pos = i;
        }
    }

    for(int i = (a_pos + 1) % size; i != b_pos; i = (i+1) % size) {
        result.push_back(faces[face][i]);
    }
    return result;
}