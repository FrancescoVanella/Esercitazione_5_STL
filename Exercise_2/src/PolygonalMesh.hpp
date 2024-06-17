#pragma once

#include <map>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary {

struct PolygonalMesh
{
    unsigned int NumberCell0D = 0;                                // Numero di celle 0D
    vector<unsigned int> Cell0DId = {};                           // ID delle celle 0D
    vector<Vector2d> Cell0DCoordinates = {};                      // Coordinate delle celle 0D
    map<unsigned int, list<unsigned int>> Cell0DMarkers = {};     // Marcatori delle celle 0D

    unsigned int NumberCell1D = 0;                                // Numero di celle 1D
    vector<unsigned int> Cell1DId = {};                           // ID delle celle 1D
    vector<vector<unsigned int>> Cell1DVertices = {};             // Due indici per rappresentare l'origine e la fine dello spigolo
    map<unsigned int, list<unsigned int>> Cell1DMarkers = {};     // Marcatori delle celle 1D

    unsigned int NumberCell2D = 0;                                // Numero di celle 2D
    vector<unsigned int> Cell2DId = {};                           // ID delle celle 2D
    vector<vector<unsigned int>> Cell2DVertices = {};             // Indici dei vertici delle celle 2D
    vector<vector<unsigned int>> Cell2DEdges = {};                // Indici degli spigoli delle celle 2D
    map<unsigned int, list<unsigned int>> Cell2DMarkers = {};     // Marcatori delle celle 2D
};

}

