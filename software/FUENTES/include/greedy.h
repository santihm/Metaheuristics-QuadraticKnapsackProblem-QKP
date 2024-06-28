#ifndef GREEDY_H
#define GREEDY_H

#include <vector>
#include "mochila.h"

using namespace std;


bool compare_ratios(const Objeto& obj1, const Objeto& obj2);
int particion(vector<Objeto>& objeto, int bajo, int alto);
void quicksort(vector<Objeto>& objeto, int bajo, int alto);
void OrdenGlobal(vector<Objeto>& objeto);
vector<bool> greedy(vector<Objeto> objeto, const int n, const int W, int& peso_final);

#endif // GREEDY_H
