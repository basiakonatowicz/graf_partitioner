#ifndef WEJSCIE_WYJSCIE_H
#define WEJSCIE_WYJSCIE_H

#include "graf.h"

Graf* wczytaj_graf_z_pliku(const char *nazwa_pliku);
void zapisz_wynik_do_pliku(const char *nazwa_pliku, CzescGrafu *czesci, int liczba_czesci, int przeciete_krawedzie, int zapis_binarny);
void zapisz_wynik_do_pliku_nowy(const char *nazwa_pliku, Graf *graf, CzescGrafu *czesci, int liczba_czesci, int przeciete_krawedzie, float margines, int zapis_binarny);

#endif