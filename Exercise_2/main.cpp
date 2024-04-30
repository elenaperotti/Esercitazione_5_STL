#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main()

{
    double tol = 1.0e-16;
    int lati = 3; //inserire un numero di lati dei poligoni per controllare area e lunghezza lati
    int check = 0;

    PolygonalMesh mesh;

    string filepath = "./PolygonalMesh";
    if(!ImportMesh(filepath,
                    mesh))
    {
        return 1;
    }


    LunghezzaLati(mesh,tol,lati,check);
    DimensioneArea(mesh,tol,lati,check);


    return 0;

}
