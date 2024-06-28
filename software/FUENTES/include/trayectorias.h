#ifndef trayectorias_H
#define trayectorias_H

#include <vector>
#include <utility> // for std::pair
#include "mochila.h"
#include "memeticos.h"
#include "geneticos.h"
#include "BL.h"

#ifndef _MAIN
extern int n; //Nº objetos
extern vector<int> pesos;
extern vector<vector<int >> b_interdependencias;
extern vector<int> b_individual;
extern vector<Objeto> objeto;
#endif


vector<bool> BMB(const int& max_iteraciones, const int& max_evaluaciones_BL, const int& n, const int& W);

Mochila mutacionILS(const Mochila& mejor, const int& n, const int& W, const int& t);

vector<bool> ILS(const int& max_iteraciones, const int& max_evaluaciones_BL, const int& n, const int& W);

using VecinosFunc = vector<pair<int, int>> (*)(const Mochila&, const int&);
vector<bool> ES(Mochila& actual, const int n, const int W, const int& max_instancias, int& n_instancias, const double& phi,
                const double& mu, VecinosFunc Vecinos);

vector<bool> ILS_ES(const int& max_iteraciones, const int& max_evaluaciones_BL, const int& n, const int& W);



#endif // trayectorias_H