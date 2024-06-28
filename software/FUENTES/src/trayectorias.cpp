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
#include "../include/trayectorias.h"


using namespace std;
using Random = effolkronium::random_static;

vector<bool> BMB(const int& max_iteraciones, const int& max_evaluaciones_BL, const int& n, const int& W){
    Mochila mejor;
    vector<Mochila> actual(max_iteraciones);

    actual = poblacionInicial(n, W, max_iteraciones);

    for (int i=0; i<max_iteraciones; i++){
        bool max_local;
        int evaluaciones = 0;
        actual[i].setAsignacion(busquedaLocal(actual[i], n, W, max_evaluaciones_BL, evaluaciones, max_local, VecinosPermutacion, -1));
        if (actual[i].getFitness()>mejor.getFitness()){
            mejor = actual[i];
        }
    }
    return mejor.getAsignacion();
}

Mochila mutacionILS(const Mochila& mejor, const int& n, const int& W, const int& t){
    Mochila actual = mejor;
    int rand_indice1 = Random::get(0, n-1);
    int rand_indice2 = (rand_indice1+t)%n;


    vector<bool> asignacion = actual.getAsignacion();
    vector<bool> segmento(t);
    if (rand_indice1<rand_indice2){
        copy(asignacion.begin()+rand_indice1, asignacion.begin()+rand_indice2, segmento.begin());
    }
    else{
        copy(asignacion.begin()+rand_indice1, asignacion.end(), segmento.begin());
        copy(asignacion.begin(), asignacion.begin()+rand_indice2, segmento.begin()+n-rand_indice1);
    }

    Random::shuffle(segmento);
    if (rand_indice1<rand_indice2){
        copy(segmento.begin(), segmento.end(), asignacion.begin()+rand_indice1);
    }
    else{
        copy(segmento.begin(), segmento.begin()+(n-rand_indice1), asignacion.begin()+rand_indice1);
        copy(segmento.begin()+(n-rand_indice1), segmento.end(), asignacion.begin());
    }
    actual.setAsignacionE(asignacion);
    asignacion = actual.getAsignacion();
    while(actual.getPeso()>W){
        int i = 0;
        if (rand_indice1<rand_indice2){
            i = Random::get(rand_indice1, rand_indice2);
        }
        else{
            if (Random::get(0, 1)){
                i = Random::get(rand_indice1, n-1);
            }
            else{
                i = Random::get(0, rand_indice2);
            }
        }
        if (asignacion[i]){
            asignacion[i] = false;
            actual.setAsignacionE(asignacion);
        }
    }
    actual.Fitness();
    return actual;
}


double enfriamientoCauchy(const double& T, const double& beta){
    return T/(1+beta*T);
}

using VecinosFunc = vector<pair<int, int>> (*)(const Mochila&, const int&);
vector<bool> ES(Mochila& actual, const int n, const int W, const int& max_instancias, int& n_instancias, const double& phi,
                const double& mu, VecinosFunc Vecinos){
    
    Mochila mejor = actual;
    const int max_vecinos = 5*n;
    const int max_exitos = 0.1*max_vecinos;
    const int M = max_instancias/max_vecinos;
    const double T0 = (mu*actual.getFitness())/(-log(phi));
    double Tf = 0.001;
    while(Tf>=T0){
        Tf = Tf / 10;
    }
    const double beta = (T0-Tf)/(M*T0*Tf);

    double T = T0;
    int exitos = 0;
    int num_vecinos = 0;
    vector<pair<int, int>> vecinos = Vecinos(actual, W);
    while (T>Tf && n_instancias<max_instancias && exitos<max_exitos && num_vecinos<max_vecinos){
        int exitos = 0;
        int num_vecinos = 0;
        vector<pair<int, int>> vecinos = Vecinos(actual, W);
        Random::shuffle(vecinos);
        while (!vecinos.empty() && n_instancias<max_instancias && num_vecinos<max_vecinos && exitos<max_exitos){
            
            ++num_vecinos;
            int i1 = vecinos.back().first;
            int i0 = vecinos.back().second;
            vecinos.pop_back();
            vector<bool> asignacion_nueva = actual.getAsignacion();
            Permutacion(asignacion_nueva, i0, i1); // Función para aplicar permutación 
            Mochila nueva = actual;
            nueva.factorizacionFitness(asignacion_nueva, i0, i1);
            n_instancias++;

            int delta = actual.getFitness() - nueva.getFitness();
            if (delta<0 || Random::get<double>(0, 1) <= exp(-delta/T)){
                actual = nueva;
                ++exitos;
                if(nueva.getFitness()>mejor.getFitness()){
                    mejor = nueva;
                }
                break;
            }

        }
        if (exitos==0){
            break;
        }
        if (!vecinos.empty()){
            T = enfriamientoCauchy(T, beta);
        }
    }
    return mejor.getAsignacion();
}

vector<bool> ILS(const int& max_iteraciones, const int& max_evaluaciones_BL, const int& n, const int& W){
    Mochila actual;
    Mochila mejor = solucionInicial(n, W);
    int iteraciones = 1;
    int evaluaciones = 0;
    bool max_local;
    mejor.setAsignacion(busquedaLocal(mejor, n, W, max_evaluaciones_BL, evaluaciones, max_local, VecinosPermutacion, -1));
    for (iteraciones; iteraciones<max_iteraciones; iteraciones++){
        actual = mutacionILS(mejor, n, W, 20);
        int evaluaciones = 0;
        actual.setAsignacion(busquedaLocal(actual, n, W, max_evaluaciones_BL, evaluaciones, max_local, VecinosPermutacion, -1));
        if (actual.getFitness()>mejor.getFitness()){
            mejor = actual;
        }
    }
    return mejor.getAsignacion();
}

vector<bool> ILS_ES(const int& max_iteraciones, const int& max_evaluaciones_BL, const int& n, const int& W){
    Mochila actual;
    Mochila mejor = solucionInicial(n, W);
    int iteraciones = 1;
    int evaluaciones = 0;
    bool max_local;
    mejor.setAsignacion(ES(mejor, n, W, max_evaluaciones_BL, evaluaciones, 0.3, 0.1, VecinosPermutacion));
    for (iteraciones; iteraciones<max_iteraciones; iteraciones++){
        actual = mutacionILS(mejor, n, W, 20);
        int evaluaciones = 0;
        actual.setAsignacion(ES(actual, n, W, max_evaluaciones_BL, evaluaciones, 0.3, 0.1, VecinosPermutacion));
        if (actual.getFitness()>mejor.getFitness()){
            mejor = actual;
        }
    }
    return mejor.getAsignacion();
}
