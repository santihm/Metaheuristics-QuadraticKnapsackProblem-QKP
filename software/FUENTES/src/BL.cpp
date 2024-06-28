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
#include "../include/BL.h"


using namespace std;
using Random = effolkronium::random_static;




/*
vector<bool> solucionInicial(const int& n, int capacidad){
  vector<bool> s(n, false);
  vector<Objeto> o = objeto;
  Random::shuffle(o);

  while(!o.empty()){
    //cout << "Capacidad: " << capacidad << endl;
    if (capacidad>=pesos[o.size()-1] && !s[o.size()-1]){
      capacidad -= pesos[o.size()-1];
      s[o.size()-1] = true;
      o.pop_back();
    }
    else{
      o.pop_back();
    }
  }
  Mochila m(s);
  ////cout << "Fitness inicial: " << m.getFitness() << endl;
  return s;
}
*/

Mochila solucionInicial(const int& n, int capacidad){
    vector<bool> s(n, false);
        Mochila m(s);
        vector<Objeto> o = objeto;
        Random::shuffle(o);
        while(!o.empty()){
            if (m.getPeso() + o.back().peso <= capacidad){
                m.setAsignacionIndice(o.back().indice);
            }
            o.pop_back();
        }
        m.Fitness();
        return m;
}


vector<pair<int, int>> VecinosPermutacion(const Mochila& actual, const int& W) {
    vector<pair<int, int>> vecinos;
    const vector<bool>& v = actual.getAsignacion();

    int peso_actual = actual.getPeso();
    for (int i = 0; i < n; i++) {
        if (v[i]) {
            for (int j = 0; j < n; j++) {
                if (!v[j] && i != j) {  // Evitar intercambiar un objeto consigo mismo
                    if (peso_actual - pesos[i] + pesos[j] <= W) {
                        vecinos.push_back({i, j});
                    }
                }
            }
        }
    }//
    return vecinos;
}





/*
vector<pair<int, vector<vector<int>>>> VecinosDobleIntercambio(const Mochila& actual, const int& capacidad) {
    vector<pair<int, vector<vector<int>>>> vecinos = VecinosPermutacion(actual, capacidad);
    const vector<Objeto>& objeto = actual.getObjeto(); // Se corrige el acceso a las referencias
    const vector<bool>& v = actual.getAsignacion(); // Se corrige el acceso a las referencias

    for (int i = 0; i < vecinos.size(); i++) {
        vector<vector<int>>& ceros = vecinos[i].second;
        int objeto_actual = vecinos[i].first;
        for (int j = 0; j < ceros.size(); j++) {
            int objeto_cero = ceros[j][0];
            int capacidad_actual = capacidad - objeto[objeto_actual].getPeso() + objeto[objeto_cero].getPeso();
            for (int k = 0; k < objeto.size(); k++) {
                if (!v[k] && k != objeto_actual && k != objeto_cero) {
                    int nuevo_peso = capacidad_actual + objeto[k].getPeso();
                    if (nuevo_peso >= 0 && nuevo_peso <= capacidad) {
                        vector<int> grupo = ceros[j];
                        if (grupo.size() < 2) { // Control de tamaño máximo del grupo
                            grupo.push_back(k);
                            vecinos[i].second.push_back(grupo);
                        }
                    }
                }
            }
        }
    }
    return vecinos;
}
*/


void Permutacion(vector<bool>& v, const int& p1, const int& p0){
  //cout << "Antes permutacion" << endl;
  v[p1] = !v[p1];
  v[p0] = !v[p0];
  //cout << "Después permutacion" << endl;
}

void Intercambio(vector<bool>&v, const int&p0, const vector<int>& p1){
  v[p0] = !v[p0];
  for (int i=0; i<p1.size(); i++){
    v[p1[i]] = !v[p1[i]];
  }
}


