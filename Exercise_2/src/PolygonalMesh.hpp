#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary {

struct PolygonalMesh
{
    // Cell0D
    unsigned int NumberCell0D = 0;
    vector<unsigned int> Cell0DId = {};
    map<unsigned int, list<unsigned int>> Cell0DMarkers = {};
    vector<Vector2d> Cell0DCoordinates = {};

    // Cell1D
    unsigned int NumberCell1D = 0;
    vector<unsigned int> Cell1DId = {};
    map<unsigned int, list<unsigned int>> Cell1DMarkers = {};
    vector<Vector2i> Cell1DVertices = {};

    // Cell2D
    unsigned int NumberCell2D = 0;
    vector<unsigned int> Cell2DId = {};
    vector<VectorXi> Cell2DVertices = {};
    vector<VectorXi> Cell2DEdges = {};

};

}
