#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "../tools/random.hpp"
#include <chrono>
#include <cmath>
#include <random>
#include <unordered_set>
#include <utility>
#include <cstring>
#include <chrono>
#include <ctime>
#include "../include/memeticos.h"


using namespace std;
using Random = effolkronium::random_static;

using CruceFunc =  void (*)(Mochila& c1, Mochila& c2, int W);

using GeneticoFunc = vector<bool> (*)(vector<Mochila>& poblacion_actual, const int genes, const int W, int& iteracion, const int& max_iteraciones, const double& prob_cruce,
                 const double& prob_mutacion, int& tam_poblacion, CruceFunc cruce);

using VecinosFunc = vector<pair<int, int>> (*)(const Mochila&, const int&);

struct ordenFitness {
    bool operator()(const Mochila& m1, const Mochila& m2) const {
        return m1.getFitness() > m2.getFitness();
    }
};

vector<bool> AM(vector<Mochila>& poblacion_actual, const int genes, const int W, const int& max_iteraciones, const double& prob_cruce,
                 const double& prob_mutacion, int& tam_poblacion, CruceFunc cruce, GeneticoFunc genetico, int aplicacion_BL,
                 const double& prob_aplicacion_BL, bool BL_elitista,  VecinosFunc vecinos){
    
    int iteracion = tam_poblacion;
    while(iteracion<max_iteraciones){

        int max;
        if (iteracion+aplicacion_BL*tam_poblacion <= max_iteraciones){
            max = aplicacion_BL*tam_poblacion;
        }
        else{
            max = max_iteraciones-iteracion;
        }
        int it = 0;
        genetico(poblacion_actual, genes, W, it, max, prob_cruce, prob_mutacion, tam_poblacion, cruce);
        iteracion += max;
        int iteraciones_BL;
        bool maximo_local;
        int bl_esperado =  tam_poblacion*prob_aplicacion_BL;
        if (BL_elitista){
            sort(poblacion_actual.begin(), poblacion_actual.end(), ordenFitness());
        }
        else if (prob_aplicacion_BL < 1.0){
            Random::shuffle(poblacion_actual);
        }
        //cout << "Mejor solucion antes BL: " << min_element(poblacion_actual.begin(), poblacion_actual.end(), ordenFitness())->getFitness() << endl;
        for (int i=0; i<bl_esperado && iteracion<max_iteraciones; i++){
            iteraciones_BL = 0;
            busquedaLocal(poblacion_actual[i], genes, W, max_iteraciones, iteraciones_BL, maximo_local, vecinos, genes);
            iteracion += iteraciones_BL;
        }
        //cout << "Mejor solucion despues BL: " << min_element(poblacion_actual.begin(), poblacion_actual.end(), ordenFitness())->getFitness() << endl;
    }
    return min_element(poblacion_actual.begin(), poblacion_actual.end(), ordenFitness())->getAsignacion();
}
