#pragma once

#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary {

bool ImportMesh(const string &filepath, PolygonalMesh& mesh);

bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh);

bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh);

bool ImportCell2Ds(const string &filename, PolygonalMesh& mesh);

bool RunTests(const PolygonalMesh& mesh);

bool ClosedPolygons(const PolygonalMesh& mesh);

bool CorrectEdges(const PolygonalMesh& mesh);

bool CorrectVertices(const PolygonalMesh& mesh);

bool TestMarkers(const PolygonalMesh& mesh);

bool TestEdgeLength(const PolygonalMesh& mesh);

bool TestPolygonArea(const PolygonalMesh& mesh);

double CalculatePolygonArea(const vector<Vector2d>& vertices);

}
