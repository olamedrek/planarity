
#ifndef PLANARITY_PLANARITYTEST_H
#define PLANARITY_PLANARITYTEST_H

#include <bits/stdc++.h>
#include "Graph.h"
#include "Embedding.h"

class PlanarityTest {
private:
    const Graph &G;

    Embedding embedding;

    void fragments_dfs(int e, int v, vector<bool> &visited, vector<vector<int>> &fragments);

    vector<vector<int>> get_fragments();

    vector<vector<int>> get_contact_vertices(vector<vector<int>> &fragments);

    vector<vector<int>> get_admissible_faces(vector<vector<int>> &fragments, vector<vector<int>> &contact_vertices);

    bool alpha_path_dfs(int v, Graph &fragment, vector<bool> &visited, vector<bool> &contact, vector<int> &path);

    vector<int> get_alpha_path(Graph &fragment, vector<bool> &contact);

public:
    PlanarityTest(const Graph &G);

    pair<bool,Embedding> test();
};


#endif //PLANARITY_PLANARITYTEST_H
