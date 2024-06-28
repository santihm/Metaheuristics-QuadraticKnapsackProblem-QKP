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
#include <cmath>
#include "../include/geneticos.h"
#include "../include/BL.h"

using namespace std;
using Random = effolkronium::random_static;





vector<Mochila> poblacionInicial(const int& n, int capacidad, const int tam_poblacion){
    vector<Mochila> pobInicial;
    for (int i=0; i<tam_poblacion; i++){
        vector<bool> s(n, false);
        Mochila m(s);
        vector<Objeto> o = objeto;
        while(!o.empty()){
            Random::shuffle(o);
            if (m.getPeso() + o.back().peso <= capacidad){
                m.setAsignacionIndice(o.back().indice);
            }
            o.pop_back();
        }
        m.Fitness();
        pobInicial.push_back(m);
    }
    return pobInicial;
}

Mochila torneo(const Mochila& c1, const Mochila& c2, const Mochila& c3){
    if (c1.getFitness() > c2.getFitness() && c1.getFitness() > c3.getFitness()){
        return c1;
    }
    else if (c2.getFitness() > c1.getFitness() && c2.getFitness() > c3.getFitness()){
        return c2;
    }
    else{
        return c3;
    }
}

void ajustarFactibilidad(vector<bool>& hijo, const vector<Objeto>& objetos, int W) {
    int peso = 0;
    // Calculamos el peso de la mochila
    for (int i = 0; i < n; i++) {
        if (hijo[i]) {
            peso += pesos[i];
        }
    }
    // Si el peso supera la capacidad máxima, eliminamos objetos aleatoriamente hasta que sea factible
    while (peso > W) {
        int idx = Random::get(0, (int) hijo.size() - 1);
        if (hijo[idx]) {
            hijo[idx] = false;
            peso -= pesos[idx];
        }
    }
}

void cruceDosPuntos(Mochila& c1, Mochila& c2, int W){
    vector<bool> s1 = c1.getAsignacion();
    vector<bool> s2 = c2.getAsignacion();
    vector<bool> hijo1 = s1;
    vector<bool> hijo2 = s2;
    int p1 = Random::get(0, n-1);
    int p2 = Random::get(p1, n-1);
    int peso_h1 = 0, peso_h2 = 0;

    for (int i = 0; i < n; i++) {
        if (i >= p1 && i < p2) {
            hijo1[i] = s2[i];
            hijo2[i] = s1[i];
        }
        if (hijo1[i]) peso_h1 += pesos[i];
        if (hijo2[i]) peso_h2 += pesos[i];
    }

    while(peso_h1 > W){
        int i = Random::get(0, n-1);
        if (hijo1[i]){
            hijo1[i] = false;
            peso_h1 -= pesos[i];
        }
    }

    while(peso_h2 > W){
        int i = Random::get(0, n-1);
        if (hijo2[i]){
            hijo2[i] = false;
            peso_h2 -= pesos[i];
        }
    }

    c1.setAsignacionE(hijo1);
    c2.setAsignacionE(hijo2);
}

void cruceUniforme(Mochila& c1, Mochila& c2, int W) {
    vector<bool> s1 = c1.getAsignacion();
    vector<bool> s2 = c2.getAsignacion();
    vector<bool> hijo1(s1.size());
    vector<bool> hijo2(s2.size());

    for (int i = 0; i < s1.size(); i++) {
        // Escoger aleatoriamente el gen del padre 1 o 2 para el hijo 1
        if (Random::get(0, 1) == 0) {
            hijo1[i] = s1[i];
        } else {
            hijo1[i] = s2[i];
        }
        // Escoger aleatoriamente el gen del padre 1 o 2 para el hijo 2
        if (Random::get(0, 1) == 0) {
            hijo2[i] = s1[i];
        } else {
            hijo2[i] = s2[i];
        }
    }

    // Ajustar los pesos de los hijos para asegurar que no superen la capacidad de la mochila
    int peso_h1 = 0, peso_h2 = 0;
    for (int i = 0; i < s1.size(); i++) {
        if (hijo1[i]) peso_h1 += pesos[i];
        if (hijo2[i]) peso_h2 += pesos[i];
    }

    // Ajustar el hijo 1 si excede el límite de peso
    while (peso_h1 > W) {
        int i = Random::get(0, n-1);
        if (hijo1[i]) {
            hijo1[i] = false;
            peso_h1 -= pesos[i];
        }
    }

    // Ajustar el hijo 2 si excede el límite de peso
    while (peso_h2 > W) {
        int i = Random::get(0, n - 1);
        if (hijo2[i]) {
            hijo2[i] = false;
            peso_h2 -= pesos[i];
        }
    }

    // Establecer las asignaciones de los hijos
    c1.setAsignacionE(hijo1);
    c2.setAsignacionE(hijo2);
}

