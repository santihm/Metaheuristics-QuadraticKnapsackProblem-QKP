#include "../include/greedy.h"

using namespace std;


bool compare_ratios(const Objeto& obj1, const Objeto& obj2) {
  float ratio_obj1 = obj1.getB_Acumulado() / obj1.getPeso();
  float ratio_obj2 = obj2.getB_Acumulado() / obj2.getPeso();
  return ratio_obj1 < ratio_obj2;
}

int particion(vector<Objeto> &objeto, int bajo, int alto) {
    Objeto pivote = objeto[alto];
    int i = bajo - 1;

    for (int j = bajo; j < alto; j++) {
        if (compare_ratios(objeto[j], pivote)) {
            i++;
            swap(objeto[i], objeto[j]);
        }
    }
    swap(objeto[i + 1], objeto[alto]);
    return i + 1;
}

void quicksort(vector<Objeto> &objeto, int bajo, int alto) {
    if (bajo < alto) {
        int pi = particion(objeto, bajo, alto);

        quicksort(objeto, bajo, pi - 1);
        quicksort(objeto, pi + 1, alto);
    }
}

void OrdenGlobal(vector<Objeto> &objeto) {
    int n = objeto.size();
    quicksort(objeto, 0, n - 1);
}






vector<bool> greedy(vector<Objeto> objeto, const int n, const int W, int &peso_final){
  vector<bool> v(n);
  int capacidad = W;
  //vector<Objeto> o = objeto;
  OrdenGlobal(objeto);
  while (objeto.size()>0){
    if (capacidad >= objeto[objeto.size()-1].getPeso()){
      capacidad -= objeto[objeto.size()-1].getPeso();
      v[objeto[objeto.size()-1].getIndice()] = 1;
      for (int j=0; j<objeto.size()-1; j++){
        objeto[j].aniadeB_Acumulado(objeto[j].getB_Interdependencias(objeto[objeto.size()-1].getIndice()));
      }
      objeto.pop_back();
      OrdenGlobal(objeto);
    }
    else{
      objeto.pop_back();
    }
    //Mochila m (o, v);
    //cout << m.getFitness() << endl;
  }
  return v;
}