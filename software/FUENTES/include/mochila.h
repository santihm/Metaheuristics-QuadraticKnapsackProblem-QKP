#ifndef MOCHILA_H // Replace MOCHILA_H with the appropriate guard name for each file
#define MOCHILA_H
#include <iostream>
#include <vector>
#include "objeto.h"

using namespace std;

#ifndef _MAIN
extern int n; //Nº objetos
extern vector<int> pesos;
extern vector<vector<int >> b_interdependencias;
extern vector<int> b_individual;
#endif

class Mochila{
    private:
        vector<bool> asignacion;
        int beneficio;
        int peso;
    public:
    Mochila();
    Mochila(vector<bool> asignacion);

    int Fitness();
    vector<bool> getAsignacion() const;
    void setAsignacion(const vector<bool> &asignacion);
    void setAsignacionE(const vector<bool> &asignacion);
    void setAsignacionIndice(const int& indice);
    int size() const;
    int getFitness() const;
    int getPeso() const;
    bool operator==(const Mochila& other) const;
    bool swap(int i, int j, int capacidad);
    void factorizacionFitness(const vector<bool>& nueva_asignacion, const int& i, const int& j);
};
#endif // MOCHILA_H