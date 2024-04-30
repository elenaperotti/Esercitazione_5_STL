#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolygonalLibrary {
bool ImportMesh(const string& filepath,
                PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",
                       mesh))
    {
        return false;
    }
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

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
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


    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        // Test 1: vedere se la mesh è corretta
        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            vector<unsigned int> edges = mesh.Cell2DEdges[c];
            unsigned int numberedges = edges.size();

            for(unsigned int e = 0; e < numberedges; e++)
            {
                const unsigned int origin = mesh.Cell1DVertices[edges[e]][0];
                const unsigned int end = mesh.Cell1DVertices[edges[e]][1];

                auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin);
                if(findOrigin == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), end);
                if(findEnd == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }

            }
        }
    }

    return true;

}





// importo i dati da Cell0Ds.csv
bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        replace(line.begin(),line.end(),';',' ');
        listLines.push_back(line); // aggiungo alla fine della lista ogni nuova riga presa dal file
    }

    file.close();

    listLines.pop_front(); // elimino la prima riga

    mesh.NumberCell0D = listLines.size(); //stampo la lunghezza della lista, che indica il numero di vettori

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }


    // qui comincio a dividere ogni riga in id, marker e coordinate (x,y)

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;


        converter >>  id >> marker >> coord(0) >> coord(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if( marker != 0)
        {
            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);

        }

    }
    file.close();
    return true;
}


// importo i dati da Cell1Ds.csv
bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        replace(line.begin(),line.end(),';',' ');
        listLines.push_back(line);
    }
    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }


    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices; // qua devo stampare 2 vertici -> coordinate origin e end

        converter >>  id >> marker >> vertices(0) >> vertices(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if( marker != 0)
        {
            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}


// importo i dati da Cell2Ds.csv
bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        replace(line.begin(),line.end(),';',' ');
        listLines.push_back(line);
    }

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int numvertices;
        unsigned int numedges;

        converter >>  id >> marker >> numvertices;

        vector<unsigned int> vertices(numvertices); // lista di vertici
        for(unsigned int i = 0; i < numvertices; i++)
            converter >> vertices[i];

        converter >> numedges;
        vector<unsigned int> edges(numedges); // lista di lati
        for(unsigned int i = 0; i < numedges; i++)
            converter >> edges[i];

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);

        if( marker != 0)
        {
            if (mesh.Cell2DMarkers.find(marker) == mesh.Cell2DMarkers.end())
                mesh.Cell2DMarkers.insert({marker, {id}});

            else
                mesh.Cell2DMarkers[marker].push_back(id);
        }
    }

    file.close();
    return true;
}

void LunghezzaLati(PolygonalMesh& mesh, double tol, int lati,int check) {
    for (unsigned int c = 0; c < mesh.NumberCell2D; c++)
    {
        // stampo i vertici
        vector<unsigned int> vertices = mesh.Cell2DVertices[c];
        unsigned int numvertices = vertices.size();


        if (numvertices == lati){
            if (lati ==3)
            {
                cout << "\nIl triangolo " << c << " ha i seguenti lati: "<<endl;
            }
            else if (lati ==4)
            {
                cout << "\nIl quadrato " << c << " ha i seguenti lati: "<<endl;
            }
            else if (lati ==5)
            {
                cout << "\nIl pentagono " << c << " ha i seguenti lati: "<<endl;
            }
            else if (lati ==6)
            {
                cout << "\nL'esagono " << c << " ha i seguenti lati: "<<endl;
            }
            else if (lati ==7)
            {
                cout << "\nL'ettagono " << c << " ha i seguenti lati: "<<endl;
            }

            for (unsigned int i = 0; i < numvertices-1; i++)
            {
                unsigned int vertice1 = vertices[i];
                unsigned int vertice2 = vertices[i+1];

                // stampo la coordinata x del primo vertice
                double x1 = mesh.Cell0DCoordinates[vertice1][0];
                double y1 = mesh.Cell0DCoordinates[vertice1][1];
                // stampo la coordinata y del secondo vertice
                double x2 = mesh.Cell0DCoordinates[vertice2][0];
                double y2 = mesh.Cell0DCoordinates[vertice2][1];

                // formula della distanza
                double distanza = sqrt(pow((x1-x2),2)+ pow((y1-y2),2));
                if (distanza > tol){
                    cout <<"Il lato di vertici "<< vertice1 <<" e " <<vertice2<< " e' lungo " << distanza << endl;
                }
                else
                    check = 1;

            }
            // per completare il calcolo della lunghezza dei alti del poligono devo legare il vertice finale ed iniziale
            unsigned int verticein = vertices[0];
            double xin = mesh.Cell0DCoordinates[verticein][0];
            double yin = mesh.Cell0DCoordinates[verticein][1];

            unsigned int verticefin = vertices[numvertices-1];
            double xfin = mesh.Cell0DCoordinates[verticefin][0];
            double yfin = mesh.Cell0DCoordinates[verticefin][1];
            double distanza = sqrt(pow((xin-xfin),2)+ pow((yin-yfin),2));
            if (distanza > tol){

                cout <<"Il lato di vertici "<< verticein <<" e " <<verticefin<< " e' lungo " << distanza << endl;
            }
            else
                check = 1;
        }

    }

    if (check == 0)
        cout << "I lati hanno tutti lunghezza maggiore della tolleranza." << endl;
    else
        cout << "I lati non hanno tutti lunghezza maggiore della tolleranza." << endl;
    return;
}

