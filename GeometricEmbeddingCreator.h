
#ifndef PLANARITY_GEOMETRICEMBEDDINGCREATOR_H
#define PLANARITY_GEOMETRICEMBEDDINGCREATOR_H

#include <vector>
#include "Graph.h"
#include "PlanarityTester.h"

class GeometricEmbeddingCreator {
private:
    Embedding embedding;

    vector<int> get_canonical_order();

public:
    GeometricEmbeddingCreator(Embedding embedding);

    vector<pair<int,int>> get_coordinates();
};


#endif //PLANARITY_GEOMETRICEMBEDDINGCREATOR_H
