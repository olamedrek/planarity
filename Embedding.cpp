
#include <algorithm>
#include "Embedding.h"

Embedding::Embedding() {}

Embedding::Embedding(Graph G) : G(G) {
    embedded.resize(G.n, vector<bool>(G.n));
    vertex_faces.resize(G.n);
}

void Embedding::embed_initial_cycle(vector<int> &cycle) {
    faces.resize(2);

    for(int i = 0; i < cycle.size()-1; i++) {
        int u = cycle[i], v = cycle[i+1];
        embedded[u][v] = embedded[v][u] = 1;
        faces[0].push_back(u);
        faces[1].push_back(u);
        vertex_faces[u].push_back(0);
        vertex_faces[u].push_back(1);
    }
}

vector<int> Embedding::get_face_fragment(int face_id, int a, int b) {
    vector<int> result;
    int a_pos, b_pos;
    int size = faces[face_id].size();

    for(int i = 0; i < size; i++) {
        if(faces[face_id][i] == a) {
            a_pos = i;
        }
        if(faces[face_id][i] == b) {
            b_pos = i;
        }
    }

    for(int i = (a_pos + 1) % size; i != b_pos; i = (i+1) % size) {
        result.push_back(faces[face_id][i]);
    }

    return result;
}

void Embedding::embed_path(vector<int> path, int face_id) {
    int a = path[0], b = path.back();
    vector<int> a_to_b = get_face_fragment(face_id, a, b);
    vector<int> b_to_a = get_face_fragment(face_id, b, a);

    for(int i = 0; i < path.size()-1; i++) {
        int u = path[i], v = path[i+1];
        embedded[u][v] = embedded[v][u] = 1;
    }

    int new_face_id = faces.size();
    faces.push_back(vector<int>());
    faces[face_id].clear();

    copy(b_to_a.begin(), b_to_a.end(), back_inserter(faces[new_face_id]));
    copy(path.begin(), path.end(), back_inserter(faces[new_face_id]));
    reverse(path.begin(), path.end());
    copy(a_to_b.begin(), a_to_b.end(), back_inserter(faces[face_id]));
    copy(path.begin(), path.end(), back_inserter(faces[face_id]));

    for(int v : b_to_a) {
        for(int i = 0; i < vertex_faces[v].size(); i++) {
            if(vertex_faces[v][i] == face_id) {
                swap(vertex_faces[v][i], vertex_faces[v].back());
                vertex_faces[v].pop_back();
                break;
            }
        }
        vertex_faces[v].push_back(new_face_id);
    }

    for(int v : path) {
        vertex_faces[v].push_back(new_face_id);
        if(v != a && v != b) {
            vertex_faces[v].push_back(face_id);
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
    int face_id = 0;

    while(face_id < faces.size()) {
        if(faces[face_id].size() > 3) {

            bool done = false;
            for(int i = 0; i < faces[face_id].size(); i++) {
                for(int j = i+1; j < faces[face_id].size(); j++) {
                    int v = faces[face_id][i];
                    int u = faces[face_id][j];

                    if(!G.matrix[v][u]) {
                        G.add_edge(v, u);
                        embed_path(vector<int>({v, u}), face_id);
                        done = true;
                        break;
                    }
                }
                if(done) break;
            }
        } else {
            face_id++;
        }
    }
}