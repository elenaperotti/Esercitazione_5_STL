#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary {

// funzione booleana che mi serve per verificare se la struttura della PolygonalMesh è corretta
bool ImportMesh(const string &filepath,
                PolygonalMesh& mesh);

// funzioni booleane che mi servono per importare i dati dei 3 file .csv e verificarne il successo della lettura
bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh);

bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh);

bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh);

bool TestEdgesTriangles(const string &filepath,
                        PolygonalMesh& mesh);

void DimensioneArea(PolygonalMesh& mesh, double tol,int lati,int check);

void LunghezzaLati(PolygonalMesh& mesh, double tol,int lati,int check);


}
