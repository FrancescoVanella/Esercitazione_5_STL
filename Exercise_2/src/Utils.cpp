#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary {

// queste funzioni, che testatno la mesh, vengono descritte sommariamente nel corpo della funzione RunTests

bool ClosedPolygons(const PolygonalMesh& mesh)
{
    int n = 0;

    for (const vector<unsigned int>& edges : mesh.Cell2DEdges)
    {
        set<unsigned int> uniqueVertices = {};

        for (unsigned int edge : edges)
        {
            unsigned int origin = mesh.Cell1DVertices[edge][0];
            unsigned int end = mesh.Cell1DVertices[edge][1];

            uniqueVertices.insert(origin);
            uniqueVertices.insert(end);
        }

        if (uniqueVertices.size() != edges.size())
        {
            cerr << "Error: 2D cell (Id:" << n <<") isn't a closed polygon."<< endl;

            return false;
        }

        n=n+1;
    }

    return true;
}

bool CorrectEdges(const PolygonalMesh& mesh)
{
    int n = 0;

    const vector<unsigned int>& lati = mesh.Cell1DId;

    for (const vector<unsigned int>& edges : mesh.Cell2DEdges)
    {
        for (const unsigned int& edge : edges)
        {
            auto it = find(lati.begin(), lati.end(), edge);

            if (it == lati.end())
            {
                cerr << "Error: 2D cell (Id:" << n <<") has an invalid edge."<< endl;
                return false;
            }
        }

        n = n+1;
    }

    return true;
}

bool CorrectVertices(const PolygonalMesh& mesh)
{

    // Celle 1D

    const vector<unsigned int>& Id0D = mesh.Cell0DId;

    int n = 0;

    for (const vector<unsigned int>& estremi : mesh.Cell1DVertices)
    {
        unsigned int origin = estremi[0];
        unsigned int end = estremi[1];

        auto it1 = find(Id0D.begin(), Id0D.end(), origin);
        auto it2 = find(Id0D.begin(), Id0D.end(), end);

        if (it1 == Id0D.end() || it2 == Id0D.end())
        {
            cerr << "Error: origin or end of 1D cell (Id:" << n <<") is wrong."<< endl;
            return false;
        }

        n = n+1;
    }

    // Celle 2D

    n = 0;

    for (const vector<unsigned int>& estremi : mesh.Cell2DVertices)
    {
        for (const unsigned int& vertice : estremi)
        {
            auto it = find(Id0D.begin(), Id0D.end(), vertice);

            if (it == Id0D.end())
            {
                cerr << "Error: vertices of 2D cell (Id: " << n << ") are wrong."<< endl;
                return false;
            }
        }

        n = n+1;
    }
    return true;
}

bool TestMarkers(const PolygonalMesh& mesh)
{
    cout << "Test marker delle celle 0D" << endl;

    for (const auto& pair : mesh.Cell0DMarkers)
    {
        unsigned int key = pair.first;
        const list<unsigned int>& values = pair.second;

        cout << endl;
        cout << "Marker: " << key << std::endl;

        cout << "Id: ";
        for (unsigned int value : values) {
            cout << value << " ";
        }
        cout << endl;
    }

    cout << endl;
    cout << "Test marker delle celle 1D" << endl;

    for (const auto& pair : mesh.Cell1DMarkers)
    {
        unsigned int key = pair.first;
        const list<unsigned int>& values = pair.second;

        cout << endl;
        cout << "Marker: " << key << std::endl;

        // Stampa i valori nella lista
        cout << "Id: ";
        for (unsigned int value : values) {
            cout << value << " ";
        }
        cout << endl;
    }

    cout << endl;

    return true;
}

// dopo un'analisi si è verificato empiricamente che i marker delle celle 0D e 1D sono stati salvati correttamente


bool TestEdgeLength(const PolygonalMesh& mesh)
{

    // Scorre tutti i vertici delle celle 1D

    int n = 0;

    for(const vector<unsigned int>& edge : mesh.Cell1DVertices)
    {
        // Ottiene i vertici delle celle
        const Vector2d& vertex1 = mesh.Cell0DCoordinates[edge[0]];
        const Vector2d& vertex2 = mesh.Cell0DCoordinates[edge[1]];
        // Calcola la lunghezza della cella
        double edgeLength = (vertex2 - vertex1).norm();

        // Controlla che la lunghezza non sia nulla
        if(edgeLength == 0)
        {
            cerr << "Error: Edge length of 1D cell (Id:" << n << ") is zero." << endl;
            return false;
        }
        n = n+1;
    }

    cout << "No edge has zero length."<< endl;
    cout << endl;

    return true;
}

bool TestPolygonArea(const PolygonalMesh& mesh)
{
    int n = 0;

    // Scorre le celle 2D
    for(unsigned int i = 0; i < mesh.NumberCell2D; ++i)
    {
        // Ottiene i vertici delle celle
        const vector<unsigned int> verticesIndices = mesh.Cell2DVertices[i];
        vector<Vector2d> polygonVertices;
        for(unsigned int vertexIndex : verticesIndices)
        {
            polygonVertices.push_back(mesh.Cell0DCoordinates[vertexIndex]);
        }

        // Calcola l'area della cella
        double polygonArea = CalculatePolygonArea(polygonVertices);

        // Controlla che l'area non sia nulla
        if(polygonArea == 0)
        {
            cerr << "Error: Polygon area of 2D cell (Id:" << n << ")is zero." << endl;
            return false;
        }

        n = n+1;
    }

    cout << "No polygon has zero area."<< endl;
    cout << endl;

    return true;
}

