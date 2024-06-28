#ifndef BL_H
#define BL_H

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

Mochila solucionInicial(const int& n, int capacidad);
vector<pair<int, int>> VecinosPermutacion(const Mochila& actual, const int& W);
//vector<pair<int, vector<vector<int>>>> VecinosDobleIntercambio(const Mochila& actual, const int& capacidad);

void Permutacion(vector<bool>& v, const int& p1, const int& p0);
void Intercambio(vector<bool>& v, const int& p1, const vector<int>& p0);

using VecinosFunc = vector<pair<int, int>> (*)(const Mochila&, const int&);

vector<bool> busquedaLocal(Mochila& actual, const int n, const int W, const int& max_instancias, int& n_instancias,
                            bool& maximo_local, VecinosFunc Vecinos, int max_vecinos_evaluados);
                            


//vector<bool> busquedaLocalMejor(Mochila& actual, const int n, const int W, const int& max_instancias, int& n_instancias,
//                           bool& maximo_local, VecinosFunc Vecinos);



#endif // BL_H
