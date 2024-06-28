#ifndef geneticos_H
#define geneticos_H

#include <vector>
#include <utility> // for std::pair
#include "mochila.h"

#ifndef _MAIN
extern int n; //Nº objetos
extern vector<int> pesos;
extern vector<vector<int >> b_interdependencias;
extern vector<int> b_individual;
extern vector<Objeto> objeto;
#endif

vector<Mochila> poblacionInicial(const int& n, int capacidad, const int tam_poblacion);

Mochila torneo(const Mochila& c1, const Mochila& c2, const  Mochila& c3);

void cruceUniforme(Mochila& c1, Mochila& c2, int W);

void cruceDosPuntos(Mochila& c1, Mochila& c2, int W);

void cruceUniformePesado(Mochila& c1, Mochila& c2, int W);

int diversidad(vector<Mochila>& poblacion);

int mutacion(const vector<bool>& c, int& peso_actual, const int& W);

vector<bool> mejorSolucion(vector<Mochila>& poblacion);

using CruceFunc =  void (*)(Mochila& c1, Mochila& c2, int W);
vector<bool> AGG(vector<Mochila>& poblacion_actual, const int cromosomas, const int W, int& iteracion, const int& max_iteraciones, const double& prob_cruce,
                 const double& prob_mutacion, int& tam_poblacion, CruceFunc cruce);

vector<bool> AGE(vector<Mochila>& poblacion_actual, const int cromosomas, const int W, int& iteracion, const int& max_iteraciones, const double& prob_cruce,
                 const double& prob_mutacion, int& tam_poblacion, CruceFunc cruce);





#endif // geneticos_H