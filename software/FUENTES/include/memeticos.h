#ifndef memeticos_H
#define memeticos_H

#include <vector>
#include <utility> // for std::pair
#include "mochila.h"
#include "geneticos.h"
#include "BL.h"

#ifndef _MAIN
extern int n; //Nº objetos
extern vector<int> pesos;
extern vector<vector<int >> b_interdependencias;
extern vector<int> b_individual;
extern vector<Objeto> objeto;
#endif

using CruceFunc =  void (*)(Mochila& c1, Mochila& c2, int W);

using GeneticoFunc = vector<bool> (*)(vector<Mochila>& poblacion_actual, const int genes, const int W, int& iteracion, const int& max_iteraciones, const double& prob_cruce,
                 const double& prob_mutacion, int& tam_poblacion, CruceFunc cruce);

using VecinosFunc = vector<pair<int, int>> (*)(const Mochila&, const int&);

vector<bool> AM(vector<Mochila>& poblacion_actual, const int genes, const int W, const int& max_iteraciones, const double& prob_cruce,
                 const double& prob_mutacion, int& tam_poblacion, CruceFunc cruce, GeneticoFunc genetico, int aplicacion_BL,
                 const double& prob_aplicacion_BL, bool BL_elitista,  VecinosFunc vecinos);



#endif // memeticos_H