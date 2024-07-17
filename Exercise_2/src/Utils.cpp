#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace PolygonalLibrary{

bool ImportMesh(const string& filePath, PolygonalMesh& mesh, double& scal)
{
    // Importo i valori di Cell0D
    if(!ImportCell0Ds(filePath + "/Cell0Ds.csv", mesh))
        return false;
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;
            cout << endl;
        }
    }
    cout << endl;


    // Importo i valori di Cell1D
    if(!ImportCell1Ds(filePath + "/Cell1Ds.csv", mesh))
        return false;
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;
            cout << endl;
        }
    }
    cout << endl;


    // Importo i valori di Cell2D
    if(!ImportCell2Ds(filePath + "/Cell2Ds.csv", mesh))
        return false;
    else
        cout << "Cell2D imported" << endl;
    cout << endl;


    // Controllo la lunghezza dei lati
    if(!CheckLength(mesh, scal))
        return false;
    cout << "Valid edges" << endl;
    cout << endl;


    // Controllo che l'area non sia nulla
    if(!CheckArea(mesh, scal))
        return false;
    cout << "Area of polygons is non-zero" << endl;
    cout << endl;


    // Controllo la corrispondenza tra i lati 1D e 2D
    if(!CheckVertices2D(mesh))
        return false;
    cout << "The match between 1D edges and 2D edges is correct" << endl;
    cout << endl;


    return true;
}



// Questa funzione importa e memorizza i dati relativi a Cell0D
bool ImportCell0Ds(const string& fileName, PolygonalMesh& mesh)
{
    // Apro il file
    ifstream file;
    file.open(fileName);

    // Controllo che venga aperto correttamente
    if(file.fail())
        return false;

    // Leggo il file sostituendo i ";" con " " e salvo i dati in una lista di stringhe
    list<string> listLines;
    string line;
    string limit = ";";
    while(getline(file, line))
    {
        size_t found = line.find(limit); // Found è la posizione di ";"
        while(found != string::npos)
        {
            line.replace(found, 1, " "); // Sostituisce ";" con " "
            found = line.find(limit, found); // Cerco il prossimo ";"
        }
        listLines.push_back(line);
    }

    // Rimuovo la prima riga
    listLines.pop_front();

    mesh.NumberCell0D = listLines.size();

    // Verifico che il file non fosse vuoto
    if(mesh.NumberCell0D == 0)
    {
        cerr << "Cell 0D doesn't exist" << endl;
        return false;
    }

    // Inizializzo la memoria
    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    // Leggo ogni stringa e salvo i dati
    for(const string& line : listLines)
    {
        istringstream converter(line);
        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        converter >> id >> marker >> coord(0) >> coord(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        // Se il marker non è zero, memorizzo marker e id
        if(marker != 0)
        {
            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});

            // Se il marker è già salvato, aggiungo solo l'id
            if(!ret.second)
                (ret.first) -> second.push_back(id);
        }
    }
    // Chiudo il file
    file.close();
    return true;
}


// Questa funzione importa e memorizza i dati relativi a Cell1D
bool ImportCell1Ds(const string& fileName, PolygonalMesh& mesh)
{
    // Apro il file
    ifstream file;
    file.open(fileName);

    // Verifico che si apra correttamente
    if(file.fail())
        return false;

    // Leggo il file sostituendo ";" con " " e salvo i dati su una lista di stringhe
    list<string> listLines;
    string line;
    string limit = ";";
    while(getline(file, line))
    {
        size_t found = line.find(limit); // Trovo la posizione del ";"
        while(found != string::npos)
        {
            line.replace(found, 1, " "); // Sostituisco ";" con " "
            found = line.find(limit, found); //Vado al prossimo ";"
        }
        listLines.push_back(line);
    }
    // Rimuovo la prima riga
    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    // Verifico che il file non fosse vuoto
    if(mesh.NumberCell1D == 0)
    {
        cerr << "Cell 1D doesn't exist" << endl;
        return false;
    }

    // Inizializzo la memoria
    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    // Leggo ogni stringa e salvo i dati
    for(const string& line : listLines)
    {
        istringstream converter(line);
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >> id >> marker >> vertices(0) >> vertices(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        // Se il marker non è zero memorizzo marker e id
        if(marker != 0)
        {
            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            // Se il marker è già salvato memorizzo solo l'id
            if(!ret.second)
                (ret.first) -> second.push_back(id);
        }
    }
    // Chiudo il file
    file.close();
    return true;
}


// Questa funzione importa e memorizza i dati relativi a Cell2D
bool ImportCell2Ds(const string& fileName, PolygonalMesh& mesh)
{
    // Apro il file
    ifstream file;
    file.open(fileName);

    // Verifico che si apra correttamente
    if(file.fail())
        return false;

    // Leggo il file, sostituisco ";" con " " e salvo il contenuto su una lista di stringhe
    list<string> listLines;
    string line;
    string limit = ";";
    while(getline(file, line))
    {
        size_t found = line.find(limit); // found rappresenta la posizione del ";"
        while(found != string::npos) //Cerco fino a che non arrivo alla fine della stringa, lo uso per vedere se ho trovato il ";"
        {
            line.replace(found, 1, " "); // Sostituisco il ";" con " "
            found = line.find(limit, found); // Cerco il prossimo ";"
        }
        listLines.push_back(line);
    }
    // Elimino la prima riga
    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    // Verifico che il file non sia vuoto
    if(mesh.NumberCell2D == 0)
    {
        cerr << "Cell 2D doesn't exist" << endl;
        return false;
    }

    // Inizializzo la memoria
    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    // Leggo le stringhe e memorizzo i dati
    for(const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int n; //Qui salvo il numero di vertici, essendo in 2D coincide con il numero di lati

        converter >> id >> marker >> n;

        VectorXi vertices(n);
        for(unsigned int i = 0; i < n; i++)
            converter >> vertices(i);

        converter >> n; //So che è uguale alla n dei vertici, mi serve solo per scorrere

        VectorXi edges(n);
        for(unsigned int i = 0; i < n; i++)
            converter >> edges(i);

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);

    }
    // Chiudo il file
    file.close();
    return true;
}


