
#include "PlanarityTester.h"

PlanarityTester::PlanarityTester(const Graph &G) : G(G), embedding(G) {
    n = G.n;
    m = G.num_of_edges();
}

pair<bool,Embedding> PlanarityTester::test() {
    if(G.edges.size() <= 1) {
        return {true, Embedding()};
    }
    if(G.edges.size() > 3 * n - 6) {
        return {false, Embedding()};
    }

    vector<int> cycle = G.get_cycle();
    embedding.embed_initial_cycle(cycle);

    while(true) {
        vector<vector<int>> fragments = get_fragments();
        if(fragments.size() == 0) break;

        vector<vector<int>> contact_vertices = get_contact_vertices(fragments);
        vector<vector<int>> admissible_faces = get_admissible_faces(fragments, contact_vertices);

        int chosen_fragment = -1;
        for(int i = 0; i < fragments.size(); i++) {
            if(admissible_faces[i].size() == 0) {
                return {false, Embedding()};
            } else if(admissible_faces[i].size() == 1) {
                chosen_fragment = i;
            }
        }
        if(chosen_fragment == -1) chosen_fragment = 0;

        Graph fragment(n);
        for(int edge_id : fragments[chosen_fragment]) {
            fragment.add_edge(G.edges[edge_id]);
        }

        vector<bool> contact(n);
        for(int v : contact_vertices[chosen_fragment]) {
            contact[v] = true;
        }

        vector<int> alpha_path = get_alpha_path(fragment, contact);
        int face_id = admissible_faces[chosen_fragment][0];
        embedding.embed_path(alpha_path, face_id);
    }
    return {true, embedding};
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
        int u = G.edges[i].first;
        int v = G.edges[i].second;
        bool u_embedded = embedding.is_vertex_embedded(u);
        bool v_embedded = embedding.is_vertex_embedded(v);

        if(!visited[i] && !embedding.is_edge_embedded(i) && (u_embedded || v_embedded)) {
            fragments.push_back(vector<int>());
            int start_vertex = u_embedded ? u : v;
            fragments_dfs(i, start_vertex, visited, fragments);
        }
    }
    return fragments;
}

vector<vector<int>> PlanarityTester::get_contact_vertices(vector<vector<int>> &fragments) {
    vector<vector<int>> contact_vertices(fragments.size());
    vector<bool> seen(n, 0);

    for(int i = 0; i < fragments.size(); i++) {
        for(int edge : fragments[i]) {
            for(int v : G.get_edge_ends(edge)) {
                if(embedding.is_vertex_embedded(v)) {
                    seen[v] = true;
                }
            }
        }
        for(int edge : fragments[i]) {
            for(int v : G.get_edge_ends(edge)) {
                if(seen[v]) {
                    contact_vertices[i].push_back(v);
                }
                seen[v] = false;
            }
        }
    }
    return contact_vertices;
}

vector<vector<int>> PlanarityTester::get_admissible_faces(vector<vector<int>> &fragments,
                                                          vector<vector<int>> &contact_vertices) {
    vector<vector<int>> admissible_faces(fragments.size());

    // TODO zapisywac przeciecia

    for(int i = 0; i < fragments.size(); i++) {
        int min_faces_num = embedding.faces.size();
        int min_vertex = contact_vertices[i][0];

        for(int v : contact_vertices[i]) {
            if(embedding.vertex_faces[v].size() < min_faces_num) {
                min_faces_num = embedding.vertex_faces[v].size();
                min_vertex = v;
            }
        }
        vector<int> candidates = embedding.vertex_faces[min_vertex];

        for(int face_id : candidates) {
            bool contain_all = true;
            for(int v : contact_vertices[i]) {
                if(!embedding.belongs(v, face_id)) {
                    contain_all = false;
                }
            }
            if(contain_all) admissible_faces[i].push_back(face_id);
        }
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
