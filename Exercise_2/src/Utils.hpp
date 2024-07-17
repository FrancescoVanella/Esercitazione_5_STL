#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary {

bool ImportMesh(const string& filePath, PolygonalMesh& mesh, double& scal);

bool ImportCell0Ds(const string& fileName, PolygonalMesh& mesh);

bool ImportCell1Ds(const string& fileName, PolygonalMesh& mesh);

bool ImportCell2Ds(const string& fileName, PolygonalMesh& mesh);

bool CheckLength(PolygonalMesh& mesh, double& scal);

bool CheckVertices2D(PolygonalMesh& mesh);

bool CheckArea(PolygonalMesh& mesh, double& scal);

