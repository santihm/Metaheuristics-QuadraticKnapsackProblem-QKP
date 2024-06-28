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
#include "../include/greedy.h"
#include "../include/mochila.h"
#include "../include/objeto.h"
#include "../include/geneticos.h"
#include "../include/memeticos.h"
#include "../include/trayectorias.h"

#define _MAIN 1


using namespace std;
using namespace std::chrono;
using Random = effolkronium::random_static;

int n; //Nº objetos
vector<int> pesos;
vector<vector<int >> b_interdependencias;
vector<int> b_individual;
vector<Objeto> objeto;

vector<vector<int>> convertir_matriz_simetrica(const vector<vector<int>>& b_interdependencias) {
  int n = b_interdependencias.size();
  vector<vector<int>> matriz_simetrica(n, vector<int>(n, 0));

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      matriz_simetrica[i][j] = b_interdependencias[i][j - i - 1];
      matriz_simetrica[j][i] = matriz_simetrica[i][j]; // Fill the symmetric counterpart
    }
  }

  return matriz_simetrica;
}




int main(int argc, char* argv[]){

  if (argc <= 2) {
    cerr << "Error: Se requieren argumentos. Ayuda:" << endl;
    cout << "$ejecutable $algoritmo(G o BL) $archivo ($semilla)\n";
    return 1;
  }

  // Obtener el primer argumento
  string algoritmo = argv[1];

  // Variable booleana

  // Evaluar el argumento
  if (!(algoritmo == "BL" || algoritmo == "BL-DOBLE" || algoritmo == "BL-MEJOR" || algoritmo == "BL-MEJOR-DOBLE" || algoritmo == "G"
        || algoritmo == "AGG-dospuntos" || algoritmo == "AGG-propuesta" || algoritmo == "AGE-dospuntos" || algoritmo == "AGE-propuesta" 
        || algoritmo == "AM-AGG-dospuntos-10,1.0" || algoritmo == "AM-AGG-dospuntos-10,0.1" || algoritmo == "AM-AGG-dospuntos-10,0.1mej" 
        || algoritmo == "AM-AGG-propuesta-10,1.0" || algoritmo == "AM-AGG-propuesta-10,0.1" || algoritmo == "AM-AGG-propuesta-10,0.1mej"
        || algoritmo == "AM-AGE-dospuntos-10,1.0" || algoritmo == "AM-AGE-dospuntos-10,0.1" || algoritmo == "AM-AGE-dospuntos-10,0.1mej" 
        || algoritmo == "AM-AGE-propuesta-10,1.0" || algoritmo == "AM-AGE-propuesta-10,0.1" || algoritmo == "AM-AGE-propuesta-10,0.1mej"
        || algoritmo == "BMB" || algoritmo == "ILS" || algoritmo == "ES" || algoritmo == "ILS-ES")){
    cerr << "Error: Argumento no válido. Debe ser 'BL' o 'G'. Ayuda: " << endl;
    cout << "$ejecutable $algoritmo(G o BL) $archivo ($semilla)" << endl;
    return 1;
  }

    

    int W; // capacidad
    char * ruta_fichero(argv[2]);
    ifstream f;
    f.open(ruta_fichero);

    if (f.fail()) {
    cerr << "Error al abrir el fichero." << endl;
     return 1;
    }

    string linea;
    getline(f, linea);
    f >> n; // Lee tamaño

    int b;
    //Leemos beneficios individuales
    for (int i = 0; i < n; ++i) {
        int beneficio;
        f >> b;
        b_individual.push_back(b);
    }
    // Leemos beneficios por interdependicias
    vector<vector<int >> beneficios_interdependencias;
    for (int i = 0; i < n; ++i) {
        vector<int> fila;
        for (int j = i + 1; j < n; ++j) {
            f >> b;
            fila.push_back(b);
        }
        beneficios_interdependencias.push_back(fila);
    }

    b_interdependencias = convertir_matriz_simetrica(beneficios_interdependencias);

    f >> W;
    //Leemos capacidad mochila
    f >> W;
    //cout << "W: " << W << endl;

    //Leemos pesos
    for (int i = 0; i < n; ++i) {
        f >> b;
        pesos.push_back(b);
    }

    //Cerramos fichero
    while (getline(f, linea)) {}
    f.close();


    
    for (int i=0; i<n; i++){
        Objeto o(i, b_individual[i], b_interdependencias[i], pesos[i]);
        objeto.push_back(o);
    }

    int peso_final;
    int beneficio_final = 0;
    vector<bool> asignacion;
    int n_instancias;
    const int max_instancias = 90000;
    int max_grupos = 1;
    bool maximo_local;
    int tam_poblacion = 50;
    double probabilidadCruce = 0.68;
    double probabilidadMutacion = 0.08;
    milliseconds tiempo;

    if (algoritmo == "G"){
      auto momentoInicio = high_resolution_clock::now();
      asignacion = greedy(objeto, n, W, peso_final);
      auto momentoFin = high_resolution_clock::now();
      tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
    }
    else{
      long int seed;

      if (argc <= 3) {
        //cout <<"Sin semilla establecida" <<endl;
      }
      else {
        seed = atoi(argv[3]);
        Random::seed(seed);
        //cout <<"Usando semilla: " <<seed <<endl;
      }
      vector<Mochila> poblacion = poblacionInicial(n, W, tam_poblacion);
      Mochila mochila = solucionInicial(n, W);
      //cout << mochila.getPeso() << endl;
      if (algoritmo == "BL"){
        n_instancias = 1;
        auto momentoInicio = high_resolution_clock::now();
        asignacion = busquedaLocal(mochila, n, W, max_instancias, n_instancias, maximo_local, VecinosPermutacion, -1);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "G"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = greedy(objeto, n, W, peso_final);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }//
      else if (algoritmo == "BL-DOBLE"){
        auto momentoInicio = high_resolution_clock::now();
        //asignacion = busquedaLocal(mochila, n, W, max_instancias, n_instancias, maximo_local, VecinosDobleIntercambio, -1);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "BL-MEJOR"){
        auto momentoInicio = high_resolution_clock::now();
        //asignacion = busquedaLocalMejor(mochila, n, W, max_instancias, n_instancias, maximo_local, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "BL-MEJOR-DOBLE"){
        auto momentoInicio = high_resolution_clock::now();
        //asignacion = busquedaLocalMejor(mochila, n, W, max_instancias, n_instancias, maximo_local, VecinosDobleIntercambio);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AGG-dospuntos"){
        int iteracion = tam_poblacion;
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AGG(poblacion, n, W, iteracion, max_instancias, probabilidadCruce, probabilidadMutacion, tam_poblacion, cruceDosPuntos);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AGG-propuesta"){
        int iteracion = tam_poblacion;
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AGG(poblacion, n, W, iteracion, max_instancias, probabilidadCruce, probabilidadMutacion, tam_poblacion, cruceUniformePesado);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AGE-dospuntos"){
        int iteracion = tam_poblacion;
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AGE(poblacion, n, W, iteracion, max_instancias, 1, probabilidadMutacion, tam_poblacion, cruceDosPuntos);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AGE-propuesta"){
        int iteracion = tam_poblacion;
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AGE(poblacion, n, W, iteracion, max_instancias, 1, probabilidadMutacion, tam_poblacion, cruceUniformePesado);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AM-AGG-dospuntos-10,1.0"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AM(poblacion, n, W, max_instancias, probabilidadCruce, probabilidadMutacion, tam_poblacion, cruceDosPuntos,
                        AGG, 10, 1.0, false, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AM-AGG-dospuntos-10,0.1"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AM(poblacion, n, W, max_instancias, probabilidadCruce, probabilidadMutacion, tam_poblacion, cruceDosPuntos,
                        AGG, 10, 0.1, false, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AM-AGG-dospuntos-10,0.1mej"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AM(poblacion, n, W, max_instancias, probabilidadCruce, probabilidadMutacion, tam_poblacion, cruceDosPuntos,
                        AGG, 10, 0.1, true, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AM-AGG-propuesta-10,1.0"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AM(poblacion, n, W, max_instancias, probabilidadCruce, probabilidadMutacion, tam_poblacion, cruceUniformePesado,
                        AGG, 10, 1.0, false, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AM-AGG-propuesta-10,0.1"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AM(poblacion, n, W, max_instancias, probabilidadCruce, probabilidadMutacion, tam_poblacion, cruceUniformePesado,
                        AGG, 10, 0.1, false, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AM-AGG-propuesta-10,0.1mej"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AM(poblacion, n, W, max_instancias, probabilidadCruce, probabilidadMutacion, tam_poblacion, cruceUniformePesado,
                        AGG, 10, 0.1, true, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AM-AGE-dospuntos-10,1.0"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AM(poblacion, n, W, max_instancias, probabilidadCruce, probabilidadMutacion, tam_poblacion, cruceDosPuntos,
                        AGE, 10, 1.0, false, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AM-AGE-dospuntos-10,0.1"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AM(poblacion, n, W, max_instancias, probabilidadCruce, probabilidadMutacion, tam_poblacion, cruceDosPuntos,
                        AGE, 10, 0.1, false, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AM-AGE-dospuntos-10,0.1mej"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AM(poblacion, n, W, max_instancias, probabilidadCruce, probabilidadMutacion, tam_poblacion, cruceDosPuntos,
                        AGE, 10, 0.1, true, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AM-AGE-propuesta-10,1.0"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AM(poblacion, n, W, max_instancias, probabilidadCruce, probabilidadMutacion, tam_poblacion, cruceUniformePesado,
                        AGE, 10, 1.0, false, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AM-AGE-propuesta-10,0.1"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AM(poblacion, n, W, max_instancias, probabilidadCruce, probabilidadMutacion, tam_poblacion, cruceUniformePesado,
                        AGE, 10, 0.1, false, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "AM-AGE-propuesta-10,0.1mej"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = AM(poblacion, n, W, max_instancias, probabilidadCruce, probabilidadMutacion, tam_poblacion, cruceUniformePesado,
                        AGE, 10, 0.1, true, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "BMB"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = BMB(20, 4500, n, W);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "ILS"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = ILS(20, 4500, n, W);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "ES"){
        n_instancias = 1;
        auto momentoInicio = high_resolution_clock::now();
        asignacion = ES(mochila, n, W, max_instancias, n_instancias, 0.3, 0.1, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "ILS-ES"){
        n_instancias = 1;
        auto momentoInicio = high_resolution_clock::now();
        asignacion = ILS_ES(20, 4500, n, W);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      
    }
    Mochila mochila(asignacion);
    beneficio_final = mochila.getFitness();
    peso_final = mochila.getPeso();
    
    ///*
    cout << "W: " << W;
    cout << "\nPeso final = " << peso_final;
    cout << "\nBeneficio final = " << beneficio_final << endl;
    cout << "Asignacion:\n";
    for (int i=0; i<asignacion.size(); i++){
        cout << asignacion[i] << " ";
    }
    cout << endl;
    if ((algoritmo=="BL" or algoritmo=="BL_DOBLE") && maximo_local){
      cout << "Se ha alcanzado un Máximo Local\n";
      cout << "Número de instancias: " << n_instancias << endl; 
    }
    cout << endl;
    //*///
    
    
    
    cout << beneficio_final<< " " <<tiempo.count() <<endl;

}