void DimensioneArea(PolygonalMesh& mesh, double tol, int lati, int check) {
    // formula tolleranza vista a lezione
    tol = max(pow(tol,2)*sqrt(3)/4, tol);

    for (unsigned int c = 0; c < mesh.NumberCell2D; c++)
    {
        // stampo i vertici
        vector<unsigned int> vertices = mesh.Cell2DVertices[c];
        unsigned int numvertices = vertices.size();

        if (numvertices == lati){ //in realtà qui inserendo un altro numero di vertici posso sapere le aree di qualsiasi poligono

            double area = 0;

// per il triangolo bastava in realta stampare i 3 vertici con le loro coordinate
// e scrivere la formula dell'area: il seguente metodo è generico per un poligono qualunque:
            for (unsigned int i = 0; i < numvertices; i++)
            {
                unsigned int vertice1 = vertices[i];
                unsigned int vertice2 = vertices[i+1];

                // stampo la coordinata x del primo vertice
                double x1 = mesh.Cell0DCoordinates[vertice1][0];
                double y1 = mesh.Cell0DCoordinates[vertice1][1];
                // stampo la coordinata y del secondo vertice
                double x2 = mesh.Cell0DCoordinates[vertice2][0];
                double y2 = mesh.Cell0DCoordinates[vertice2][1];

                // formula dell'area di Gauss
                area += abs(x1*y2 - x2*y1);
            }
            // per completare il calcolo dell'area devo legare il vertice finale ed iniziale
            unsigned int verticein = vertices[0];
            double xin = mesh.Cell0DCoordinates[verticein][0];
            double yin = mesh.Cell0DCoordinates[verticein][1];

            unsigned int verticefin = vertices[numvertices-1];
            double xfin = mesh.Cell0DCoordinates[verticefin][0];
            double yfin = mesh.Cell0DCoordinates[verticefin][1];

            area += abs(xfin*yin -xin*yfin);
            area = area / 2;

            if (area > tol) {
                if (lati ==3)
                {
                    cout << "\nIl triangolo " << c << " di vertici: "<<endl;
                }
                else if (lati ==4)
                {
                    cout << "\nIl quadrato " << c << " di vertici: "<<endl;
                }
                else if (lati ==5)
                {
                    cout << "\nIl pentagono " << c << " di vertici: "<<endl;
                }
                else if (lati ==6)
                {
                    cout << "\nL'esagono " << c << " di vertici: "<<endl;
                }
                else if (lati ==7)
                {
                    cout << "\nL'ettagono " << c << " di vertici: "<<endl;
                }
                for (unsigned int i:vertices)
                    cout <<i << " ";
                cout <<"ha area: " <<area<< endl;
            }
            else
                check = 1;
        }

    }

    if (check == 0)
        cout << "Tutte le aree hanno dimensione non trascurabile." << endl;
    else
        cout << "Non tutte le aree hanno dimensione non trascurabile." << endl;
    return;

}

}
