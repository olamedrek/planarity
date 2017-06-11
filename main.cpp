
#include <iostream>
#include "Graph.h"
#include "GeometricEmbeddingCreator.h"
#include "check.h"

using namespace std;

// TODO more tests, documentation

bool test_non_biconnected(vector<Graph> &components) {
    bool result;
    Embedding embedding;
    for(Graph &comp : components) {
        PlanarityTester tester(comp);
        tie(result, embedding) = tester.test();
        if(!result) return false;
    }
    return true;
}

pair<bool,vector<pair<int,int>>> get_coordinates(Graph &G) {
    PlanarityTester tester(G);
    bool result;
    Embedding embedding;
    tie(result, embedding) = tester.test();
    if(result) {
        GeometricEmbeddingCreator creator(embedding);
        return {result, creator.get_coordinates()};
    } else {
        return {result, vector<pair<int,int>>()};
    }
}

int main(int argc, char **argv) {
    char* input_path = argv[1];
    char* output_path = argv[2];

    ifstream in(input_path);
    cin.rdbuf(in.rdbuf());
    ofstream out(output_path);
    cout.rdbuf(out.rdbuf());

    int n, m;
    cin >> n >> m;

    Graph G(n);

    for(int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--; b--;
        G.add_edge(a, b);
    }

    vector<Graph> components = G.get_biconnected_components();

    if(components.size() == 1) {
        cout << 1 << endl;

        bool planar;
        vector<pair<int,int>> coordinates;
        tie(planar, coordinates) = get_coordinates(G);

        cout << planar << endl;

        if(planar) {
            cout << check_drawing(G, coordinates) << endl;

            for(int i = 0; i < n; i++) {
                cout << coordinates[i].first << " " << coordinates[i].second << endl;
            }
        }
    } else {
        cout << 0 << endl;
        bool planar = test_non_biconnected(components);
        cout << planar << endl;
    }

    return 0;
}
