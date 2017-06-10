
#ifndef PLANARITY_GEOMETRICEMBEDDINGCREATOR_H
#define PLANARITY_GEOMETRICEMBEDDINGCREATOR_H

#include <vector>
#include "Graph.h"
#include "PlanarityTester.h"

class GeometricEmbeddingCreator {
public:
    Embedding embedding;

    GeometricEmbeddingCreator(Embedding embedding);

    vector<pair<int,int>> get_coordinates();

private:
    vector<int> get_canonical_order();
};


#endif //PLANARITY_GEOMETRICEMBEDDINGCREATOR_H
