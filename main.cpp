
#include <iostream>
#include "Graph.h"
#include "GeometricEmbeddingCreator.h"
#include "check.h"

using namespace std;

tuple<bool,vector<pair<int,int>>> get_coordinates(const Graph &G) {
    PlanarityTester tester(G);
    bool result;
    Embedding embedding;
    tie(result, embedding) = tester.test();
    if(result) {
        GeometricEmbeddingCreator creator(embedding);
        return make_tuple(result, creator.get_coordinates());
    } else {
        return make_tuple(result, vector<pair<int,int>>());
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

   /* if(!G.two_connected()) {
        cout << "UNKNOWN\n";
        return 0;
    }*/

    bool result;
    vector<pair<int,int>> coordinates;

    tie(result, coordinates) = get_coordinates(G);

    if(!result) {
        cout << result << endl;
    } else {
        if(!check_drawing(G, coordinates)) {
            cout << "ERROR\n";
        } else {
            cout << result << endl;
            for(int i = 0; i < n; i++) {
                cout << coordinates[i].first << " " << coordinates[i].second << endl;
            }
        }
    }

    return 0;
}