void cruceUniformePesado(Mochila& c1, Mochila& c2, int W) {
    vector<bool> s1 = c1.getAsignacion();
    vector<bool> s2 = c2.getAsignacion();
    vector<bool> hijo1(s1.size());
    vector<bool> hijo2(s2.size());

    // Realizar el cruce uniforme
    for (int i = 0; i < s1.size(); i++) {
        // Escoger aleatoriamente el gen del padre 1 o 2 para el hijo 1
        if (Random::get(0, 1) == 0) {
            hijo1[i] = s1[i];
        } else {
            hijo1[i] = s2[i];
        }
        // Escoger aleatoriamente el gen del padre 1 o 2 para el hijo 2
        if (Random::get(0, 1) == 0) {
            hijo2[i] = s1[i];
        } else {
            hijo2[i] = s2[i];
        }
    }

    // Ajustar los pesos de los hijos para asegurar que no superen la capacidad de la mochila
    int peso_h1 = 0, peso_h2 = 0;
    for (int i = 0; i < s1.size(); i++) {
        if (hijo1[i]) peso_h1 += pesos[i];
        if (hijo2[i]) peso_h2 += pesos[i];
    }

    // Ajustar el hijo 1 si excede el límite de peso
    while (peso_h1 > W) {
        // Encontrar el objeto con el mayor peso y eliminarlo
        int max_peso_index = -1;
        int max_peso = -1;
        for (int i = 0; i < s1.size(); i++) {
            if (hijo1[i] && pesos[i] > max_peso) {
                max_peso = pesos[i];
                max_peso_index = i;
            }
        }
        // Eliminar el objeto de mayor peso
        if (max_peso_index != -1) {
            hijo1[max_peso_index] = false;
            peso_h1 -= pesos[max_peso_index];
        }
    }

    // Ajustar el hijo 2 si excede el límite de peso
    while (peso_h2 > W) {
        // Encontrar el objeto con el mayor peso y eliminarlo
        int max_peso_index = -1;
        int max_peso = -1;
        for (int i = 0; i < s2.size(); i++) {
            if (hijo2[i] && pesos[i] > max_peso) {
                max_peso = pesos[i];
                max_peso_index = i;
            }
        }
        // Eliminar el objeto de mayor peso
        if (max_peso_index != -1) {
            hijo2[max_peso_index] = false;
            peso_h2 -= pesos[max_peso_index];
        }
    }

    // Establecer las asignaciones de los hijos
    c1.setAsignacionE(hijo1);
    c2.setAsignacionE(hijo2);
}


int mutacion(const vector<bool>& c, int& peso_actual, const int& W){
    vector<bool> s_mut = c;
    int gen_mutar;
    bool factible = false;
    while(!factible){
        gen_mutar = Random::get(0, n-1);
        if (!s_mut[gen_mutar] && (peso_actual + pesos[gen_mutar]) > W){
            factible = false;
        }
        else{
            factible = true;
        }
    }
    return gen_mutar;
}

struct ordenFitness {
    bool operator()(const Mochila& m1, const Mochila& m2) const {
        return m1.getFitness() > m2.getFitness();
    }
};

vector<bool> mejorSolucion(vector<Mochila>& poblacion){
    int max = 0;
    int index = 0;
    for (int i=0; i<poblacion.size(); i++){
        if (poblacion[i].getFitness() > max){
            max = poblacion[i].getFitness();
            index = i;
        }
    }
    return poblacion[index].getAsignacion();
}

int diversidad(vector<Mochila>& poblacion){
    vector<vector<bool>> soluciones;
    for (int i=0; i<poblacion.size(); i++){
        soluciones.push_back(poblacion[i].getAsignacion());
    }
    unordered_set<vector<bool>> unique(soluciones.begin(), soluciones.end());
    return unique.size();
}


using CruceFunc =  void (*)(Mochila& c1, Mochila& c2, int W);

