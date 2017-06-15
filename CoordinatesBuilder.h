
#ifndef PLANARITY_COORDINATESBUILDER_H
#define PLANARITY_COORDINATESBUILDER_H

#include <vector>
#include "Graph.h"
#include "PlanarityTest.h"

class CoordinatesBuilder {
private:
    Embedding embedding;

    vector<int> get_canonical_order();

public:
    CoordinatesBuilder(Embedding embedding);

    vector<pair<int,int>> get_coordinates();
};


#endif //PLANARITY_COORDINATESBUILDER_H