double CalculatePolygonArea(const vector<Vector2d>& vertices)
{
    // Usa la formula di Gauss per calcolare l'area di un poligono
    double area = 0.0;
    int n = vertices.size();
    for(int i = 0; i < n-1; ++i)
    {
        int j = (i + 1);
        area += vertices[i][0] * vertices[j][1];
        area -= vertices[j][0] * vertices[i][1];
    }

    area += vertices[n-1][0] * vertices[0][1];
    area -= vertices[0][0] * vertices[n-1][1];

    area = abs(area) / 2.0;

    return area;
}

// Queste funzioni importano i dati delle celle e la mesh

bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh)
{
    ifstream file(filename);
    if(!file.is_open())
    {
        cerr << "Error: Unable to open file " << filename << endl;
        return false;
    }

    // Ignora l'intestazione
    string line;
    getline(file, line);

    unsigned int id, marker;
    char ch;
    double x, y;
    while(getline(file, line))
    {
        stringstream ss(line);
        ss >> id >> ch >> marker >> ch >> x >> ch >> y;
        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(Vector2d(x, y));
        if(marker != 0)
        {
            mesh.Cell0DMarkers[marker].push_back(id);
        }
    }

    file.close();

    mesh.NumberCell0D = mesh.Cell0DId.size();
    return true;
}

bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh)
{
    ifstream file(filename);
    if(!file.is_open())
    {
        cerr << "Error: Unable to open file " << filename << endl;
        return false;
    }

    // Ignora l'intestazione
    string line;
    getline(file, line);

    unsigned int id, marker, origin, end;
    char ch;

    while(getline(file, line))
    {
        stringstream ss(line);
        ss >> id >> ch >> marker >> ch >> origin >> ch >> end;

        mesh.Cell1DId.push_back(id);
        vector<unsigned int> vertices;
        vertices.push_back(origin);
        vertices.push_back(end);
        mesh.Cell1DVertices.push_back(vertices);

        if(marker != 0)
        {
            mesh.Cell1DMarkers[marker].push_back(id);
        }
    }

    file.close();

    mesh.NumberCell1D = mesh.Cell1DId.size();
    return true;
}

bool ImportCell2Ds(const string &filename, PolygonalMesh& mesh)
{
    ifstream file(filename);
    if(!file.is_open())
    {
        cerr << "Error: Unable to open file " << filename << endl;
        return false;
    }

    // Ignora l'intestazione
    string line;
    getline(file, line);

    unsigned int id, marker, numVertices, verticesID, numEdges, edgesID;
    char ch;

    while(getline(file, line))
    {
        stringstream ss(line);
        ss >> id >> ch >> marker >> ch >> numVertices;

        vector<unsigned int> vertices;
        for(unsigned int i = 0; i < numVertices; ++i)
        {
            ss >> ch >> verticesID;
            vertices.push_back(verticesID);
        }

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);

        ss >> ch >> numEdges;

        vector<unsigned int> edges;
        for(unsigned int i = 0; i < numEdges; ++i)
        {
            ss >> ch >> edgesID;
            edges.push_back(edgesID);
        }

        mesh.Cell2DEdges.push_back(edges);

        if(marker != 0)
        {
            mesh.Cell2DMarkers[marker].push_back(id);
        }
    }

    file.close();

    mesh.NumberCell2D = mesh.Cell2DId.size();
    return true;
}

bool ImportMesh(const string& filepath, PolygonalMesh& mesh)
{
    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv", mesh))
    {
        cerr << "Error importing Cell0Ds." << endl;
        return false;
    }

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv", mesh))
    {
        cerr << "Error importing Cell1Ds." << endl;
        return false;
    }

    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv", mesh))
    {
        cerr << "Error importing Cell2Ds." << endl;
        return false;
    }

    if(!RunTests(mesh))
    {
        cerr << "Error: Tests failed." << endl;
        return false;
    }

    return true;
}

// Esegue i test sulla mesh

bool RunTests(const PolygonalMesh& mesh)
{
    bool allTestsPassed = true;

    // Controlla che le celle 2D siano dei poligoni chiusi
    if(!ClosedPolygons(mesh))
    {
        allTestsPassed = false;
    }

    // Controlla che i lati delle celle 2D corrispondano a celle 1D esistenti
    if(!CorrectEdges(mesh))
    {
        allTestsPassed = false;
    }

    // Valuta se gli estremi delle celle 1D e i vertici delle celle 2D corrispondono alle celle 0D
    if(!CorrectVertices(mesh))
    {
        allTestsPassed = false;
    }

    // Stampa a schermo i marked delle celle 0D e 1D in modo da verificare empiricamente se sono stati registrati in modo corretto
    if(!TestMarkers(mesh))
    {
        allTestsPassed = false;
    }

    // Controlla che le celle 1D non abbiano lunghezza nulla
    if(!TestEdgeLength(mesh))
    {
        allTestsPassed = false;
    }

    // Controlla che le celle 2D non abbiano area nulla
    if(!TestPolygonArea(mesh))
    {
        allTestsPassed = false;
    }

    return allTestsPassed;
}

}
