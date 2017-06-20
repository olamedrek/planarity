
#include "PlanarityTest.h"

PlanarityTest::PlanarityTest(const Graph &G) : G(G), embedding(G) {}

pair<bool,Embedding> PlanarityTest::test() {
    if(G.num_of_edges() <= 1) {
        embedding.embed_tree();
        return {true, embedding};
    }

    if(G.num_of_edges() > 3 * G.n - 6) {
        return {false, Embedding()};
    }

    vector<int> cycle = G.get_cycle();
    embedding.embed_initial_cycle(cycle);

    while(true) {
        vector<vector<int>> fragments = get_fragments();
        if(fragments.size() == 0) break;

        vector<vector<int>> contact_vertices = get_contact_vertices(fragments);
        vector<vector<int>> admissible_faces = get_admissible_faces(fragments, contact_vertices);

        int chosen_fragment = 0;
        for(int i = 0; i < fragments.size(); i++) {
            if(admissible_faces[i].size() == 0) {
                return {false, Embedding()};
            } else if(admissible_faces[i].size() == 1) {
                chosen_fragment = i;
            }
        }

        Graph fragment(G.n);
        for(int e : fragments[chosen_fragment]) {
            fragment.add_edge(G.edges[e].first, G.edges[e].second);
        }

        vector<bool> contact(G.n);
        for(int v : contact_vertices[chosen_fragment]) {
            contact[v] = true;
        }

        vector<int> alpha_path = get_alpha_path(fragment, contact);
        int face = admissible_faces[chosen_fragment][0];
        embedding.embed_path(alpha_path, face);
    }
    return {true, embedding};
}

void PlanarityTest::fragments_dfs(int e, int v, vector<bool> &visited, vector<vector<int>> &fragments) {
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

vector<vector<int>> PlanarityTest::get_fragments() {
    vector<vector<int>> fragments;
    vector<bool> visited(G.num_of_edges());

    for(int i = 0; i < G.num_of_edges(); i++) {
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

vector<vector<int>> PlanarityTest::get_contact_vertices(vector<vector<int>> &fragments) {
    vector<vector<int>> contact_vertices(fragments.size());
    vector<bool> seen(G.n, 0);

    for(int i = 0; i < fragments.size(); i++) {
        for(int e : fragments[i]) {
            for(int v : G.get_edge_ends(e)) {
                if(embedding.is_vertex_embedded(v)) {
                    seen[v] = true;
                }
            }
        }
        for(int e : fragments[i]) {
            for(int v : G.get_edge_ends(e)) {
                if(seen[v]) {
                    contact_vertices[i].push_back(v);
                }
                seen[v] = false;
            }
        }
    }
    return contact_vertices;
}

vector<vector<int>> PlanarityTest::get_admissible_faces(vector<vector<int>> &fragments,
                                                          vector<vector<int>> &contact_vertices) {
    vector<vector<int>> admissible_faces(fragments.size());

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

        for(int face : candidates) {
            bool contains_all = true;
            for(int v : contact_vertices[i]) {
                if(!embedding.belongs(v, face)) {
                    contains_all = false;
                    break;
                }
            }
            if(contains_all) admissible_faces[i].push_back(face);
        }
    }
    return admissible_faces;
}

bool PlanarityTest::alpha_path_dfs(int v, Graph &fragment, vector<bool> &visited, vector<bool> &contact,
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

vector<int> PlanarityTest::get_alpha_path(Graph &fragment, vector<bool> &contact) {
    vector<int> path;
    vector<bool> visited(G.n);

    for(int i = 0; i < contact.size(); i++) {
        if(contact[i]) {
            alpha_path_dfs(i, fragment, visited, contact, path);
            break;
        }
    }
    return path;
}
