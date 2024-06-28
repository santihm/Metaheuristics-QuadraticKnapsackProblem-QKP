#include <iostream>
#include <vector>
#include "../include/objeto.h"

using namespace std;

Objeto::Objeto(){
    b_individual = 0;
    indice = 0;
    peso = 0;
}

Objeto::Objeto(int i, int b_i, vector<int> b_inter, int p){
    indice = i;
    b_individual = b_i;
    b_interdependencias = b_inter;
    peso = p;
    b_acumulado = b_i;
}

int Objeto::getIndice() const{
    return indice;
}
    
int Objeto::getB_Individual() const{
    return b_individual;
}

int Objeto::getB_Acumulado() const{
    return b_acumulado;
}

void Objeto::aniadeB_Acumulado(int b){
    b_acumulado += b;
}
    
int Objeto::getB_Interdependencias(const int& j) const{
    return b_interdependencias[j];
}

int Objeto::getPeso() const{
    return peso;
}

bool Objeto::operator==(const Objeto& other) const {
    return indice == other.indice && 
           b_individual == other.b_individual && 
           b_interdependencias == other.b_interdependencias && 
           peso == other.peso;
}