vector<bool> AGG(vector<Mochila>& poblacion_actual, const int cromosomas, const int W, int& iteracion, const int& max_iteraciones, const double& prob_cruce,
                 const double& prob_mutacion, int& tam_poblacion, CruceFunc cruce){

    while (iteracion<=max_iteraciones){
        //seleccion
        vector<Mochila> p_nueva = poblacion_actual;
        for (int i=0; i<tam_poblacion; i++){
            int c1 = Random::get(0, tam_poblacion-1);
            int c2 = Random::get(0, tam_poblacion-1);
            int c3 = Random::get(0, tam_poblacion-1);
            p_nueva[i] = torneo(poblacion_actual[c1], poblacion_actual[c2], poblacion_actual[c3]);
        }
        
        //cruce
        int cruces_esperados = (tam_poblacion/2)*prob_cruce;
        for(int i=0; i<cruces_esperados; i+=2){
            cruce(p_nueva[i], p_nueva[i+1], W);
        }

        //mutacion
        int mutaciones_esperadas = tam_poblacion*prob_mutacion;
         for (int i=0; i<mutaciones_esperadas; i++){
              int individuo = Random::get(0, tam_poblacion-1);
              int p = p_nueva[individuo].getPeso();
              p_nueva[individuo].setAsignacionIndice(mutacion(p_nueva[individuo].getAsignacion(), p, W));
        }

        for (int i=0; i<tam_poblacion; i++){
            p_nueva[i].Fitness();
        }

        // reemplazo
        Mochila mejorPadre = *min_element(poblacion_actual.begin(), poblacion_actual.end(), ordenFitness());
        Mochila peorHijo = *max_element(p_nueva.begin(), p_nueva.end(), ordenFitness());
        Mochila mejorHijo = *min_element(p_nueva.begin(), p_nueva.end(), ordenFitness());

        // Encuentra la peor mochila en la población cruzada y la reemplaza con la mejor mochila de la población actual
        if (mejorPadre.getFitness() > mejorHijo.getFitness()){
            replace(p_nueva.begin(), p_nueva.end(), peorHijo, mejorPadre);
        }

        // Copia la población cruzada en la población actual
        copy(p_nueva.begin(), p_nueva.end(), poblacion_actual.begin());
        
        iteracion += tam_poblacion;
    }

    return min_element(poblacion_actual.begin(), poblacion_actual.end(), ordenFitness())->getAsignacion();
}

 

vector<bool> AGE(vector<Mochila>& poblacion_actual, const int cromosomas, const int W, int& iteracion, const int& max_iteraciones, const double& prob_cruce,
                 const double& prob_mutacion, int& tam_poblacion, CruceFunc cruce){
    
    while(iteracion<max_iteraciones){

        //seleccion
        int c1 = Random::get(0, tam_poblacion-1);
        int c2 = Random::get(0, tam_poblacion-1);
        int c3 = Random::get(0, tam_poblacion-1);
        Mochila h1 = torneo(poblacion_actual[c1], poblacion_actual[c2], poblacion_actual[c3]);
        c1 = Random::get(0, tam_poblacion-1);
        c2 = Random::get(0, tam_poblacion-1);
        c3 = Random::get(0, tam_poblacion-1);
        Mochila h2 = torneo(poblacion_actual[c1], poblacion_actual[c2], poblacion_actual[c3]);

        //cruce
        cruce(h1, h2, W);
        
        //mutacion
        if (Random::get(0.0, 1.0) < prob_mutacion){
            //cout << "Fitness antes de mutacion: " << h1.getFitness() << endl;
            int p = h1.getPeso();
            h1.setAsignacionIndice(mutacion(h1.getAsignacion(), p, W));
            //h1.Fitness();
            //cout << "Fitness despues de mutacion: " << h1.getFitness() << endl;
        }  
        if (Random::get(0.0, 1.0) < prob_mutacion){
            //cout << "Mutacion2 en iteracion: " << iteracion << endl;
            int p = h2.getPeso();
            h2.setAsignacionIndice(mutacion(h2.getAsignacion(), p, W));
        }
        

        h1.Fitness();
        h2.Fitness();
        //cout << iteracion << " " << h1.getFitness() << endl;

        //reemplazo
        sort(poblacion_actual.begin(), poblacion_actual.end(), ordenFitness());
        for (int i=0; i<poblacion_actual.size(); i++){
            //cout << poblacion_actual[i].getFitness() << " ";
        }
        //cout << endl;
        vector<Mochila> cambios = {h1, h2, poblacion_actual.back(), poblacion_actual[poblacion_actual.size()-2]};
        sort(cambios.begin(), cambios.end(), ordenFitness());
        for (int i=0; i<4; i++){
            //cout << cambios[i].getFitness() << " ";
        }
        //cout << endl;
        poblacion_actual.back() = cambios[0];
        poblacion_actual[poblacion_actual.size()-2] = cambios[1];

        iteracion+=2;

    }
    return min_element(poblacion_actual.begin(), poblacion_actual.end(), ordenFitness())->getAsignacion();
}