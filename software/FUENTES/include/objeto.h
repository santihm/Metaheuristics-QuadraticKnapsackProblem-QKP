#ifndef OBJETO_H  // Corrected typo here
#define OBJETO_H

#include <iostream>
#include <vector>

using namespace std;

class Objeto {
public:
    Objeto();
    Objeto(int i, int b_i, vector<int> b_inter, int p);

    int getIndice() const;
    int getB_Individual() const;
    int getB_Acumulado() const;
    int getB_Interdependencias(const int& j) const;
    int getPeso() const;
    void aniadeB_Acumulado(int b);
    bool operator==(const Objeto& other) const;


    int indice;
    int b_individual;
    vector<int> b_interdependencias;
    int peso;
    int b_acumulado;
};

#endif // OBJETO_H
