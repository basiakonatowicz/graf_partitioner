#ifndef PODZIAL_ALGORYTMY_H
#define PODZIAL_ALGORYTMY_H

#include "graf.h"

CzescGrafu* podziel_graf(Graf *graf, int liczba_czesci, float margines);
int oblicz_przeciete_krawedzie(Graf *graf, CzescGrafu *czesci, int liczba_czesci);

#endif