using VecinosFunc = vector<pair<int, int>> (*)(const Mochila&, const int&);
vector<bool> busquedaLocal(Mochila& actual, const int n, const int W, const int& max_instancias, int& n_instancias,
                           bool& maximo_local, VecinosFunc Vecinos, int max_vecinos_evaluados) {
    vector<bool> mejor_asignacion = actual.getAsignacion();
    //cout << "Fitness inicial: " << actual.getFitness() << endl;
    int mejor_fitness = actual.getFitness();
    int capacidad = W;
    maximo_local = false;
    int vecinos_evaluados = 0;
    if (max_vecinos_evaluados == -1) {
        max_vecinos_evaluados = n * (n - 1) / 2;
    }

    while (n_instancias < max_instancias && !maximo_local && vecinos_evaluados < max_vecinos_evaluados) {
        vector<pair<int, int>> vecinos = Vecinos(actual, capacidad);
        Random::shuffle(vecinos);
        
        bool mejora_encontrada = false;
        while (n_instancias < max_instancias && !vecinos.empty() && !mejora_encontrada && vecinos_evaluados < max_vecinos_evaluados) {
            //mejora_encontrada = false;
            ++vecinos_evaluados;
            int i1 = vecinos.back().first;
            int i0 = vecinos.back().second;
            vecinos.pop_back();
            vector<bool> asignacion_nueva = actual.getAsignacion();
            Permutacion(asignacion_nueva, i0, i1); // Función para aplicar permutación
            Mochila nueva = actual;
            nueva.factorizacionFitness(asignacion_nueva, i0, i1);
            ++n_instancias;
            if (nueva.getFitness() > actual.getFitness()) { // Verificar si mejora el fitness
                actual = nueva; // Actualizar el estado de la mochila solo si se encontró una mejora
                mejora_encontrada = true;
            } 
        }
        
        if (!mejora_encontrada) {
            maximo_local = true; // No se encontró mejora en esta iteración
        }
        
        
        //cout << "Fitness: " << actual.getFitness() << endl;
    }
    
    return actual.getAsignacion();
}


/*
using VecinosFunc = vector<pair<int, vector<int>>> (*)(const Mochila&, const int&);
vector<bool> busquedaLocalMejor(Mochila& actual, const int n, const int W, const int& max_instancias, int& n_instancias,
                                 bool& maximo_local, VecinosFunc Vecinos) {
    vector<bool> mejor_asignacion = actual.getAsignacion();
    int mejor_fitness = actual.getFitness();
    int capacidad = W;
    n_instancias = 0;
    vector<Objeto> objeto = actual.getObjeto();
    maximo_local = false;

    while (n_instancias < max_instancias && !maximo_local) {
        vector<pair<int, vector<vector<int>>>> vecinos = Vecinos(actual, capacidad);

        bool mejora_encontrada = false;
        Mochila mejor_mochila = actual; // Mantener un seguimiento de la mejor mochila encontrada en esta iteración

        for (const auto& vecino : vecinos) {
            const int indice_objeto = vecino.first;
            const vector<vector<int>>& intercambios = vecino.second;

            for (const auto& intercambio : intercambios) {
                vector<bool> v = actual.getAsignacion();
                Intercambio(v, indice_objeto, intercambio); // Función para aplicar intercambio
                Mochila nueva(objeto, v);

                if (nueva.getFitness() > mejor_fitness) { // Verificar si mejora el fitness
                    mejor_asignacion = v;
                    mejor_fitness = nueva.getFitness();
                    mejor_mochila = nueva; // Actualizar la mejor mochila encontrada
                    mejora_encontrada = true;
                    //cout << mejor_fitness << endl;
                }
            }
        }

        if (mejora_encontrada) {
            actual = mejor_mochila; // Actualizar el estado de la mochila solo si se encontró una mejora
        } else {
            maximo_local = true; // No se encontró mejora en esta iteración
        }

        ++n_instancias;
    }

    return mejor_asignacion;
}
*/


