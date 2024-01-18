//
//  DiagramaVoronoi.hpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 23/08/23.
//  Copyright © 2023 Márcio Sarroglia Pinho. All rights reserved.
//

#ifndef DiagramaVoronoi_h
#define DiagramaVoronoi_h

#include <iostream>
#include <fstream>
using namespace std;

#include "Poligono.h"
#include "Envelope.h"
#include "ListaDeCoresRGB.h"


class Voronoi
{
    Poligono Diagrama[1000];
    Envelope Envelopes[1000];
    unsigned int qtdDePoligonos;
    Ponto Min, Max;
public:
    Voronoi();
    Poligono LeUmPoligono();
    void LePoligonos(const char *nome);
    Poligono getPoligono(int i);
    void obtemLimites(Ponto &min, Ponto &max);
    unsigned int getNPoligonos();
    void obtemVizinhosDasArestas();
    Envelope getEnvelope(int i);
    void insereEnvelope(Envelope envelope);
    int testeInclusaoConvexo(Ponto pontoMovel, int& posicaoPoligono);
    int TestArestas (int index, Ponto pontoMovel);
    int testVizinhanca(int origem, Ponto pontoMovel);
};
#endif /* DiagramaVoronoi_h */
