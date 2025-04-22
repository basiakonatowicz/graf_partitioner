#ifndef WEJSCIE_WYJSCIE_H
#define WEJSCIE_WYJSCIE_H

#include "graf.h"

Graf* wczytaj_graf_z_pliku(const char *nazwa_pliku);
void zapisz_wynik_do_pliku(const char *nazwa_pliku, CzescGrafu *czesci, int liczba_czesci, int przeciete_krawedzie, int zapis_binarny);

#endif