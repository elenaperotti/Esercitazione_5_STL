#pragma once

#include <iostream>
#include "Eigen/Eigen"
#include "map"

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary
{
    // struttura mesh poligonale
    struct PolygonalMesh
    {
        unsigned int NumberCell0D = 0; // rete 1-dimensionale -> VERTICI
        vector<unsigned int> Cell0DId = {}; // identificatore
        vector<Vector2d> Cell0DCoordinates = {}; // coordinate
        map<unsigned int, list<unsigned int>> Cell0DMarkers = {}; // marker

        unsigned int NumberCell1D = 0; // rete 2-dimensionale -> LATI
        vector<unsigned int> Cell1DId = {}; // identificatore
        vector<Vector2i> Cell1DVertices = {}; // descritto da due vertici: origin e end
        map<unsigned int, list<unsigned int>> Cell1DMarkers = {}; // marker

        unsigned int NumberCell2D = 0; // rete 3-dimensionale -> CELLE
        vector<unsigned int> Cell2DId = {}; // identificatore
        vector<vector<unsigned int>> Cell2DVertices = {}; // lista dei vertici (la lunghezza è variabile)
        vector<vector<unsigned int>> Cell2DEdges = {}; // lista dei lati
        map<unsigned int, list<unsigned int>> Cell2DMarkers = {}; // marker
    };

}

