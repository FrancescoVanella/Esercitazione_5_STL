#include <iostream>
#include "Utils.hpp"
#include "PolygonalMesh.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;


int main()
{
    PolygonalMesh mesh;


    double eps_scal =  3;

    string filePath = "PolygonalMesh";
    if(!ImportMesh(filePath, mesh, eps_scal))
        return 1;



    return 0;
}
