//
//  DiagramaVoronoi.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 23/08/23.
//  Copyright © 2023 Márcio Sarroglia Pinho. All rights reserved.
//

#include "DiagramaVoronoi.h"

ifstream input;            // ofstream arq;

Voronoi::Voronoi()
{

}
Poligono Voronoi::LeUmPoligono()
{
    Poligono P;
    unsigned int qtdVertices;
    input >> qtdVertices;  // arq << qtdVertices
    for (int i=0; i< qtdVertices; i++)
    {
        double x,y;
        // Le um ponto
        input >> x >> y;
        Ponto(x, y).imprime();
        if(!input)
        {
            cout << "Fim inesperado da linha." << endl;
            break;
        }
        P.insereVertice(Ponto(x,y));
    }
    cout << "Poligono lido com sucesso!" << endl;
    return P;
}

void Voronoi::LePoligonos(const char *nome)
{
    input.open(nome, ios::in); //arq.open(nome, ios::out);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    string S;

    input >> qtdDePoligonos;
    cout << "qtdDePoligonos:" << qtdDePoligonos << endl;
    Ponto A, B;
    Diagrama[0] = LeUmPoligono();
    Diagrama[0].obtemLimites(Min, Max);// obtem o envelope do poligono
    Envelopes[0] = Envelope(Min,Max);

    for (int i=1; i< qtdDePoligonos; i++)
    {
        Diagrama[i] = LeUmPoligono();
        Diagrama[i].obtemLimites(A, B); // obtem o envelope do poligono
        Envelopes[i] = Envelope(A,B);

        Min = ObtemMinimo (A, Min);
        Max = ObtemMaximo (B, Max);
    }
    cout << "Lista de Poligonos lida com sucesso!" << endl;

}

Poligono Voronoi::getPoligono(int i)
{
    if (i >= qtdDePoligonos)
    {
        cout << "Nro de Poligono Inexistente" << endl;
        return Diagrama[0];
    }
    return Diagrama[i];
}

Envelope Voronoi::getEnvelope(int i){
    return Envelopes[i];
}

unsigned int Voronoi::getNPoligonos()
{
    return qtdDePoligonos;
}

void Voronoi::obtemLimites(Ponto &min, Ponto &max)
{
    min = this->Min;
    max = this->Max;
}

//Método para obter todos os vizjnhos
void Voronoi::obtemVizinhosDasArestas(){
    Poligono poligono;
    Poligono poligonoVizinho;
    int nVertices;
    int nVerticesVizinho;

    for(int eu = 0; eu<qtdDePoligonos; eu++){
        poligono = Diagrama[eu];
        nVertices = poligono.getNVertices();

        for(int meusPontos = 0; meusPontos< nVertices; meusPontos++){

                Ponto euA,euB;
                poligono.getAresta(meusPontos, euA, euB);

            for(int vizinho = 0; vizinho<qtdDePoligonos; vizinho++){

                poligonoVizinho = Diagrama[vizinho];
                nVerticesVizinho = poligonoVizinho.getNVertices();

                for(int vizinhoPontos = 0; vizinhoPontos < nVerticesVizinho; vizinhoPontos++){

                    //Ponto euP (poligono.getVertice(meusPontos).x, poligono.getVertice(meusPontos).y, poligono.getVertice(meusPontos).z);
                    //Ponto vizinhoP(poligonoVizinho.getVertice(vizinhoPontos).x, poligonoVizinho.getVertice(vizinhoPontos).y, poligonoVizinho.getVertice(vizinhoPontos).z);
                    Ponto vizA, vizB;
                    poligonoVizinho.getAresta(vizinhoPontos, vizA, vizB);

                    if(euA == vizB && euB == vizA)
                    {

                            Diagrama[eu].insereVizinho(vizinho);
                            //poligonoVizinho.insereVizinho(eu);
                    }
                }
            }
        }
        cout << "Poligono " << eu << "  |  N. Vizinhos " << Diagrama[eu].getNVizinhos() << endl;
    }
}

