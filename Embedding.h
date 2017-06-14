
#ifndef PLANARITY_EMBEDDING_H
#define PLANARITY_EMBEDDING_H

#include <vector>
#include "Graph.h"

class Embedding {
public:
    Graph G;
    vector<vector<int>> faces;
    vector<vector<bool>> embedded;
    vector<vector<int>> vertex_faces;
    vector<vector<bool>> vertex_on_face;

    Embedding();

    Embedding(Graph G);

    void embed_initial_cycle(vector<int> &cycle);

    void embed_path(vector<int> path, int face);

    bool is_vertex_embedded(int v);

    bool is_edge_embedded(int e);

    void triangulate();

    bool belongs(int v, int face);

private:
    vector<int> get_face_fragment(int face, int a, int b);
};


#endif //PLANARITY_EMBEDDING_H
