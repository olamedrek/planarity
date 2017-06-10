
#include "PlanarityTester.h"

PlanarityTester::PlanarityTester(const Graph &G) : G(G), embedding(G) {
    n = G.n;
    m = G.num_of_edges();
}

void PlanarityTester::fragments_dfs(int e, int v, vector<bool> &visited, vector<vector<int>> &fragments) {
    visited[e] = true;
    fragments.back().push_back(e);

    int u = G.get_neighbour(v, e);

    if(embedding.is_vertex_embedded(u)) return;

    for(int edge : G.adj_list[u]) {
        if(!visited[edge] && !embedding.is_edge_embedded(edge)) {
            fragments_dfs(edge, u, visited, fragments);
        }
    }
}

vector<vector<int>> PlanarityTester::get_fragments() {
    vector<vector<int>> fragments;
    vector<bool> visited(m);

    for(int i = 0; i < m; i++) {
        int u = G.edges[i].first, v = G.edges[i].second;
        if(!visited[i] && !embedding.is_edge_embedded(i) && (embedding.is_vertex_embedded(u) || embedding.is_vertex_embedded(v))) {
            fragments.push_back(vector<int>());
            if(embedding.is_vertex_embedded(v)) swap(u, v);
            fragments_dfs(i, u, visited, fragments);
        }
    }
    return fragments;
}

vector<vector<int>> PlanarityTester::get_contact_vertices(vector<vector<int>> &fragments) {
    vector<vector<int>> contact_vertices(fragments.size());
    vector<bool> seen(n, 0);

    for(int i = 0; i < fragments.size(); i++) {
        for(int edge : fragments[i]) for(int v : G.get_edge_ends(edge)) {
            if(embedding.is_vertex_embedded(v)) seen[v] = true;
        }

        for(int edge : fragments[i]) for(int v : G.get_edge_ends(edge)) {
            if(seen[v]) {
                contact_vertices[i].push_back(v);
            }
            seen[v] = false;
        }
    }
    return contact_vertices;
}

vector<vector<int>> PlanarityTester::get_admissible_faces(vector<vector<int>> &fragments,
                                                          vector<vector<int>> &contact_vertices) {
    vector<vector<int>> admissible_faces(fragments.size());
    vector<int> common(embedding.faces.size(), 0);

    // TODO zapisywac przeciecia

    for(int i = 0; i < fragments.size(); i++) {
        int min_faces_num = 1000000;
        int min_vertex = -1;

        for(int cv : contact_vertices[i]) {
            if(embedding.vertex_faces[cv].size() < min_faces_num) {
                min_faces_num = embedding.vertex_faces[cv].size();
                min_vertex = cv;
            }
        }

        vector<int> candidate_faces = embedding.vertex_faces[min_vertex];
        vector<int> final;

        for(int face_id : candidate_faces) {
            bool ok = true;
            for(int cv : contact_vertices[i]) {
                if(!embedding.belongs(cv, face_id)) ok = false;
            }
            if(ok) final.push_back(face_id);
        }

        admissible_faces[i] = final;
    }
    return admissible_faces;
}

bool PlanarityTester::alpha_path_dfs(int v, Graph &fragment, vector<bool> &visited, vector<bool> &contact,
                                     vector<int> &path) {
    visited[v] = true;

    for(int u : fragment.get_neighbours(v)) {
        if(!visited[u]) {
            if(contact[u]) {
                path.push_back(u);
                path.push_back(v);
                return true;
            }

            if(alpha_path_dfs(u, fragment, visited, contact, path)) {
                path.push_back(v);
                return true;
            }
        }
    }
    return false;
}

vector<int> PlanarityTester::get_alpha_path(Graph &fragment, vector<bool> &contact) {
    vector<int> path;
    vector<bool> visited(n);

    for(int i = 0; i < contact.size(); i++) {
        if(contact[i]) {
            alpha_path_dfs(i, fragment, visited, contact, path);
            break;
        }
    }
    return path;
}

int PlanarityTester::one_iteration() {
    vector<vector<int>> fragments = get_fragments();

    if(fragments.size() == 0) return 2;

    vector<vector<int>> contact_vertices = get_contact_vertices(fragments);
    vector<vector<int>> admissible_faces = get_admissible_faces(fragments, contact_vertices);

    for(int i = 0; i < fragments.size(); i++) {
        if(admissible_faces[i].size() == 0) {
            return 0;
        }
    }

    int chosen = -1;

    for(int i = 0; i < admissible_faces.size(); i++) {
        if(admissible_faces[i].size() == 1) {
            chosen = i;
            break;
        }
    }
    if(chosen == -1) chosen = 0;

    Graph fragment(n);
    for(int edge_id : fragments[chosen]) {
        fragment.add_edge(G.edges[edge_id]);
    }

    vector<bool> contact(n);
    for(int v : contact_vertices[chosen]) {
        contact[v] = true;
    }

    vector<int> alpha_path = get_alpha_path(fragment, contact);
    int face_id = admissible_faces[chosen][0];

    embedding.embed_path(alpha_path, face_id);

    return 1;
}

tuple<bool,Embedding> PlanarityTester::test() {
    if(G.edges.size() <= 1) {
        return make_tuple(true, Embedding());
    }

    // if edges > 3n - 6 return false

    vector<int> cycle = G.get_cycle();
    embedding.embed_initial_cycle(cycle);

    while(true) {
        int result = one_iteration();
        if(result == 0) return make_tuple(false, Embedding());
        if(result == 2) break;
    }
    return make_tuple(true, embedding);
}