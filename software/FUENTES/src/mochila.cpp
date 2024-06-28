#include "../include/mochila.h"

using namespace std;



Mochila::Mochila() {
    asignacion = vector<bool>(n, false);
    beneficio = 0;
    peso = 0;
}

Mochila::Mochila(vector<bool> asignacion) {
    this->asignacion = asignacion;
    Fitness();
}

int Mochila::Fitness() {
    beneficio = 0;
    peso = 0;
    for (int i = 0; i < b_individual.size(); i++) {
      if (asignacion[i]) {
        beneficio += b_individual[i];
        peso += pesos[i];
        for (int j = 0; j < b_individual.size(); j++) {
          beneficio += b_interdependencias[i][j]*asignacion[j];
        }
      }
    }
    return beneficio;
}


vector<bool> Mochila::getAsignacion() const { return asignacion; }


void Mochila::setAsignacion(const vector<bool> &asignacion) { 
    this->asignacion = asignacion;
    Fitness();
}
//
void Mochila::setAsignacionE(const vector<bool> &asignacion){
  this->asignacion = asignacion;
  peso = 0;
  for (int i = 0; i < n; i++) {
    if (asignacion[i]) {
      peso += pesos[i];
    }
  }
}

void Mochila::setAsignacionIndice(const int& indice){
    if (asignacion[indice]){
      peso -= pesos[indice];
    }
    else{
      peso += pesos[indice];
    }
    asignacion[indice] = !asignacion[indice];
}

int Mochila::getFitness() const { return beneficio; }

bool Mochila::operator==(const Mochila& other) const {
    return asignacion == other.asignacion;
}

int Mochila::getPeso() const{
  return peso;
}


int Mochila::size() const{
  return asignacion.size();
}

bool Mochila::swap(int i, int j, int capacidad){
  bool se_pudo = false;
  vector<bool> v = asignacion;
  v[i] = !v[i];
  v[j] = !v[j];
  Mochila nueva(v);
  if (nueva.getPeso() <= capacidad){
    asignacion = v;
    se_pudo = true;
  }
  return se_pudo;
}

void Mochila::factorizacionFitness(const vector<bool>& nueva_asignacion, const int& i, const int& j){
  asignacion = nueva_asignacion;
  beneficio += b_individual[i];
  beneficio -= b_individual[j];
  peso += pesos[i];
  peso -= pesos[j];
  for (int k = 0; k < n; k++) {
    beneficio += b_interdependencias[k][i] * asignacion[k];
    beneficio -= b_interdependencias[k][j] * asignacion[k];
  }
}