// Funzione che controlla la lunghezza dei lati
bool CheckLength(PolygonalMesh& mesh, double& scal)
{
    // Definisco la precisione
    double eps = numeric_limits<double>::epsilon()*scal;

    // Scorro il vettore dei vertici per ogni lato
    for(const auto& val : mesh.Cell1DVertices)
    {
        // Assegno le due coordinate dei singoli vertici a due vettori coord1 e coord2
        Vector2d coord1 = mesh.Cell0DCoordinates[val(0)];
        Vector2d coord2 = mesh.Cell0DCoordinates[val(1)];
        if(abs(coord1(0) - coord2(0)) < eps && abs(coord1(1) - coord2(1)) < eps)
        {
            cerr << "Edge has null length" << endl;
            return false;
        }
    }
    return true;
}


// Verifico che non ci siano poligoni con area nulla
bool CheckArea(PolygonalMesh& mesh, double& scal)
{
    // Definisco la precisione
    double eps = numeric_limits<double>::epsilon()*scal;

    for(const auto& edges : mesh.Cell2DEdges)
    {
        // Definisco un contatore che mi indica il numero di lati paralleli (cioè con prodotto vettoriale nullo)
        unsigned int n = 0;

        for(unsigned int i = 0; i < edges.size(); i++)
        {
            // Calcolo il prodotto vettoriale dei primi n-1 lati
            if(i != edges.size() - 1)
            {
                Vector2i edge1 = mesh.Cell1DVertices[edges(i)];
                Vector2i edge2 = mesh.Cell1DVertices[edges(i+1)];
                Vector2d coordA1 = mesh.Cell0DCoordinates[edge1(0)];
                Vector2d coordA2 = mesh.Cell0DCoordinates[edge2(0)];
                Vector2d coordB1 = mesh.Cell0DCoordinates[edge1(1)];
                Vector2d coordB2 = mesh.Cell0DCoordinates[edge2(1)];
                Vector2d length1 = coordB1 - coordA1;
                Vector2d length2 = coordB2 - coordA2;
                double vectorial = length1.x() * length2.y() - length1.y() * length2.x();

                // Se il prodotto vettoriale è minore della precisione i vettori sono paralleli, aumento il contatore
                if(abs(vectorial) < eps)
                    n++;
            }
            // Ora calcolo il prodotto vettoriale tra l'ultimo e il primo lato
            else
            {
                Vector2i edge1 = mesh.Cell1DVertices[edges(i)];
                Vector2i edge2 = mesh.Cell1DVertices[edges(0)];
                Vector2d coordA1 = mesh.Cell0DCoordinates[edge1(0)];
                Vector2d coordA2 = mesh.Cell0DCoordinates[edge2(0)];
                Vector2d coordB1 = mesh.Cell0DCoordinates[edge1(1)];
                Vector2d coordB2 = mesh.Cell0DCoordinates[edge2(1)];
                Vector2d length1 = coordB1 - coordA1;
                Vector2d length2 = coordA2 - coordB2;
                double vectorial = length1.x() * length2.y() - length1.y() * length2.x();

                // Se il prodotto vettoriale è minore della precisione i vettori sono paralleli, aumento il contatore
                if(abs(vectorial) < eps)
                    n++;
            }
        }
        // Se tutti i lati sono paralleli, l'area è nulla
        if(n == edges.size())
        {
            cerr << "The area of some polygons is zero" << endl;
            return false;
        }
    }
    return true;
}


// Verifico la corrispondenza dei lati 1D e 2D
bool CheckVertices2D(PolygonalMesh& mesh)
{
    for(const auto& vertices : mesh.Cell2DVertices)
    {
        bool found = false;

        for(unsigned int i = 0; i < vertices.size(); i++)
        {
            // Confronto le coppie di vertici con le coppie di lati
            if(i != vertices.size() - 1)
            {
                Vector2i search(vertices(i), vertices(i+1));
                for(const auto& couple : mesh.Cell1DVertices)
                {
                    // Considero anche le coppie al contrario
                    Vector2i inverted(couple(1), couple(0));
                    if(search == couple || search == inverted)
                    {
                        found = true;
                        break;
                    }
                }
                // Se non è vero found, i lati 2D non corrispondono ai lati 1D
                if(!found)
                {
                    cerr << "2D edges doesn't match 1D edges" << endl;
                    return false;
                }
            }

            // Confronto primo e ultimo vertice con le coppie di lati
            else
            {
                Vector2i search(vertices(i), vertices(0));
                for(const auto& couple : mesh.Cell1DVertices)
                {
                    Vector2i inverted(couple(1), couple(0));
                    if(search == couple || search == inverted)
                    {
                        found = true;
                        break;
                    }
                }
                // Se non è vero found i lati 2D non corrispondono ai lati 1D
                if(!found)
                {
                    cerr << "2D edges doesn't match 1D edges" << endl;
                    return false;
                }
            }
        }
    }
    return true;
}

}