//Teste de Inclusão de Poligonos Convexos
int Voronoi::testeInclusaoConvexo(Ponto pontoMovel, int& posicaoPoligono){
    Ponto verticesEnv [4];
    Ponto miP, maP, P1, v1, v2;
    int qtdTF = 0;

    for(int env = 0; env < getNPoligonos(); env++){

        Envelope envelope = Envelopes[env];

        miP = envelope.Min;
        maP= envelope.Max;
        int cont = 0;
        verticesEnv[0]=miP;
        verticesEnv[1]=Ponto(miP.x,maP.y,0);
        verticesEnv[2]=maP;
        verticesEnv[3]=Ponto(maP.x,miP.y,0);

        //Produto vetorial de arestas de todos envelopes,
        //comparando com ponto que estara sendo movido

        if(envelope.pontoEstaDentro(pontoMovel)){
            for(int aresta = 0; aresta < 4; aresta++){

                v1 = verticesEnv[(aresta+1)%4] - verticesEnv[aresta];
                v2 = verticesEnv[(aresta+1)%4] - pontoMovel;
                ProdVetorial(v1, v2, P1);
                qtdTF++;

                if(P1.z < 0.0){
                    cont = 0;
                   break;
                }
                cont++;
            }
        }
         if(cont == 4){
            int tam = Diagrama[env].getNVertices();

            for(int i = 0; i < tam; i++){
                Diagrama[env].getAresta(i,miP,maP);

                v1 = maP - miP;
                v2 = maP - pontoMovel;

                ProdVetorial(v1, v2, P1);
                qtdTF++;

                if(P1.z < 0.0){
                    cont = 0;
                   break;
                }
            }
            if(cont != 0){
            cout << "Numero de chamados do ProdVetorial() no testeInclusaoConvexo: " << qtdTF << endl;
            posicaoPoligono = env;

            return posicaoPoligono;

            }
            cont = 0;

        }
        else{

        }

    }
    cout << "Numero de chamados do ProdVetorial() no testeInclusaoConvexo: " << qtdTF << endl;
}

//Teste para verificar arestas
int Voronoi::TestArestas (int index, Ponto pontoMovel){
    Ponto Esq;
    Ponto Dir (-1,0);
    Esq = pontoMovel + Dir * (1000);
    Ponto miP, maP, P1, v1, v2;
    Ponto verticesEnv [4];
    int qtdTF = 0;

    vector <int> interseccoes;


    for(int env = 0; env < getNPoligonos(); env++){

        Envelope envelope = Envelopes[env];

        miP = envelope.Min;
        maP= envelope.Max;
        int cont = 0;
        verticesEnv[0]=miP;
        verticesEnv[1]=Ponto(miP.x,maP.y,0);
        verticesEnv[2]=maP;
        verticesEnv[3]=Ponto(maP.x,miP.y,0);

        if(HaInterseccao(verticesEnv[0], verticesEnv[1], Esq, pontoMovel) == true ||
            HaInterseccao(verticesEnv[2], verticesEnv[3], Esq, pontoMovel) == true){
                interseccoes.push_back(env);
            }

        }

    for(int i = 0; i < interseccoes.size(); i++){
        Poligono poligono = Diagrama[interseccoes[i]];
        int cont=0;
        for(int arestas = 0; arestas < poligono.getNVertices(); arestas++){
            poligono.getAresta(arestas,miP,maP);

            if(HaInterseccao(miP, maP, Esq, pontoMovel) == true){

                cont++;
            }
        }
        if(cont%2 != 0){
            cout << "Numero de chamados do HaInterseccao() no TestArestas: " << getContadorInt() << endl;
            resetContadorInt();
            return interseccoes[i];
        }
    }

}

//Verificação dos vizinhos caso polígono tenha trocado de lugar
int Voronoi::testVizinhanca(int origem, Ponto pontoMovel ){
    //int poligonoAtual = testeInclusaoConvexo(pontoMovel);
    //Diagrama[poligonoAtual].Vizinhos.size()

    Ponto miP, maP, P1, v1, v2;
    int qtdTF = 0;
    //fazer getNVizinhos
    //cout <<"Numero de Vizinhos = "<< Diagrama[origem].getNVizinhos()<< endl;
    for(int v = 0; v < Diagrama[origem].getNVizinhos(); v++ )
    {
        int tam = Diagrama[origem].getNVertices();

            for(int i = 0; i < tam; i++){
                Diagrama[origem].getAresta(i,miP,maP);

                v1 = maP - miP;
                v2 = maP - pontoMovel;

                ProdVetorial(v1, v2, P1);
                qtdTF++;
                //cout << "i: " << i << endl;
                if(P1.z < 0.0){
                    cout << "Numero de chamados do ProdVetorial() no testVizinhanca: " << qtdTF << endl;
                    return i;
                   break;
                }
        }
    }
    cout << "Numero de chamados do ProdVetorial() no testVizinhanca: " << qtdTF << endl;

    return origem;
}

